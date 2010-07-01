#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	mdi = ui.mdi;
	
	//ui.lstSteps->setRowCount(0);
	//ui.lstSteps->setColumnCount(0);
	
	// misc stuff
	setCentralWidget(ui.mdi);
	setWindowState( windowState() | Qt::WindowMaximized );
	
	//ui.lstSteps->horizontalHeader()->setResizeMode();
	
	// cpd list drag&drop stuff
	lstCpds = new DragListWidget(ui.fraCpds);
	lstCpds->setSizePolicy(ui.lstSteps->sizePolicy());
	ui.cpdLayout->addWidget(lstCpds, 1,0,2,2, Qt::AlignLeft);
	//lstCpds->show();
	
	///////////////////
	// SIGNALS/SLOTS //
	///////////////////
	
	// cpd
	connect(ui.pushAddCpd,    SIGNAL(clicked()), this, SLOT(showCpdWindow()));
	connect(ui.pushRemoveCpd, SIGNAL(clicked()), this, SLOT(removeCpd()));
	connect(lstCpds, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(showCpdWindow(QTableWidgetItem*)));
	connect(ui.pushMoveCpdUp,   SIGNAL(clicked()), this, SLOT(moveCpdUp()));
	connect(ui.pushMoveCpdDown, SIGNAL(clicked()), this, SLOT(moveCpdDown()));
	connect(mix, SIGNAL(cpdListChanged()), this, SLOT(updateCpdList()));
	
	// step
	connect(ui.pushAddStep,    SIGNAL(clicked()), this, SLOT(addStep()));
	connect(ui.pushRemoveStep, SIGNAL(clicked()), this, SLOT(removeStep()));
	connect(ui.lstSteps, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editStepWindow(QListWidgetItem*)));
	connect(ui.pushMoveStepUp,   SIGNAL(clicked()), this, SLOT(moveStepUp()));
	connect(ui.pushMoveStepDown, SIGNAL(clicked()), this, SLOT(moveStepDown()));
	connect(mix, SIGNAL(stepListChanged()), this, SLOT(updateStepList()));
	
	// saving/loading
	connect(ui.actSaveMechDb, SIGNAL(triggered()), this, SLOT(saveMechDb()));
	connect(ui.actLoadMechDb, SIGNAL(triggered()), this, SLOT(loadMechDb()));
	
	// exit
	connect(ui.actExit, SIGNAL(triggered()), qApp, SLOT(quit()));
	//connect(qApp, SIGNAL(aboutToQuit()), mix->db, SLOT(closeDb()));
	
	mix->addCpd(new Cpd("A", Cpd::HOMO ));
	mix->addCpd(new Cpd("B", Cpd::AQ   ));
	mix->addCpd(new Cpd("C", Cpd::S    ));
	//updateCpdList();
	
	Step *step = new Step();
	step->setName("Decay");
	step->setKPlus(2.41);
	step->setKMinus(6.8);
	
}

// compound editing implementations ////
//

void MainWindow::showCpdWindow( QTableWidgetItem* item )
{
	Cpd *cpd;
	
	// if function was called from Add button
	if( item == 0 ) {
		// make it a new compound
		cpd = new Cpd();
	}
	
	// if function was called from list double-clicked
	else {
		cpd = mix->CpdList.at( item->listWidget()->row(item) );
		
		// if subwindow exists, raise it and return
		QList<QMdiSubWindow*> windowlist = ui.mdi->subWindowList(QMdiArea::ActivationHistoryOrder);
		for(int i=0; i<windowlist.size(); i++) {
			if( windowlist[i]->windowTitle() == cpd->toString() ){
				windowlist[i]->showNormal();
				windowlist[i]->raise();
				windowlist[i]->setFocus();
				return;
			}
		}
	}
	
	// create the subwindow
	CpdWindow* win = new CpdWindow( cpd, this, 0, (item==0) );
	QMdiSubWindow *mdiSubWin = ui.mdi->addSubWindow(win);
	mdiSubWin->setMinimumSize( win->minimumSize() + QSize(10,28) );
	mdiSubWin->setMaximumSize( win->maximumSize() + QSize(10,28) );
	mdiSubWin->show();
	
}
void MainWindow::removeCpd()
{
	if( lstCpds->currentRow()<0 )
		return;
	
	Cpd *cpd; // = mix->getCpdById(lstCpds->item(lstCpds->currentRow())->text());
	mix->removeCpd(cpd);
	//delete cpd;
	updateCpdList();
	
	QList<QMdiSubWindow*> windowlist = ui.mdi->subWindowList(QMdiArea::ActivationHistoryOrder);
	for(int i=0; i<windowlist.size(); i++)
	{
		if( windowlist[i]->windowTitle() == cpd->toString() ){
			ui.mdi->removeSubWindow(windowlist[i]);
			delete windowlist[i];
			return;
		}
	}
}
/*void MainWindow::editCpdWindow(QListWidgetItem* item)
{
	//get the cpd
	Cpd* cpd = mix->CpdList.at(
			item->listWidget()->row(item) );
	
	//see if it already has a window open
	QList<QMdiSubWindow*> windowlist = ui.mdi->subWindowList(QMdiArea::ActivationHistoryOrder);
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
	CpdWindow* win = new CpdWindow(cpd, this);
	QMdiSubWindow *mdiSubWin = ui.mdi->addSubWindow(win);
	mdiSubWin->setMinimumSize(win->minimumSize() + QSize(10,28) );
	mdiSubWin->setMaximumSize(win->maximumSize() + QSize(10,28) );
	mdiSubWin->show();
	mdiSubWin->showNormal();
	mdiSubWin->raise();
	mdiSubWin->setFocus();
}*/
void MainWindow::moveCpdUp()
{
	QTableWidget *list = lstCpds;
	int cur = list->currentRow();
	if( cur < 1 )
		return;
	mix->swapCpds(cur, cur-1);
	//list->setCurrentRow(cur-1);
	list->setCurrentCell(cur-1, 0);
}
void MainWindow::moveCpdDown()
{
	QTableWidget *list = lstCpds;
	int cur = list->currentRow();
	if( cur==-1 || cur==list->rowCount()-1 )
		return;
	mix->swapCpds(cur, cur+1);
	//list->setCurrentRow(cur+1);
	list->setCurrentCell(cur+1, 0);
}
void MainWindow::updateCpdList()
{
	lstCpds->clear();
	//lstCpds->addItems(mix->cpdIdList());
}

//
////////////////////////////////

// step editing implementations ////
//

void MainWindow::addStep()
{
	StepWindow* win = new StepWindow(new Step(), this, true);	
	QMdiSubWindow *mdiSubWin = ui.mdi->addSubWindow(win);
	mdiSubWin->setMinimumSize(win->minimumSize() + QSize(10,28) );
	mdiSubWin->setMaximumSize(win->maximumSize() + QSize(10,28) );
	mdiSubWin->show();
}
void MainWindow::removeStep()
{
	
}
void MainWindow::editStepWindow(QListWidgetItem *)
{
	
}
void MainWindow::moveStepUp()
{
	/*QListWidget *list = ui.lstSteps;
	int cur = list->currentRow();
	if( cur < 1 )
		return;
	mix->swapSteps(cur, cur-1);
	list->setCurrentRow(cur-1);*/
}
void MainWindow::moveStepDown()
{
	/*QListWidget *list = ui.lstSteps;
	int cur = list->currentRow();
	if( cur==-1 || cur==list->count()-1 )
		return;
	mix->swapSteps(cur, cur+1);
	list->setCurrentRow(cur+1);*/
}
void MainWindow::updateStepList()
{
	ui.lstSteps->clearContents();
	
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
