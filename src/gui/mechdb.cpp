#include "mechdb.h"

using namespace Mix;

MechDB::MechDB(dlgtype_t _dlgtype, QWidget *parent) :
    QDialog(parent),
	ui(new Ui::mechdb),
	dlgtype(_dlgtype)
{
	ui->setupUi(this);

	// set the title as appropriate
	this->setWindowTitle((dlgtype==save ? tr("Save Mech to") : tr("Load Mech from")) + tr(" DB.."));

	// add the current list of mechanisms to the listbox
	ui->mechlist->addItems(Mix::db->mech_list());

	// set the name box to read-only if loading
	if( dlgtype==load )
		ui->mechname->setReadOnly(true);
	else
		ui->mechname->setReadOnly(false);

	// connect sql information
	updateInfo();
	QObject::connect( db, SIGNAL(hostnameChanged(QString)), this, SLOT(updateInfo()));
	QObject::connect( db, SIGNAL(dbChanged(QString)), this, SLOT(updateInfo()));
	QObject::connect( db, SIGNAL(usernameChanged(QString)), this, SLOT(updateInfo()));
	QObject::connect( db, SIGNAL(portChanged(int)), this, SLOT(updateInfo()));
	QObject::connect( db, SIGNAL(connectionStatus(QString)), this, SLOT(updateInfo()));

	connect( ui->configure, SIGNAL(clicked()), this, SLOT(sqlConfig()));
}

void MechDB::sqlConfig()
{
	SqlConfig* config = new SqlConfig(this);
	bool ok = config->exec();
	
	if(!ok) return;
	
	// set values
	/*db->setHostName( config->getHostname() );
	db->setUserName( config->getUsername() );
	db->setPassword( config->getPassword() );*/
	
	/*db->connectToServer();
	if( !db->isConnected() ) {
		QErrorMessage* msg = new QErrorMessage(this);
		msg->showMessage("Could not connect to server.");
	}*/
	updateInfo();
}

void MechDB::updateInfo()
{
	bool con = db->isOpen();
	ui->lblInfo->setText( db->userName() + "@" + db->hostName() );
	ui->lblInfo->setEnabled(con);
	ui->frameMechs->setEnabled(con);
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(con);
}

MechDB::~MechDB()
{
    delete ui;
}

void MechDB::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
