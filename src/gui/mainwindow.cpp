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

	mdi->addSubWindow( tempstep );
	mdi->addSubWindow( tempcpd );*/

	//setWindowState( windowState() | Qt::WindowMaximized );

	ui.lstSteps->setStatusTip("Put your steps here");
	
	///////////////////
	// SIGNALS/SLOTS //
	///////////////////
	
	// cpd and step
	connect(ui.pushAddCpd, SIGNAL(clicked()), this, SLOT(addCpd()));
	connect(mix, SIGNAL(addedCpd(Cpd*)), this, SLOT(updateCpdList()));
	connect(mix, SIGNAL(addedStep(Step*)), this, SLOT(updateStepList()));
	connect(ui.lstCpds, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
			this, SLOT(editCpdWindow(QListWidgetItem*)));
	
	
	// saving/loading
	connect(ui.actSaveMechDb, SIGNAL(triggered()), this, SLOT(saveMechDb()));
	connect(ui.actLoadMechDb, SIGNAL(triggered()), this, SLOT(loadMechDb()));
	
	// exit
	connect(ui.actExit, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(qApp, SIGNAL(aboutToQuit()), mix->db, SLOT(closeDb()));
}

// saving/loading ////
//

void MainWindow::saveMechDb()
{
	// run the dialog box
	MechDB* dialog = new MechDB(MechDB::save, this);
	dialog->exec();
}
void MainWindow::loadMechDb()
{
	// run the dialog box
	MechDB* dialog = new MechDB(MechDB::load, this);
	dialog->exec();
}

//
////////////////////////////////

// compound editing workings ////
//

void MainWindow::addCpd()
{
	CpdWindow* win = new CpdWindow(new Cpd(), this, 0, true);
	mdi->addSubWindow(win);
	win->show();
	qDebug() << ":D";
	return;
	
	//populate the fields and validate
	/*NewCpdDialog* dialog = new NewCpdDialog(this);
	int rtn = dialog->exec();

	//if the add was rejected/canceled, return
	if(rtn == 0) return;

	//populate the new cpd object
	Cpd* newcpd = new Cpd();
	newcpd->setShortName( dialog->getName() );
	newcpd->setState( dialog->getState() );

	//add to the list
	mix->CpdList.append( newcpd );
	ui.lstCpds->addItem( newcpd->toString() );
	CpdWindow* tempcpd = new CpdWindow(newcpd);
	mdi->addSubWindow( tempcpd );
	tempcpd->show();*/
}

void MainWindow::editCpdWindow(QListWidgetItem* item)
{
	//get the cpd
	Cpd* cpd = mix->CpdList.at(
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
	CpdWindow* newCpdWindow = new CpdWindow(cpd, this);
	mdi->addSubWindow( newCpdWindow );
	newCpdWindow->showNormal();
	newCpdWindow->raise();
	newCpdWindow->setFocus();
}

void MainWindow::updateCpdList()
{
	ui.lstCpds->clear();
	QListIterator<QString> i(mix->cpdIdList());
	for(;i.hasNext();i.next())
		ui.lstCpds->addItem(i.peekNext());
}
void MainWindow::updateStepList()
{
	ui.lstSteps->clear();
	QListIterator<QString> i(mix->stepNameList());
	for(;i.hasNext();i.next())
		ui.lstSteps->addItem(i.peekNext());
}

//
////////////////////////////////
