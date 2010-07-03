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
	
	// cpd list drag&drop stuff
	lstCpds = ui.lstCpds;
	/*lstCpds = new DragListWidget(ui.fraCpds);
	lstCpds->setSizePolicy(ui.lstSteps->sizePolicy());
	ui.cpdLayout->addWidget(lstCpds, 1,0,2,2, Qt::AlignLeft);*/
	//lstCpds->show();
	
	///////////////////
	// SIGNALS/SLOTS //
	///////////////////
	
	// cpd
	connect(ui.pushAddCpd,    SIGNAL(pressed()), this, SLOT(showCpdWindow()));
	connect(ui.pushRemoveCpd, SIGNAL(clicked()), this, SLOT(removeCpd()));
	connect(lstCpds, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(showCpdWindow(QTableWidgetItem*)));
	connect(ui.pushEditCpd, SIGNAL(pressed()), this, SLOT(showCpdWindow()));
	connect(ui.pushMoveCpdUp,   SIGNAL(clicked()), this, SLOT(moveCpdUp()));
	connect(ui.pushMoveCpdDown, SIGNAL(clicked()), this, SLOT(moveCpdDown()));
	connect(mix, SIGNAL(cpdListChanged()), this, SLOT(updateCpdList()));
	connect(lstCpds, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(setCpdInitConc(QTableWidgetItem*)) );
	
	// step
	connect(ui.pushAddStep,    SIGNAL(clicked()), this, SLOT(showStepWindow()));
	connect(ui.pushRemoveStep, SIGNAL(clicked()), this, SLOT(removeStep()));
	connect(ui.lstSteps, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(showStepWindow(QTableWidgetItem*)));
	connect(ui.pushEditStep, SIGNAL(pressed()), this, SLOT(showStepWindow()));
	connect(ui.pushMoveStepUp,   SIGNAL(clicked()), this, SLOT(moveStepUp()));
	connect(ui.pushMoveStepDown, SIGNAL(clicked()), this, SLOT(moveStepDown()));
	connect(mix, SIGNAL(stepListChanged()), this, SLOT(updateStepList()));
	
	// mdi area
	connect( ui.actCascade,  SIGNAL(triggered()), ui.mdi, SLOT(cascadeSubWindows())  );
	connect( ui.actTile,     SIGNAL(triggered()), ui.mdi, SLOT(tileSubWindows())     );
	connect( ui.actCloseAll, SIGNAL(triggered()), ui.mdi, SLOT(closeAllSubWindows()) );
	
	// saving/loading
	connect(ui.actSaveAs,     SIGNAL(triggered()), this, SLOT(saveAs())     );
	connect(ui.actSaveMechDb, SIGNAL(triggered()), this, SLOT(saveMechDb()) );
	connect(ui.actLoadMechDb, SIGNAL(triggered()), this, SLOT(loadMechDb()) );
	
	// exit
	connect(ui.actExit, SIGNAL(triggered()), qApp, SLOT(quit()));
	//connect(qApp, SIGNAL(aboutToQuit()), mix->db, SLOT(closeDb()));
	
	Cpd* cpd_a = mix->addCpd(new Cpd("A", Cpd::HOMO ));
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
	mix->addStep( step );
	
}


/** showCpdWindow
  * Displays a specie editing window.
  * Called in three instances:
  *   1. A compound from lstCpds is double-clicked.
  *   2. The "Add Specie" button is clicked.
  *   3. The "Edit Specie" button is clicked.
  */
void MainWindow::showCpdWindow( QTableWidgetItem* item )
{
	Cpd *cpd;
	if( item==0 )
		item = lstCpds->item( lstCpds->currentRow(), 0 );
	else if( item->column()==1 )
		return;
	
	// if function was called from Add button
	if( ui.pushAddCpd->isDown() ) {
		// make it a new compound
		cpd = new Cpd();
	}
	// if function was called from either list double-clicked or "Edit Specie" button
	else {
		cpd = mix->CpdList.at( item->tableWidget()->row(item) );
		
		// if subwindow exists, raise it and return
		QList<QMdiSubWindow*> windowlist = ui.mdi->subWindowList(QMdiArea::ActivationHistoryOrder);
		for(int i=0; i<windowlist.size(); i++) {
			if( windowtypes[windowlist[i]] == CPDWIN
					&& windowlist[i]->windowTitle() == cpd->toString() ){
				windowlist[i]->showNormal();
				windowlist[i]->raise();
				windowlist[i]->setFocus();
				return;
			}
		}
	}
	
	// create the subwindow, as new only if "Add Specie" button is down
	CpdWindow* win = new CpdWindow( cpd, this, (ui.pushAddCpd->isDown()) );
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
	if( lstCpds->currentRow()<0 )
		return;
	
	Cpd *cpd = mix->getCpdById( lstCpds->item(lstCpds->currentRow(),0)->text() );
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
	QTableWidget *list = lstCpds;
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
	QTableWidget *list = lstCpds;
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
	lstCpds->clearContents();
	lstCpds->setRowCount(mix->CpdList.size());
	for( int i=0; i<mix->CpdList.size(); i++ ) {
		QTableWidgetItem *id = new QTableWidgetItem( mix->CpdList[i]->toString() );
		id->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
		lstCpds->setItem( i,0, id );
		QTableWidgetItem *conc = new QTableWidgetItem( QString("%1").arg(mix->CpdList[i]->initialConc()) );
		conc->setFlags( Qt::ItemIsEnabled | Qt::ItemIsEditable );\
		conc->setTextAlignment( Qt::AlignRight );
		lstCpds->setItem( i,1, conc );
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



void MainWindow::showStepWindow( QTableWidgetItem *item )
{
	Step *step;
	
	// if function was called from Add button
	if( item == 0 ) {
		// make it a new step
		step = new Step();
	}
	
	// if function was called from list double-clicked
	else {
		step = mix->StepList.at( item->tableWidget()->row(item) );
		
		// if subwindow exists, raise it and return
		QList<QMdiSubWindow*> windowlist = ui.mdi->subWindowList(QMdiArea::ActivationHistoryOrder);
		for(int i=0; i<windowlist.size(); i++) {
			if( windowtypes[windowlist[i]] == STEPWIN
					&& windowlist[i]->windowTitle() == step->name() && windowlist[i]->widget() ){
				windowlist[i]->showNormal();
				windowlist[i]->raise();
				windowlist[i]->setFocus();
				
				ui.lstSteps->setCurrentCell(-1,-1);
				
				return;
			}
		}
		
	}
	
	ui.lstSteps->setCurrentCell(-1,-1);
	
	// create the subwindow
	StepWindow* win = new StepWindow( step, 0, (item==0) );
	connect( win, SIGNAL(addCpdClicked()), this, SLOT(showCpdWindow()) );
	QMdiSubWindow *mdiSubWin = ui.mdi->addSubWindow(win);
	mdiSubWin->setMinimumSize( win->minimumSize() + QSize(10,28) );
	mdiSubWin->setMaximumSize( win->maximumSize() + QSize(10,28) );
	mdiSubWin->show();
	windowtypes[mdiSubWin] = STEPWIN;
	purgeWindowTypes();
	
	qDebug() << step->tov3String();
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
	ui.pushEditCpd ->setEnabled( ui.lstCpds ->currentRow()<0 );
	ui.pushEditStep->setEnabled( ui.lstSteps->currentRow()<0 );
}

void MainWindow::purgeWindowTypes()
{
	for( int i=0; i<windowtypes.keys().size(); i++ )
		if( !ui.mdi->subWindowList().contains(windowtypes.keys()[i]) )
			windowtypes.remove(windowtypes.keys()[i]);	
}

//
////////////////////////////////

// saving/loading ////
//

void MainWindow::saveAs()
{
	QString filename = QFileDialog::getSaveFileName(
			this, "Save Mech", QDir::current().path(), "");
	iomgr->saveToCM3( filename + ".cm3s", filename + ".cm3m" );
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
