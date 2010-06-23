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
	
	// cpd
	connect(ui.pushAddCpd,    SIGNAL(clicked()), this, SLOT(addCpd()));
	connect(ui.pushRemoveCpd, SIGNAL(clicked()), this, SLOT(removeCpd()));
	connect(mix, SIGNAL(addedCpd(Cpd*)), this, SLOT(updateCpdList()));
	connect(ui.lstCpds, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
			this, SLOT(editCpdWindow(QListWidgetItem*)));
	connect(ui.pushMoveCpdUp,   SIGNAL(clicked()), this, SLOT(moveCpdUp()));
	connect(ui.pushMoveCpdDown, SIGNAL(clicked()), this, SLOT(moveCpdDown()));
	
	// step
	connect(mix, SIGNAL(addedStep(Step*)), this, SLOT(updateStepList()));
	
	// saving/loading
	connect(ui.actSaveMechDb, SIGNAL(triggered()), this, SLOT(saveMechDb()));
	connect(ui.actLoadMechDb, SIGNAL(triggered()), this, SLOT(loadMechDb()));
	
	// exit
	connect(ui.actExit, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(qApp, SIGNAL(aboutToQuit()), mix->db, SLOT(closeDb()));
	
	mix->addCpd(new Cpd("A", Cpd::HOMO));
	mix->addCpd(new Cpd("Z", Cpd::HOMO));
	mix->addCpd(new Cpd("X", Cpd::HOMO));
	//updateCpdList();
	
}

// compound editing workings ////
//

void MainWindow::addCpd()
{
	CpdWindow* win = new CpdWindow(new Cpd(), this, 0, true);
	mdi->addSubWindow(win);
	win->show();
}

void MainWindow::removeCpd()
{
	Cpd *cpd = mix->getCpdById(ui.lstCpds->item(ui.lstCpds->currentRow())->text());
	mix->removeCpd(cpd);
	delete cpd;
	updateCpdList();
	
	QList<QMdiSubWindow*> windowlist = mdi->subWindowList(QMdiArea::ActivationHistoryOrder);
	for(int i=0; i<windowlist.size(); i++)
	{
		if( windowlist[i]->windowTitle() == cpd->toString() ){
			mdi->removeSubWindow(windowlist[i]);
			delete windowlist[i];
			return;
		}
	}
}

void MainWindow::editCpdWindow(QListWidgetItem* item)
{
	//get the cpd
	Cpd* cpd = mix->CpdList.at(
			item->listWidget()->row(item) );
	
	qDebug() << item->listWidget()->row(item);
	qDebug() << cpd->toString();
	
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

void MainWindow::moveCpdUp()
{
	QListWidget *list = ui.lstCpds;
	int cur = list->currentRow();
	if( cur < 1 )
		return;
	Cpd *temp;
	temp = mix->CpdList[cur];
	mix->CpdList[cur] = mix->CpdList[cur-1];
	mix->CpdList[cur-1] = temp;
	updateCpdList();
	list->setCurrentRow(cur-1);
}
void MainWindow::moveCpdDown()
{
	QListWidget *list = ui.lstCpds;
	int cur = list->currentRow();
	if( cur==-1 || cur==list->count()-1 )
		return;
	Cpd *temp;
	temp = mix->CpdList[cur];
	mix->CpdList[cur] = mix->CpdList[cur+1];
	mix->CpdList[cur+1] = temp;
	updateCpdList();
	list->setCurrentRow(cur+1);
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
