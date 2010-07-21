#include "mainwindow.h"

/** MainWindow::MainWindow
  * Initializes a few widgets, connects signals and slots.
  */
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	mdi = ui.mdi;
	
	QCoreApplication::setOrganizationName("UNF Chemistry Department");
	QCoreApplication::setApplicationName("CheMecher4");
	
	// misc stuff
	setCentralWidget(ui.mdi);
	setWindowState( windowState() | Qt::WindowMaximized );
	
	// step editing signal stuff
	cpdMapper = new QSignalMapper(this);
	cpdMapper->setMapping( ui.actAddCpd,   "AddCpd"   );
	cpdMapper->setMapping( ui.actEditCpd,  "EditCpd"  );
	connect( cpdMapper, SIGNAL(mapped(QString)), this, SLOT(showCpdWindow(QString)) );
	stepMapper = new QSignalMapper(this);
	stepMapper->setMapping( ui.actAddStep,  "AddStep"  );
	stepMapper->setMapping( ui.actEditStep, "EditStep" );
	connect( stepMapper, SIGNAL(mapped(QString)), this, SLOT(showStepWindow(QString)) );
	
	///////////////////
	// SIGNALS/SLOTS //
	///////////////////
	
	// context menu
	connect( ui.lstCpds,  SIGNAL(customContextMenuRequested(QPoint)),
			 this,        SLOT(cpdContextMenu(QPoint)) );
	connect( ui.lstSteps, SIGNAL(customContextMenuRequested(QPoint)),
			 this,        SLOT(stepContextMenu(QPoint)) );
	
	// cpd
	connect(ui.pushAddCpd,    SIGNAL(clicked()), ui.actAddCpd,    SLOT(trigger()) );
	connect(ui.pushEditCpd,   SIGNAL(clicked()), ui.actEditCpd,   SLOT(trigger()) );
	connect(ui.pushDeleteCpd, SIGNAL(clicked()), ui.actDeleteCpd, SLOT(trigger()) );
	//connect(ui.lstCpds, SIGNAL(cellDoubleClicked(int,int)), ui.actEditCpd, SLOT(trigger()) );
	connect(ui.lstCpds, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(cpdListDoubleClicked(int,int)) );
	connect(ui.pushMoveCpdUp,   SIGNAL(clicked()), ui.actMvCpdUp,   SLOT(trigger()) );
	connect(ui.pushMoveCpdDown, SIGNAL(clicked()), ui.actMvCpdDown, SLOT(trigger()) );
	connect(mix, SIGNAL(cpdListChanged()), this, SLOT(updateCpdList()));
	connect(ui.lstCpds, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(setCpdInitConc(QTableWidgetItem*)) );
	
	// step
	connect(ui.pushAddStep,    SIGNAL(clicked()), ui.actAddStep,    SLOT(trigger()) );
	connect(ui.pushEditStep,   SIGNAL(clicked()), ui.actEditStep,   SLOT(trigger()) );
	connect(ui.pushDeleteStep, SIGNAL(clicked()), ui.actDeleteStep, SLOT(trigger()) );
	connect(ui.lstSteps, SIGNAL(cellDoubleClicked(int,int)), ui.actEditStep, SLOT(trigger()) );
	connect(ui.pushMoveStepUp,   SIGNAL(clicked()), ui.actMvStepUp, SLOT(trigger()) );
	connect(ui.pushMoveStepDown, SIGNAL(clicked()), ui.actMvStepDown, SLOT(trigger()));
	connect(mix, SIGNAL(stepListChanged()), this, SLOT(updateStepList()));
	
	// file menu
	connect(ui.actNew,         SIGNAL(triggered()), this, SLOT(newMech())     );
	connect(ui.actSaveAs,      SIGNAL(triggered()), this, SLOT(saveToCM4())   );
	connect(ui.actLoad,        SIGNAL(triggered()), this, SLOT(loadFromCM4()) );
	connect(ui.actSaveToCM3,   SIGNAL(triggered()), this, SLOT(saveToCM3())   );
	connect(ui.actLoadFromCM3, SIGNAL(triggered()), this, SLOT(loadFromCM3()) );
	connect(ui.actSaveMechDb,  SIGNAL(triggered()), this, SLOT(saveMechDb())  );
	connect(ui.actLoadMechDb,  SIGNAL(triggered()), this, SLOT(loadMechDb())  );
	connect(ui.actExit,        SIGNAL(triggered()), qApp, SLOT(quit())        );
	
	// mechanism menu
	connect( ui.actAddCpd,         SIGNAL(triggered()), cpdMapper,  SLOT(map())            );
	connect( ui.actEditCpd,        SIGNAL(triggered()), cpdMapper,  SLOT(map())            );
	connect( ui.actMvCpdUp,        SIGNAL(triggered()), this,       SLOT(moveCpdUp())      );
	connect( ui.actMvCpdDown,      SIGNAL(triggered()), this,       SLOT(moveCpdDown())    );
	connect( ui.actDeleteCpd,      SIGNAL(triggered()), this,       SLOT(deleteCpd())      );
	connect( ui.actDeleteAllCpds,  SIGNAL(triggered()), this,       SLOT(deleteAllCpds())  );
	connect( ui.actAddStep,        SIGNAL(triggered()), stepMapper, SLOT(map())            );
	connect( ui.actEditStep,       SIGNAL(triggered()), stepMapper, SLOT(map())            );
	connect( ui.actMvStepUp,       SIGNAL(triggered()), this,       SLOT(moveStepUp())     );
	connect( ui.actMvStepDown,     SIGNAL(triggered()), this,       SLOT(moveStepDown())   );
	connect( ui.actDeleteStep,     SIGNAL(triggered()), this,       SLOT(deleteStep())     );
	connect( ui.actDeleteAllSteps, SIGNAL(triggered()), this,       SLOT(deleteAllSteps()) );
	connect( ui.actEditSimParams,  SIGNAL(triggered()), this,       SLOT(editSimParams())  );
	// set current mech to not new
	newMech( false );
	
	// tools menu
	connect( ui.actPrefs, SIGNAL(triggered()), this, SLOT(editPrefs()) );
	
	// view menu
	connect( ui.actCascade,  SIGNAL(triggered()), ui.mdi, SLOT(cascadeSubWindows())  );
	connect( ui.actTile,     SIGNAL(triggered()), ui.mdi, SLOT(tileSubWindows())     );
	connect( ui.actCloseAll, SIGNAL(triggered()), ui.mdi, SLOT(closeAllSubWindows()) );
	
	// help menu
	connect(ui.actReportBug,      SIGNAL(triggered()), this, SLOT(reportBug())       );
	connect(ui.actSuggestFeature, SIGNAL(triggered()), this, SLOT(suggestFeature())  );
	connect(ui.actAbout,          SIGNAL(triggered()), this, SLOT(showAboutWindow()) );
	connect(ui.actAboutQt,        SIGNAL(triggered()), qApp, SLOT(aboutQt())         );
	
	initTestMech();
	
}

/** DEBUG MODE ONLY -- DISABLE FOR ACTUAL BUILD **/
void MainWindow::initTestMech()
{
	return;
	
	/*
	// uncomment next line to disable
	iomgr->loadFromCM3(
			"/home/vergenz/programming/chemecher/input/TestMech.txt",
			"/home/vergenz/programming/chemecher/input/TestSim.txt");
	
	return;
	
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
	mix->addStep( step );*/
}

/** showCpdWindow
  * Displays a specie editing window. Uses a signal mapper
  * to determine appropriate action.
  * 
  * Called in three instances (args in parentheses):
  *   1. A compound from ui.lstCpds is double-clicked. ("EditCpdDClicked")
  *   2. The "Add Specie" button is clicked. ("AddCpd")
  *   3. The "Edit Specie" button is clicked. ("EditCpd")
  */
void MainWindow::showCpdWindow( QString action )
{
	Cpd *cpd;
	bool isnew = (action=="AddCpd");
	
	if( isnew ) {
		
		/*
		// count the number of unsaved "New Species" subwindows that already exist
		int count = 0;
		QMdiSubWindow* win = 0;
		QList<QMdiSubWindow*> list = ui.mdi->subWindowList();
		for( int i=0; i<list.count(); i++ )
			if( windowtypes[list[i]]==CPDWIN
				&& !mix->CpdList.contains( static_cast<CpdWindow*>(list[i]->widget())->base() ) ) {
				count++;
				win = list[i];
			}
		
		// if 5 or more "New Species" windows exist, use the most recent
		if( count>=5 ) {
			win->showNormal();
			win->raise();
			win->setFocus();
			return;
		}
		*/
		
		cpd = new Cpd();
	}
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

/** cpdListDoubleClicked
  * Called when user double-clicks the cpd list
  * Ensures that CpdWindow does not come up when editing concentration
  */
void MainWindow::cpdListDoubleClicked(int r, int c)
{
	if( c==0 && r>-1 )
		showCpdWindow( "EditCpd" );
}

/** removeCpd
  * Removes the currently selected compound from the list.
  */
void MainWindow::deleteCpd( int i )
{
	// use argument if it's greater than 0
	// otherwise use current row of lstSteps
	i = (i<0 ? ui.lstCpds->currentRow() : i);
	if( i<0 )
		return;
	
	Cpd *cpd = mix->getCpdById( ui.lstCpds->item(i,0)->text() );
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
void MainWindow::deleteAllCpds()
{
	QMessageBox::StandardButton ret;
	ret = QMessageBox::warning ( this, "CheMecher",
			"Are you sure you want to delete all species from the current mechanism?",
			QMessageBox::Yes | QMessageBox::Cancel ) ;
	if( ret != QMessageBox::Yes )
		return;
	
	while( ui.lstCpds->rowCount()>0 )
		deleteCpd(0);
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
		QTableWidgetItem *conc = new QTableWidgetItem( QString::number(mix->CpdList[i]->initialConc()) );
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
	Cpd *cpd = mix->getCpdById(	item->tableWidget()->item(item->row(), 0)->text() );
	bool ok = true;
	double dispconc = item->text().toDouble(&ok);
	
	// if not valid
	if( !ok || dispconc<0 ) {
		item->setText( QString::number(cpd->initialConc()) );
		return;
	}
	
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
void MainWindow::deleteStep( int i )
{
	// use argument if it's greater than 0
	// otherwise use current row of lstSteps
	i = (i<0 ? ui.lstSteps->currentRow() : i);
	if( i<0 )
		return;
	
	QString name = ui.lstSteps->item(i,0)->text();
	
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
void MainWindow::deleteAllSteps()
{
	QMessageBox::StandardButton ret;
	ret = QMessageBox::warning ( this, "CheMecher",
			"Are you sure you want to delete all steps from the current mechanism?",
			QMessageBox::Yes | QMessageBox::Cancel ) ;
	if( ret != QMessageBox::Yes )
		return;
	
	while( ui.lstSteps->rowCount()>0 )
		deleteStep(0);
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
		string->setFlags( Qt::ItemIsEnabled );
		string->setTextAlignment( Qt::AlignRight );
		ui.lstSteps->setItem( i,1, string );
		if( !mix->StepList[i]->isValid() ) {
			//name->setTextColor( QColor(Qt::red) );
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
	SimParams simparams(this);
	simparams.exec();
}

void MainWindow::cpdContextMenu( QPoint pos )
{
	QMenu menu(this);
	menu.addAction(ui.actAddCpd);
	menu.addAction(ui.actEditCpd);
	menu.addAction(ui.actDeleteCpd);
	menu.addAction(ui.actDeleteAllCpds);
	menu.exec(ui.lstCpds->mapToGlobal(pos));
}
void MainWindow::stepContextMenu( QPoint pos )
{
	QMenu menu(this);
	menu.addAction(ui.actAddStep);
	menu.addAction(ui.actEditStep);
	menu.addAction(ui.actDeleteStep);
	menu.addAction(ui.actDeleteAllSteps);
	menu.exec(ui.lstSteps->mapToGlobal(pos));
}

// file menu ////
//

void MainWindow::newMech( bool val )
{
	if( mix->isActive && val ) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning( this, "CheMecher",
				"Are you sure you want to close the current mechanism and create a new one?",
				QMessageBox::Yes | QMessageBox::Cancel );
		if( ret == QMessageBox::Cancel )
			return;
	}
	
	if( val ) {
		Mix *newmix = new Mix();
		mix->clone( newmix );
		delete newmix;
	}
	mix->isActive = val;
	foreach( QAction *act, ui.menu_Mechanism->actions() )
		act->setEnabled(val);
	foreach( QAction *act, ui.menu_Steps->actions() )
		act->setEnabled(val);
	foreach( QAction *act, ui.menu_Species->actions() )
		act->setEnabled(val);
	ui.dockWidget->setVisible(val);
	ui.actSave->setEnabled(val);
	ui.actSaveAs->setEnabled(val);
	ui.actSaveToCM3->setEnabled(val);
	ui.actSaveMechDb->setEnabled(val);
}

void MainWindow::saveToCM4()
{
	QString filename = "";
	QFileDialog save(this);
	save.setAcceptMode(QFileDialog::AcceptSave);
	save.setDefaultSuffix("cm4");
	save.setFilter("CheMecher4 files (*.cm4);;All files (*.*)");
	save.setWindowTitle("CheMecher");
	save.setDirectory(QDir::current().path()+"/../input");
	if( !save.exec() )
		return;
	filename = save.selectedFiles()[0];
	statusBar()->showMessage("Saving to " + QFileInfo(filename).fileName() + "...", 1000);
	if( filename!="" )
		iomgr->saveToCM4(filename);
	//statusBar()->clearMessage();
}

void MainWindow::loadFromCM4()
{
	QFileDialog load(this);
	load.setAcceptMode(QFileDialog::AcceptOpen);
	load.setFileMode( QFileDialog::ExistingFile );
	QString mech;
	
	// get the mechanism filename
	load.setFilter("CheMecher4 files (*.cm4);;All files (*.*)");
	load.setWindowTitle("Mechanism File");
	load.setDirectory(QDir::current().path()+"/../input");
	if( !load.exec() )
		return;
	mech = load.selectedFiles()[0];
	if( mech=="" )
		return;
	
	newMech();
	
	// load the file
	if( !iomgr->loadFromCM4( mech ) ){
		QMessageBox::critical(this, "Chemecher 4", "Failed to load file: "+iomgr->getMessage());
	}
}

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
	save.setFilter("CheMecher3 mechanism files (*.cm3m *.txt);;All files (*.*)");
	save.setWindowTitle("Mechanism File");
	QString cp = QDir::current().path();
	save.setDirectory( QFileInfo(cp+"/../input").exists() ? cp+"/../input" : cp );
	save.setLabelText( QFileDialog::FileName, "Mechanism file &name:" );
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
	save.setFilter("CheMecher3 simulation files (*.cm3s *.txt);;All files (*.*)");
	save.setWindowTitle("Simulation File (Mechanism: " + QFileInfo(mech).fileName() + ")");
	save.setDirectory(fi.absolutePath());
	save.setLabelText( QFileDialog::FileName, "Simulation file &name:" );
	if( !save.exec() )
		return;
	sim = save.selectedFiles()[0];
	if( sim =="" ) return;
	
	// check for uniqueness
	if( mech==sim ) {
		QMessageBox::warning ( this, "CheMecher",
				"Mechanism and simulation files must be different!",
				QMessageBox::Ok ) ;
		return;
	}
	
	// save the file
	if( !iomgr->saveToCM3( mech, sim ) )
		QMessageBox::warning ( this, "CheMecher",
				iomgr->getMessage(),
				QMessageBox::Ok ) ;
}

void MainWindow::loadFromCM3()
{
	QFileDialog load(this);
	load.setAcceptMode(QFileDialog::AcceptOpen);
	load.setFileMode( QFileDialog::ExistingFile );
	QString mech, sim;
	
	// get the mechanism filename
	load.setFilter("CheMecher3 mechanism files (*.cm3m *.txt);;All files (*.*)");
	load.setWindowTitle("Mechanism File");
	load.setDirectory(QDir::current().path()+"/../input");
	if( !load.exec() )
		return;
	mech = load.selectedFiles()[0];
	if( mech=="" )
		return;
	
	// get the simulation filename
	load.setFilter("CheMecher3 simulation files (*.cm3s *.txt);;All files (*.*)");
	load.setWindowTitle("Simulation File (Mechanism: " + QFileInfo(mech).fileName() + ")");
	load.setDirectory(QFileInfo(mech).absolutePath());
	if( !load.exec() )
		return;
	sim = load.selectedFiles()[0];
	if( sim=="" ) return;
	
	// check for uniqueness
	if( mech==sim ) {
		QMessageBox::warning ( this, "CheMecher",
				"Mechanism and simulation files must be different!") ;
		return;
	}
	
	newMech();
	
	// load the file
	if( !iomgr->loadFromCM3( mech, sim ) ) {
		newMech(false);
		QString title = "CheMecher";
		switch( iomgr->getStatus() ) {
		case IOManager::FS_ERROR:
			title = "Filesystem Error";
			break;
		case IOManager::PARSE_ERROR:
			title = "Parse Error";
			break;
		case IOManager::ERROR:
			title = "Error";
			break;
		default:
			break;
		}
		QMessageBox::critical( this, title, iomgr->getMessage() );
	}
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
	newMech();
	MechDB* dialog = new MechDB(MechDB::load, this);
	dialog->exec();
}

//
////////////////////////////////

void MainWindow::editPrefs()
{
	PrefsWindow *prefs = new PrefsWindow(this);
	prefs->exec();
}

// help menu
void MainWindow::reportBug()
{
	QMessageBox::StandardButton ret;
	ret = QMessageBox::information( this, "CheMecher",
			"Please, before you report a bug, make sure that it satisfies three requirements:\n\n"
			"1) Repeatability. Make sure you can restart the program and generate the same bug again.\n\n"
			"2) Specificity. Please give as many details as you can when you fill in the"
			" information. We can only fix it if we know what's wrong. This means PLEASE ATTACH ANY"
			" INPUT FILES ASSOCIATED!\n\n"
			"3) Don't infer.  Just tell us what happened, not why you think it happened.  Odds are your"
			" guess will just confuse us and it'll take longer to fix it.\n\n"
			"If you've checked these three requirements, please click OK to continue. If not, hit Cancel to go back"
			" and check them.",
			QMessageBox::Ok | QMessageBox::Cancel ) ;
	if( ret == QMessageBox::Cancel )
		return;
	QDesktopServices::openUrl( QUrl("http://sourceforge.net/tracker/?func=add&group_id=313094&atid=1317696") );
}
void MainWindow::suggestFeature()
{QDesktopServices::openUrl( QUrl("http://sourceforge.net/tracker/?func=add&group_id=313094&atid=1317699") );}
void MainWindow::showAboutWindow()
{
	About about;
	about.exec();
}
