#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	//set up the mdi area
	mdi = new QMdiArea();
	mdi->setObjectName("mdi");
	setCentralWidget(mdi);

	/*Cpd* acpd = new Cpd("A", Cpd::AQ);
	Step* astep = new Step();

	StepWindow* tempstep = new StepWindow(astep);
	CpdWindow* tempcpd = new CpdWindow(acpd);

	windowlist.append( mdi->addSubWindow( tempstep ) );
	windowlist.append( mdi->addSubWindow( tempcpd ) );
	*/

	setWindowState( windowState() | Qt::WindowMaximized );

	ui.lstSteps->setStatusTip("Put your steps here");

	/*************************************************************/
	/***************** connect signals and slots *****************/
	/*************************************************************/
	connect(ui.pushAddCpd,	SIGNAL(clicked()),
			this,			SLOT(addNewCpd())
	);
	connect(ui.lstSpecies,	SIGNAL(itemDoubleClicked(QListWidgetItem*)),
			this,			SLOT(showCpdWindow(QListWidgetItem*))
	);
	connect(ui.actSaveMechDb, SIGNAL(triggered()),
			this,			SLOT(saveMechDb()));
	connect(ui.actLoadMechDb, SIGNAL(triggered()),
			this,			SLOT(loadMechDb()));
	connect(ui.actExit, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::saveMechDb()
{
	// run the dialog box
	MechDB* dialog = new MechDB(MechDB::save, this);
	int rtn = dialog->exec();

	if(rtn == 0) return;

}
void MainWindow::loadMechDb()
{
	// run the dialog box
	MechDB* dialog = new MechDB(MechDB::load, this);
	int rtn = dialog->exec();

	if(rtn == 0) return;
}

void MainWindow::addNewCpd()
{
	//populate the fields and validate
	NewCpdDialog* dialog = new NewCpdDialog(this);
	int rtn = dialog->exec();

	//if the add was rejected/canceled, return
	if(rtn == 0) return;

	//populate the new cpd object
	Cpd* newcpd = new Cpd();
	newcpd->setShortName( dialog->getName() );
	newcpd->setState( dialog->getState() );

	//add to the list
	Mix::CpdList.append( newcpd );
	ui.lstSpecies->addItem( newcpd->toString() );
	CpdWindow* tempcpd = new CpdWindow(newcpd);
	mdi->addSubWindow( tempcpd );
	tempcpd->show();
}

void MainWindow::showCpdWindow(QListWidgetItem* item)
{
	//get the cpd
	Cpd* cpd = Mix::CpdList.at(
			item->listWidget()->row(item) );

	//see if it already has a window open
	QList<QMdiSubWindow*> windowlist = mdi->subWindowList(QMdiArea::ActivationHistoryOrder);
	for(int i=0; i<windowlist.size(); i++)
	{
		if( windowlist[i]->windowTitle() == cpd->toString() ){
			windowlist[i]->showNormal();
			windowlist[i]->raise();
			windowlist[i]->setFocus();
			return;
		}
	}

	//the window was not found, create a new one
	CpdWindow* newCpdWindow = new CpdWindow(cpd);
	mdi->addSubWindow( newCpdWindow );
	newCpdWindow->showNormal();
	newCpdWindow->raise();
	newCpdWindow->setFocus();
}
