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

	// set the name box to read-only if loading
	if( dlgtype==load )
		ui->mechname->setReadOnly(true);
	else
		ui->mechname->setReadOnly(false);

	// connect sql information
	
	updateInfo();
	connect( &db, SIGNAL(hostNameChanged(QString)), this, SLOT(updateInfo()));
	connect( &db, SIGNAL(databaseChanged(QString)), this, SLOT(updateInfo()));
	connect( &db, SIGNAL(userNameChanged(QString)), this, SLOT(updateInfo()));
	connect( &db, SIGNAL(portChanged(int)), this, SLOT(updateInfo()));
	connect( &db, SIGNAL(connectionStatus(QString)), this, SLOT(updateInfo()));

	connect( ui->configure, SIGNAL(clicked()), this, SLOT(sqlConfig()));
	
	connect( ui->mechname, SIGNAL(textChanged(QString)), this, SLOT(updateList()) );
	connect( ui->refresh, SIGNAL(clicked()), this, SLOT(refreshList()) );
}

void MechDB::sqlConfig()
{
	SqlConfig* config = new SqlConfig(this);
	bool ok = config->exec();
	
	if(!ok) return;
}

void MechDB::updateInfo()
{
	bool con = db.sqldb.open();
	ui->lblInfo->setText( db.getUserName() + "@" + db.getHostName() );
	ui->lblInfo->setEnabled( con );
	ui->frameMechs->setEnabled( con );
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled( con );
	
	refreshList();
}

void MechDB::refreshList()
{
	ui->mechlist->clear();
	// add the current list of mechanisms to the listbox
	ui->mechlist->addItems(db.mechList());
}

void MechDB::updateList()
{
	// search for text in mechname within mechlist
	QList<QListWidgetItem*> list = ui->mechlist->findItems(ui->mechname->text(), Qt::MatchFixedString|Qt::MatchCaseSensitive);
	if( !list.isEmpty() )
		ui->mechlist->setCurrentItem(list.first());
	else
		ui->mechlist->setCurrentRow(-1);
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
