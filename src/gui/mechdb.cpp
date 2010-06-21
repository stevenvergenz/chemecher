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

	ui->buttonBox->addButton(ui->configure, QDialogButtonBox::ActionRole);
	
	// connect sql information	
	updateInfo();
	connect( &db, SIGNAL(hostNameChanged(QString)), this, SLOT(updateInfo()));
	connect( &db, SIGNAL(databaseChanged(QString)), this, SLOT(updateInfo()));
	connect( &db, SIGNAL(userNameChanged(QString)), this, SLOT(updateInfo()));
	connect( &db, SIGNAL(passwordChanged(QString)), this, SLOT(updateInfo()));
	connect( &db, SIGNAL(portChanged(int)), this, SLOT(updateInfo()));
	connect( &db, SIGNAL(connectionStatus(QString)), this, SLOT(updateInfo()));

	// connect the buttons
	connect( ui->configure, SIGNAL(clicked()), this, SLOT(sqlConfig()));
	connect( ui->mechname, SIGNAL(textChanged(QString)), this, SLOT(updateList()) );
	connect( ui->refresh, SIGNAL(clicked()), this, SLOT(updateInfo()) );
}

// overloaded to determine the operation
void MechDB::accept()
{
	if( dlgtype==save )
		db.saveMech(ui->mechname->text());
	else
		db.loadMech(ui->mechname->text());
	QDialog::accept();
}

void MechDB::sqlConfig()
{
	SqlConfig* config = new SqlConfig(this);
	config->exec();
}

void MechDB::updateInfo()
{
	bool con = db.sqldb.open();
	
	// set the info box
	ui->lblInfo->setText( db.getUserName() + "@" + db.getHostName() );
	
	// enable/disable the form according to whether a connection could be established
	ui->infmech     ->setEnabled( con );
	ui->infmechname ->setEnabled( con );
	ui->mechname    ->setEnabled( con );
	ui->mechlist    ->setEnabled( con );
	ui->buttonBox->button(QDialogButtonBox::Ok)
			        ->setEnabled( con );
	
	// set listbox settings
	ui->mechlist->setSortingEnabled( con );
	ui->mechlist->setSelectionMode(
			(con ? QAbstractItemView::SingleSelection : QAbstractItemView::NoSelection ) );
	ui->mechlist->clear();
	
	// get list of mechanisms
	if( con ) {
		ui->mechlist->addItems(db.mechList());
		ui->mechlist->setEnabled(true);
		if( ui->mechlist->count()==0 ) {
			ui->mechlist->addItem("No mechanisms to display.");
			ui->mechlist->setEnabled(false);
		}
	}
	// give the error information
	else {
		ui->mechlist->addItem("Could not connect to database.");
		ui->mechlist->addItem("Please ensure server exists and has");
		ui->mechlist->addItem("   database `chemecher`.");
		ui->mechlist->addItem("If database is not configured, run");
		ui->mechlist->addItem("   mysql < configdb.sql");
	}
	
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
