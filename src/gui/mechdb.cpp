#include "mechdb.h"

using namespace Mix;

MechDB::MechDB(dlgtype_t _dlgtype, QWidget *parent) :
    QDialog(parent),
	ui(new Ui::mechdb),
	dlgtype(_dlgtype)
{
	ui->setupUi(this);

	// set the title as appropriate
	this->setWindowTitle((dlgtype==save ? "Save Mech to" : "Load Mech from") + tr(" DB..."));

	// add the current list of mechanisms to the listbox
	ui->mechlist->addItems(Mix::db.mech_list());

	// set the name box to read-only if loading
	if( dlgtype==load )
		ui->mechname->setReadOnly(true);
	else
		ui->mechname->setReadOnly(false);

	// connect sql information
	db.connectToServer();
	updateInfo();
	connect( &db, SIGNAL(hostnameChanged(QString)), this, SLOT(updateInfo()));
	connect( &db, SIGNAL(dbChanged(QString)), this, SLOT(updateInfo()));
	connect( &db, SIGNAL(usernameChanged(QString)), this, SLOT(updateInfo()));
	connect( &db, SIGNAL(portChanged(int)), this, SLOT(updateInfo()));
	connect( &db, SIGNAL(connectionStatus(QString)), this, SLOT(updateInfo()));

	connect( ui->configure, SIGNAL(clicked()), this, SLOT(sqlConfig()));
}

void MechDB::sqlConfig()
{
	SqlConfig* config = new SqlConfig(this);
	bool ok = config->exec();
	
	if(!ok) return;
	
	// set values
	db.setHostname( config->getHostname() );
	db.setUsername( config->getUsername() );
	db.setPassword( config->getPassword() );
}

void MechDB::updateInfo()
{
	bool con = db.isConnected();
	ui->lblInfo->setText( "Server:  "
						  + ( con ? db.getUsername() + "@" + db.getHostname() + ", " + db.getDB()
						  : "Could not connect to host." ) );
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
