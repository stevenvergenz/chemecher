#include "mainwindow.h"

/** MainWindow::MainWindow
  * Initializes a few widgets, connects signals and slots.
  */
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	mdi = ui.mdi;
	
	// misc stuff
	setCentralWidget(ui.mdi);
	setWindowState( windowState() | Qt::WindowMaximized );
	
	// step editing signal stuff
	cpdMapper = new QSignalMapper(this);
	cpdMapper->setMapping( ui.pushAddCpd,   "AddCpd"   );
	cpdMapper->setMapping( ui.pushEditCpd,  "EditCpd"  );
	cpdMapper->setMapping( ui.lstCpds,      "EditCpd"  );
	connect( cpdMapper, SIGNAL(mapped(QString)), this, SLOT(showCpdWindow(QString)) );
	stepMapper = new QSignalMapper(this);
	stepMapper->setMapping( ui.pushAddStep,  "AddStep"  );
	stepMapper->setMapping( ui.pushEditStep, "EditStep" );
	stepMapper->setMapping( ui.lstSteps,     "EditStep" );
	connect( stepMapper, SIGNAL(mapped(QString)), this, SLOT(showStepWindow(QString)) );
	
	
	// cpd list drag&drop stuff
	//ui.lstCpds = ui.lstCpds;
	/*ui.lstCpds = new DragListWidget(ui.fraCpds);
	ui.lstCpds->setSizePolicy(ui.lstSteps->sizePolicy());
	ui.cpdLayout->addWidget(ui.lstCpds, 1,0,2,2, Qt::AlignLeft);*/
	//ui.lstCpds->show();
	
	///////////////////
	// SIGNALS/SLOTS //
	///////////////////
	
	// sim parameters
	
	
	// cpd
	connect(ui.pushAddCpd,    SIGNAL(clicked()), cpdMapper, SLOT(map()) );
	connect(ui.pushRemoveCpd, SIGNAL(clicked()), this, SLOT(removeCpd()));
	connect(ui.lstCpds, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), cpdMapper, SLOT(map()) );
	connect(ui.pushEditCpd, SIGNAL(clicked()), cpdMapper, SLOT(map()));
	connect(ui.pushMoveCpdUp,   SIGNAL(clicked()), this, SLOT(moveCpdUp()));
	connect(ui.pushMoveCpdDown, SIGNAL(clicked()), this, SLOT(moveCpdDown()));
	connect(mix, SIGNAL(cpdListChanged()), this, SLOT(updateCpdList()));
	connect(ui.lstCpds, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(setCpdInitConc(QTableWidgetItem*)) );
	
	// step
	connect(ui.pushAddStep,    SIGNAL(clicked()), stepMapper, SLOT(map()) );
	connect(ui.pushRemoveStep, SIGNAL(clicked()), this, SLOT(removeStep()));
	connect(ui.lstSteps, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), stepMapper, SLOT(map()) );
	connect(ui.pushEditStep, SIGNAL(clicked()), stepMapper, SLOT(map()) );
	connect(ui.pushMoveStepUp,   SIGNAL(clicked()), this, SLOT(moveStepUp()));
	connect(ui.pushMoveStepDown, SIGNAL(clicked()), this, SLOT(moveStepDown()));
	connect(mix, SIGNAL(stepListChanged()), this, SLOT(updateStepList()));
	
	// file menu
	connect(ui.actSaveToCM3,  SIGNAL(triggered()), this, SLOT(saveToCM3())     );
	connect(ui.actSaveMechDb, SIGNAL(triggered()), this, SLOT(saveMechDb()) );
	connect(ui.actLoadMechDb, SIGNAL(triggered()), this, SLOT(loadMechDb()) );
	connect(ui.actExit,       SIGNAL(triggered()), qApp, SLOT(quit()));
	
	// simulation menu
	connect(ui.actEditSimParams, SIGNAL(triggered()), this, SLOT(editSimParams()));
	
	// view menu
	connect( ui.actCascade,  SIGNAL(triggered()), ui.mdi, SLOT(cascadeSubWindows())  );
	connect( ui.actTile,     SIGNAL(triggered()), ui.mdi, SLOT(tileSubWindows())     );
	connect( ui.actCloseAll, SIGNAL(triggered()), ui.mdi, SLOT(closeAllSubWindows()) );
	
	// help menu
	connect(ui.actAbout,   SIGNAL(triggered()), this, SLOT(showAboutWindow()));
	connect(ui.actAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	
	/*Cpd* cpd_a = mix->addCpd(new Cpd("A", Cpd::HOMO ));
	cpd_a->setInitialConc(1.599);
	Cpd* cpd_b = mix->addCpd(new Cpd("B", Cpd::AQ   ));
	Cpd* cpd_c = mix->addCpd(new Cpd("C", Cpd::S    ));
	
	Step *step = new Step();
	step->setName("Decay");
	step->setKPlus(2.41);
	step->setKMinus(6.8);
	step->addReactant( cpd_a );
	//step->addReactant( cpd_b );
	//step->addProduct ( cpd_c );
	mix->addStep( step );
	
	step = new Step();
	step->setName("Growth");
	step->setKPlus(5.12);
	step->setKMinus(6.9);
	step->addReactant( cpd_c );
	step->addProduct ( cpd_b );
	//step->addProduct ( cpd_c );
	mix->addStep( step );*/
	
}


/** showCpdWindow
  * Displays a specie editing window. Uses a signal mapper
  * to determine appropriate action.
  * 
  * Called in three instances (args in parentheses):
  *   1. A compound from ui.lstCpds is double-clicked. ("EditCpd")
  *   2. The "Add Specie" button is clicked. ("AddCpd")
  *   3. The "Edit Specie" button is clicked. ("EditCpd")
  */
void MainWindow::showCpdWindow( QString action )
{
	Cpd *cpd;
	bool isnew = (action=="AddCpd");
	
	if( isnew )
		cpd = new Cpd();
	else {
		int row = ui.lstCpds->currentRow();
		if( row < 0 )
			return;
		else
			cpd = mix->getCpdById( ui.lstCpds->item(row,0)->text() );
	}
	
	// if subwindow exists, raise it and return
	QList<QMdiSubWindow*> windowlist = ui.mdi->subWindowList(QMdiArea::ActivationHistoryOrder);
	for(int i=0; i<windowlist.size(); i++) {
		if( windowtypes[windowlist[i]] == CPDWIN
				&& static_cast<CpdWindow*>(windowlist[i]->widget())->baseCpd == cpd ){
			windowlist[i]->showNormal();
			windowlist[i]->raise();
			windowlist[i]->setFocus();
			return;
		}
	}
		
	// create the subwindow, as new only if "Add Specie" button is down
	CpdWindow* win = new CpdWindow( cpd, this, isnew );
	QMdiSubWindow *mdiSubWin = ui.mdi->addSubWindow(win);
	mdiSubWin->setMinimumSize( win->minimumSize() + QSize(10,28) );
	mdiSubWin->setMaximumSize( win->maximumSize() + QSize(10,28) );
	mdiSubWin->show();
	
	// set the type of the window
	windowtypes[mdiSubWin] = CPDWIN;
	purgeWindowTypes();
}

/** removeCpd
  * Removes the currently selected compound from the list.
  */
void MainWindow::removeCpd()
{
	if( ui.lstCpds->currentRow()<0 )
		return;
	
	Cpd *cpd = mix->getCpdById( ui.lstCpds->item(ui.lstCpds->currentRow(),0)->text() );
	mix->removeCpd(cpd);
	delete cpd;
	updateCpdList();
	
	QList<QMdiSubWindow*> windowlist = ui.mdi->subWindowList(QMdiArea::ActivationHistoryOrder);
	for(int i=0; i<windowlist.size(); i++)
	{
		if( windowtypes[windowlist[i]] == CPDWIN
				&& windowlist[i]->windowTitle() == cpd->toString() ){
			ui.mdi->removeSubWindow(windowlist[i]);
			delete windowlist[i];
			return;
		}
	}
}

/** moveCpdUp
  * Moves the currently selected compound up on the list, if possible.
  */
void MainWindow::moveCpdUp()
{
	QTableWidget *list = ui.lstCpds;
	int cur = list->currentRow();
	if( cur < 1 )
		return;
	mix->swapCpds(cur, cur-1);
	list->setCurrentCell(cur-1, 0);
}

/** moveCpdDown()
  * Moves the currently selected compound down on the list, if possible.
  */
void MainWindow::moveCpdDown()
{
	QTableWidget *list = ui.lstCpds;
	int cur = list->currentRow();
	if( cur==-1 || cur==list->rowCount()-1 )
		return;
	mix->swapCpds(cur, cur+1);
	list->setCurrentCell(cur+1, 0);
}

/** updateCpdList
  * Refreshes the current list of compounds to match mix->CpdList.
  */
void MainWindow::updateCpdList()
{
	ui.lstCpds->clearContents();
	ui.lstCpds->setRowCount(mix->CpdList.size());
	for( int i=0; i<mix->CpdList.size(); i++ ) {
		QTableWidgetItem *id = new QTableWidgetItem( mix->CpdList[i]->toString() );
		id->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
		ui.lstCpds->setItem( i,0, id );
		QTableWidgetItem *conc = new QTableWidgetItem( QString("%1").arg(mix->CpdList[i]->initialConc()) );
		conc->setFlags( Qt::ItemIsEnabled | Qt::ItemIsEditable );\
		conc->setTextAlignment( Qt::AlignRight );
		ui.lstCpds->setItem( i,1, conc );
	}
}

/** setCpdInitConc
  * Sets the initial concentration of the compound when its table cell is edited.
  */
void MainWindow::setCpdInitConc( QTableWidgetItem* item )
{
	if( item->column()!=1 )
		return;
	
	// set variables (to make code more readable ;) )
	double dispconc = item->text().toDouble();
	Cpd *cpd = mix->getCpdById(	item->tableWidget()->item(item->row(), 0)->text() );
	
	if( dispconc != cpd->initialConc() )
		cpd->setInitialConc( dispconc );
}



void MainWindow::showStepWindow( QString action )
{
	Step *step;
	bool isnew = (action=="AddStep");
	
	if( isnew )
		step = new Step();
	else {
		int row = ui.lstSteps->currentRow();
		if( row < 0 )
			return;
		else
			step = mix->getStepByName( ui.lstSteps->item(row,0)->text() );
	}
	
	// if subwindow exists, raise it and return
	QList<QMdiSubWindow*> windowlist = ui.mdi->subWindowList(QMdiArea::ActivationHistoryOrder);
	for(int i=0; i<windowlist.size(); i++) {
		if( windowtypes[windowlist[i]] == STEPWIN
				&& static_cast<StepWindow*>(windowlist[i]->widget())->baseStep == step ){
			windowlist[i]->showNormal();
			windowlist[i]->raise();
			windowlist[i]->setFocus();
			return;
		}
	}
	
	ui.lstSteps->setCurrentCell(-1,-1);
	
	// create the subwindow
	StepWindow* win = new StepWindow( step, 0, isnew );
	cpdMapper->setMapping( win, "AddCpd" );
	connect( win, SIGNAL(addCpdClicked()), cpdMapper, SLOT(map()) );
	QMdiSubWindow *mdiSubWin = ui.mdi->addSubWindow(win);
	mdiSubWin->setMinimumSize( win->minimumSize() + QSize(10,28) );
	mdiSubWin->setMaximumSize( win->maximumSize() + QSize(10,28) );
	mdiSubWin->show();
	windowtypes[mdiSubWin] = STEPWIN;
	purgeWindowTypes();
}
void MainWindow::removeStep()
{
	if( ui.lstSteps->currentRow()<0 )
		return;
	
	QString name = ui.lstSteps->item(ui.lstSteps->currentRow(),0)->text();
	
	Step *step = mix->getStepByName( name );
	mix->removeStep(step);
	delete step;
	updateStepList();
	
	QList<QMdiSubWindow*> windowlist = ui.mdi->subWindowList(QMdiArea::ActivationHistoryOrder);
	for(int i=0; i<windowlist.size(); i++)
	{
		if( windowtypes[windowlist[i]] == STEPWIN
				&& windowlist[i]->windowTitle() == name ){
			ui.mdi->removeSubWindow(windowlist[i]);
			delete windowlist[i];
			return;
		}
	}
}
void MainWindow::moveStepUp()
{
	QTableWidget *list = ui.lstSteps;
	int cur = list->currentRow();
	if( cur < 1 )
		return;
	mix->swapSteps(cur, cur-1);
	list->setCurrentCell( cur-1, 0 );
}
void MainWindow::moveStepDown()
{
	QTableWidget *list = ui.lstSteps;
	int cur = list->currentRow();
	if( cur==-1 || cur==list->rowCount()-1 )
		return;
	mix->swapSteps(cur, cur+1);
	list->setCurrentCell( cur+1, 0 );
}
void MainWindow::updateStepList()
{
	ui.lstSteps->clearContents();
	ui.lstSteps->setRowCount(mix->StepList.size());
	for( int i=0; i<mix->StepList.size(); i++ ) {
		QTableWidgetItem *name = new QTableWidgetItem( mix->StepList[i]->name() );
		name->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
		ui.lstSteps->setItem( i,0, name );
		QTableWidgetItem *string = new QTableWidgetItem( mix->StepList[i]->toString() );
		string->setFlags( name->flags() );
		string->setTextAlignment( Qt::AlignRight );
		ui.lstSteps->setItem( i,1, string );
		if( !mix->StepList[i]->isValid() ) {
			name->setTextColor( QColor(Qt::red) );
			string->setTextColor( QColor(Qt::red) );
		}
	}
}

void MainWindow::updateEditButtonEnabled()
{
	//ui.pushEditCpd ->setEnabled( ui.lstCpds ->currentRow()<0 );
	//ui.pushEditStep->setEnabled( ui.lstSteps->currentRow()<0 );
}

void MainWindow::purgeWindowTypes()
{
	for( int i=0; i<windowtypes.keys().size(); i++ )
		if( !ui.mdi->subWindowList().contains(windowtypes.keys()[i]) )
			windowtypes.remove(windowtypes.keys()[i]);	
}

//
////////////////////////////////

void MainWindow::editSimParams()
{
	SimParams *simparams = new SimParams();
	simparams->show();
}

/** contextMenuEvent
  * Brings up a popup menu when step or specie lists are clicked */
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
	QPoint pos = event->globalPos();
	QTableWidget *list = static_cast<QTableWidget*>(qApp->widgetAt(pos));
	if( list!=ui.lstCpds && list!=ui.lstSteps )
		return;
	
	qDebug() << ":D" << list->rowCount();
	
	/*&& ui.lstSteps->itemAt(pos-ui.lstSteps->pos())*/
}

// saving/loading ////
//

void MainWindow::saveToCM3()
{
	// warn the user about CM3 format
	QMessageBox::StandardButton ret;
	ret = QMessageBox::warning ( this, "CheMecher",
			"Warning:\n"
			"Information on step names, descriptions, specie long names, "
			"etc. will be not be saved in CheMecher3 format.",
			QMessageBox::Ok | QMessageBox::Cancel ) ;
	if( ret == QMessageBox::Cancel )
		return;
	
	QFileDialog save(this);
	save.setAcceptMode(QFileDialog::AcceptSave);
	QString mech, sim;
	
	// get the mechanism filename
	save.setDefaultSuffix("cm3m");
	save.setFilter("CheMecher3 mechanism files (*.cm3m);;Text files (*.txt);;All files (*.*)");
	save.setWindowTitle("Mechanism File");
	save.setDirectory(QDir::current().path()+"/../input");
	if( !save.exec() )
		return;
	mech = save.selectedFiles()[0];
	
	// construct the default simulation filename
	QFileInfo fi = QFileInfo(mech);
	sim = fi.absolutePath()+"/"+fi.completeBaseName();
	if( fi.suffix()=="cm3m" )
		sim += ".cm3s";
	else
		sim += ".sim." + fi.suffix();
	save.selectFile(sim);
	
	// get the simulation filename
	save.setDefaultSuffix( fi.suffix()=="cm3m"?"cm3s":fi.suffix() );
	save.setFilter("CheMecher3 solution files (*.cm3s);;Text files (*.txt);;All files (*.*)");
	save.setWindowTitle("Simulation File (Mechanism: " + QFileInfo(mech).fileName() + ")");
	save.setDirectory(fi.absolutePath());
	if( !save.exec() )
		return;
	sim = save.selectedFiles()[0];
	if( sim =="" ) return;
	
	// check for uniqueness
	if( mech==sim ) {
		QMessageBox::warning ( this, "CheMecher",
				"Mechanism and solution files must be different!",
				QMessageBox::Ok ) ;
		return;
	}
	
	// save the file
	iomgr->saveToCM3( mech, sim );
}

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

void MainWindow::showAboutWindow()
{
	About *about = new About();
	about->exec();
}
