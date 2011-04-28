#include "simparams.h"
#include "ui_simparams.h"

SimParams::SimParams(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimParams)
{
    ui->setupUi(this);
	
	// macros to shorten validator declarations
	#define DV(min,max) (new QDoubleValidator(min,max,30,this))
	#define IV(min,max) (new QIntValidator(min,max,this))
	
	// set validators
	ui->txtPrecision  ->setValidator( DV(0, 100      ));
	ui->txtTimeStep   ->setValidator( DV(0, 1000000  ));
	ui->txtReportStep ->setValidator( DV(0, 1000000  ));
	ui->txtStartTime  ->setValidator( DV(0, 1000000  ));
	ui->txtEndTime    ->setValidator( DV(0, 10000000 ));
	ui->txtDebugStart ->setValidator( IV(0, 1000000  ));
	ui->txtDebugEnd   ->setValidator( IV(0, 10000000 ));
	ui->txtGateband   ->setValidator( DV(0, 1000     ));
	ui->txtShifttest  ->setValidator( IV(0, 100      ));
	ui->txtMaxreduce  ->setValidator( IV(0, 10       ));
	ui->txtStepfactor ->setValidator( DV(0, 10       ));
	
	// initialize fields
	//ui->txtMechName   ->setText( mix->mechName );
	//ui->txtMechDesc   ->setText( mix->mechDesc );
	
	ui->comboOrder->setCurrentIndex( mix->order-1 );
	setOrder( mix->order-1 );
	
	// initialize the method
	QString value = "";
	for( int i=0; i<ui->comboMethod->count(); i++ ) {
		value = ui->comboMethod->itemText(i).toLower();
		if( value.right(2)=="'s" )
			value.chop(2);
		if( mix->method == value )
			ui->comboMethod->setCurrentIndex( i );
	}
	
	// initialize the transition
	ui->comboTrans->setCurrentIndex( mix->transition );
	
	ui->groupAutostep ->setChecked( mix->autostep );
	ui->txtPrecision  ->setText( QString::number( mix->precision  ));
	ui->txtTimeStep   ->setText( QString::number( mix->timeStep   ));
	ui->txtReportStep ->setText( QString::number( mix->reportStep ));
	ui->txtStartTime  ->setText( QString::number( mix->startTime  ));
	ui->txtEndTime    ->setText( QString::number( mix->endTime    ));
	ui->txtDebugStart ->setText( QString::number( mix->debugStart ));
	ui->txtDebugEnd   ->setText( QString::number( mix->debugEnd   ));
	ui->txtGateband   ->setText( QString::number( mix->gateband   ));
	ui->txtShifttest  ->setText( QString::number( mix->shifttest  ));
	ui->txtMaxreduce  ->setText( QString::number( mix->maxreduce  ));
	ui->txtStepfactor ->setText( QString::number( mix->stepfactor ));
	ui->txtOutfile    ->setText( iomgr->outputFile );
	ui->txtLogfile    ->setText( iomgr->logFile );
	ui->txtDebugfile  ->setText( iomgr->debugFile );
	
	// connect slots
	connect( ui->comboOrder,  SIGNAL(currentIndexChanged(int)), this, SLOT(setOrder(int)) );
	connect( ui->comboMethod, SIGNAL(currentIndexChanged(QString)), this, SLOT(setMethod(QString)) );
	connect( ui->comboTrans,  SIGNAL(currentIndexChanged(int)), this, SLOT(setTransition(int)) );
	connect( ui->groupAutostep, SIGNAL(toggled(bool)),        this, SLOT(setAutostep(bool)) );
	
	// stores information about the fields in the form
	doubleparams["precision"  ] = makeParam( &mix->precision,  ui->txtPrecision  );
	doubleparams["timestep"   ] = makeParam( &mix->timeStep,   ui->txtTimeStep   );
	doubleparams["reportstep" ] = makeParam( &mix->reportStep, ui->txtReportStep );
	doubleparams["startsime"  ] = makeParam( &mix->startTime,  ui->txtStartTime  );
	doubleparams["endtime"    ] = makeParam( &mix->endTime,    ui->txtEndTime    );
	intparams   ["debugstart" ] = makeParam( &mix->debugStart, ui->txtDebugStart );
	intparams   ["debugend"   ] = makeParam( &mix->debugEnd,   ui->txtDebugEnd   );
	doubleparams["gateband"   ] = makeParam( &mix->gateband,   ui->txtGateband   );
	intparams   ["shifttest"  ] = makeParam( &mix->shifttest,  ui->txtShifttest  );
	intparams   ["maxreduce"  ] = makeParam( &mix->maxreduce,  ui->txtMaxreduce  );
	doubleparams["stepfactor" ] = makeParam( &mix->stepfactor, ui->txtStepfactor );
	
	// set up the signal mapper
	paramMapper = new QSignalMapper(this);
	paramMapper->setMapping( ui->txtPrecision  , "precision"  );
	paramMapper->setMapping( ui->txtTimeStep   , "timestep"   );
	paramMapper->setMapping( ui->txtReportStep , "reportstep" );
	paramMapper->setMapping( ui->txtStartTime  , "starttime"  );
	paramMapper->setMapping( ui->txtEndTime    , "endtime"    );
	paramMapper->setMapping( ui->txtDebugStart , "debugstart" );
	paramMapper->setMapping( ui->txtDebugEnd   , "debugend"   );
	paramMapper->setMapping( ui->txtGateband   , "gateband"   );
	paramMapper->setMapping( ui->txtShifttest  , "shifttest"  );
	paramMapper->setMapping( ui->txtMaxreduce  , "maxreduce"  );
	paramMapper->setMapping( ui->txtStepfactor , "stepfactor" );
	connect( ui->txtPrecision  , SIGNAL(textChanged(QString)), paramMapper, SLOT(map()) );
	connect( ui->txtTimeStep   , SIGNAL(textChanged(QString)), paramMapper, SLOT(map()) );
	connect( ui->txtReportStep , SIGNAL(textChanged(QString)), paramMapper, SLOT(map()) );
	connect( ui->txtStartTime  , SIGNAL(textChanged(QString)), paramMapper, SLOT(map()) );
	connect( ui->txtEndTime    , SIGNAL(textChanged(QString)), paramMapper, SLOT(map()) );
	connect( ui->txtDebugStart , SIGNAL(textChanged(QString)), paramMapper, SLOT(map()) );
	connect( ui->txtDebugEnd   , SIGNAL(textChanged(QString)), paramMapper, SLOT(map()) );
	connect( ui->txtGateband   , SIGNAL(textChanged(QString)), paramMapper, SLOT(map()) );
	connect( ui->txtShifttest  , SIGNAL(textChanged(QString)), paramMapper, SLOT(map()) );
	connect( ui->txtMaxreduce  , SIGNAL(textChanged(QString)), paramMapper, SLOT(map()) );
	connect( ui->txtStepfactor , SIGNAL(textChanged(QString)), paramMapper, SLOT(map()) );
	connect( paramMapper, SIGNAL(mapped(QString)), this, SLOT(setParameter(QString)) );
	
	// connect the browse buttons
	browseMapper = new QSignalMapper(this);
	browseMapper->setMapping( ui->outputBrowse, "output" );
	browseMapper->setMapping( ui->logBrowse,    "log"    );
	browseMapper->setMapping( ui->debugBrowse,  "debug"  );
	connect( ui->outputBrowse , SIGNAL(clicked()), browseMapper, SLOT(map()) );
	connect( ui->logBrowse    , SIGNAL(clicked()), browseMapper, SLOT(map()) );
	connect( ui->debugBrowse  , SIGNAL(clicked()), browseMapper, SLOT(map()) );
	connect( browseMapper , SIGNAL(mapped(QString)), this, SLOT(browseFile(QString)) );
	
	// connect the output files
	connect( ui->txtOutfile,   SIGNAL(textChanged(QString)), iomgr, SLOT(setOutputFile(QString)) );
	connect( ui->txtLogfile,   SIGNAL(textChanged(QString)), iomgr, SLOT(setLogFile(QString))    );
	connect( ui->txtDebugfile, SIGNAL(textChanged(QString)), iomgr, SLOT(setDebugFile(QString))  );
}

void SimParams::setOrder(int index)
{
	// set the order
	mix->order = index+1;
	
	// set the available methods
	QStringList items;
	switch( mix->order ) {
	case 1:
		items.append("Euler's");
		break;
	case 2:
		items.append("Modified Euler's");
		items.append("Heun's");
		items.append("Ralston's");
		break;
	case 3:
		items.append("Runge-Kutta");
		break;
	case 4:
		items.append("Runge");
		items.append("Kutta");
		items.append("Gill");
		break;
	case 5:
		items.append("Runge-Kutta-Fehlberg");
		break;
	}
	ui->comboMethod->clear();
	ui->comboMethod->addItems(items);
	
	updateAutostep();
}

void SimParams::setAutostep(bool state)
{
	// update the mix object
	mix->setAutostep(state);
	updateAutostep();
}

void SimParams::updateAutostep()
{
	//std::cout << "autostep: " << mix->autostep << std::endl;
	
	// toggle the precision field with the autostep fields
	if( mix->order == 5 ){
		// enable precision fields
		ui->lblPrecision->setEnabled(true);
		ui->txtPrecision->setEnabled(true);
		ui->groupAutostep->setCheckable(false);
	} 
	else {
		// disable precision fields
		ui->lblPrecision->setEnabled(false);
		ui->txtPrecision->setEnabled(false);
		if( !ui->groupAutostep->isCheckable() ){
			ui->groupAutostep->setCheckable(true);
		}
	}
	
	// enable now (maybe) useful fields
	if( mix->order != 5 && mix->autostep ){
		ui->lblGateband->setEnabled(true);    ui->txtGateband->setEnabled(true);
		ui->lblMaxreduce->setEnabled(true);   ui->txtMaxreduce->setEnabled(true);
		ui->lblShifttest->setEnabled(true);   ui->txtShifttest->setEnabled(true);
		ui->lblStepfactor->setEnabled(true);  ui->txtStepfactor->setEnabled(true);
	}
	else {
		// disable useless fields
		ui->lblGateband->setEnabled(false);   ui->txtGateband->setEnabled(false);
		ui->lblMaxreduce->setEnabled(false);  ui->txtMaxreduce->setEnabled(false);
		ui->lblShifttest->setEnabled(false);  ui->txtShifttest->setEnabled(false);
		ui->lblStepfactor->setEnabled(false); ui->txtStepfactor->setEnabled(false);
	}
}

void SimParams::setMethod(QString value)
{
	value = value.toLower();
	if( value.right(2)=="'s" )
		value.chop(2);
	mix->method = value;
}

void SimParams::setTransition(int value)
{
	mix->transition = value;
}

void SimParams::browseFile(QString field)
{
	QFileDialog dlg;
	
	// set up the file dialog
	dlg.setDefaultSuffix("*.txt");
	dlg.setNameFilter("All files (*)");
	if( field=="log" ) dlg.setAcceptMode(QFileDialog::AcceptOpen);
	else               dlg.setAcceptMode(QFileDialog::AcceptSave);
	
	// run the file dialog
	if( dlg.exec() == QDialog::Accepted ){
		if(field=="output") ui->txtOutfile  ->setText( dlg.selectedFiles().at(0) );
		if(field=="log"   ) ui->txtLogfile  ->setText( dlg.selectedFiles().at(0) );
		if(field=="debug" ) ui->txtDebugfile->setText( dlg.selectedFiles().at(0) );
	}
}

void SimParams::setParameter(QString name)
{
	if( doubleparams.keys().contains(name) )
		*doubleparams[name]->doubleval = doubleparams[name]->field->text().toDouble();
	if( intparams.keys().contains(name) )
		*intparams[name]->intval = intparams[name]->field->text().toInt();
}

ParamInfo* SimParams::makeParam(double *dval, QLineEdit *field)
{
	ParamInfo* ret = new ParamInfo();
	ret->doubleval = dval;
	ret->field = field;
	return ret;
}

ParamInfo* SimParams::makeParam(int *ival, QLineEdit *field)
{
	ParamInfo* ret = new ParamInfo();
	ret->intval = ival;
	ret->field = field;
	return ret;
}

SimParams::~SimParams()
{
    delete ui;
}

void SimParams::changeEvent(QEvent *e)
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
