#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	mdi = ui.mdi;

	// misc stuff
	setCentralWidget(ui.mdi);
	setWindowState( windowState() | Qt::WindowMaximized );
	
	// cpd list drag&drop stuff
	lstCpds = new DragListWidget(ui.fraCpds);
	ui.cpdLayout->addWidget(lstCpds, 1,0,2,2, Qt::AlignLeft);
	//lstCpds->show();
	
	///////////////////
	// SIGNALS/SLOTS //
	///////////////////
	
	// cpd
	connect(ui.pushAddCpd,    SIGNAL(clicked()), this, SLOT(addCpd()));
	connect(ui.pushRemoveCpd, SIGNAL(clicked()), this, SLOT(removeCpd()));
	connect(mix, SIGNAL(addedCpd(Cpd*)), this, SLOT(updateCpdList()));
	connect(lstCpds, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
			this, SLOT(editCpdWindow(QListWidgetItem*)));
	connect(ui.pushMoveCpdUp,   SIGNAL(clicked()), this, SLOT(moveCpdUp()));
	connect(ui.pushMoveCpdDown, SIGNAL(clicked()), this, SLOT(moveCpdDown()));
	
	// step
	connect(ui.pushAddStep,    SIGNAL(clicked()), this, SLOT(addStep()));
	connect(ui.pushRemoveStep, SIGNAL(clicked()), this, SLOT(removeStep()));
	connect(mix, SIGNAL(addedStep(Step*)), this, SLOT(updateStepList()));
	connect(ui.lstSteps, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
			this, SLOT(editStepWindow(QListWidgetItem*)));
	connect(ui.pushMoveStepUp,   SIGNAL(clicked()), this, SLOT(moveStepUp()));
	connect(ui.pushMoveStepDown, SIGNAL(clicked()), this, SLOT(moveStepDown()));
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

// compound editing implementations ////
//

void MainWindow::addCpd()
{
	CpdWindow* win = new CpdWindow(new Cpd(), this, 0, true);
	ui.mdi->addSubWindow(win);
	win->resize(261, 337);
	win->show();
}
void MainWindow::removeCpd()
{
	if( lstCpds->currentRow()<0 )
		return;
	
	Cpd *cpd = mix->getCpdById(lstCpds->item(lstCpds->currentRow())->text());
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
void MainWindow::editCpdWindow(QListWidgetItem* item)
{
	//get the cpd
	Cpd* cpd = mix->CpdList.at(
			item->listWidget()->row(item) );
	
	qDebug() << item->listWidget()->row(item);
	qDebug() << cpd->toString();
	
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
	ui.mdi->addSubWindow( win );
	//QList<QMdiSubWindow*> windowlist = mdi->subWindowList(QMdiArea::ActivationHistoryOrder);
	win->resize(261, 337);
	win->showNormal();
	win->raise();
	win->setFocus();
}
void MainWindow::moveCpdUp()
{
	QListWidget *list = lstCpds;
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
	QListWidget *list = lstCpds;
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
	lstCpds->clear();
	QListIterator<QString> i(mix->cpdIdList());
	for(;i.hasNext();i.next())
		lstCpds->addItem(i.peekNext());
}

//
////////////////////////////////

// step editing implementations ////
//

void MainWindow::addStep()
{
	qDebug() << ":D (1)";
	StepWindow* win = new StepWindow(new Step(), this, true);
	QMdiSubWindow *subwin = new QMdiSubWindow;
	subwin->setWidget(win);
	subwin->setAttribute(Qt::WA_DeleteOnClose);
	ui.mdi->addSubWindow(subwin);
	subwin->show();
	subwin->setGeometry(0,0,490,339);
	subwin->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	subwin->setMinimumSize(490,339);
	subwin->setMaximumSize(490,339);
	//subwin->setAttribute(Qt::WA_DeleteOnClose);
	//subwin->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	//win->show();
	
}
void MainWindow::removeStep()
{
	
}
void MainWindow::editStepWindow(QListWidgetItem *)
{
	
}
void MainWindow::moveStepUp()
{
	
}
void MainWindow::moveStepDown()
{
	
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
