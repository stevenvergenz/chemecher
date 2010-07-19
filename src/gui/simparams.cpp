#include "simparams.h"
#include "ui_simparams.h"

SimParams::SimParams(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimParams)
{
    ui->setupUi(this);
	
	// stores information about the fields in the form
	doubleparams["precision"  ] = &mix->precision  ;
	doubleparams["timestep"   ] = &mix->timeStep   ;
	doubleparams["reportstep" ] = &mix->reportStep ;
	doubleparams["startsime"  ] = &mix->startTime  ;
	doubleparams["endtime"    ] = &mix->endTime    ;
	intparams   ["debugstart" ] = &mix->debugStart ;
	intparams   ["debugend"   ] = &mix->debugEnd   ;
	intparams   ["gateband"   ] = &mix->gateband   ;
	intparams   ["shifttest"  ] = &mix->shifttest  ;
	intparams   ["maxreduce"  ] = &mix->maxreduce  ;
	intparams   ["stepfactor" ] = &mix->stepfactor ;
	
	// macros to shorten validator declarations
	#define DV(min,max) (new QDoubleValidator(min,max,30,this))
	#define IV(min,max) (new QIntValidator(min,max,this))
	
	// set the validators
	ui->txtPrecision  ->setValidator( DV(0, 100      ));
	ui->txtTimeStep   ->setValidator( DV(0, 1000000  ));
	ui->txtReportStep ->setValidator( DV(0, 1000000  ));
	ui->txtStartTime  ->setValidator( DV(0, 1000000  ));
	ui->txtEndTime    ->setValidator( DV(0, 10000000 ));
	ui->txtDebugStart ->setValidator( IV(0, 1000000  ));
	ui->txtDebugEnd   ->setValidator( IV(0, 10000000 ));
	ui->txtGateband   ->setValidator( IV(0, 1000     ));
	ui->txtShifttest  ->setValidator( IV(0, 1000     ));
	ui->txtMaxreduce  ->setValidator( IV(0, 10000000 ));
	ui->txtStepfactor ->setValidator( IV(0, 1000     ));
	
	// initialize fields
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
	
	// connect slots
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
	
}

void SimParams::setParameter(QString name)
{
	if( doubleparams.keys().contains(name) ) {
		//*doubleparams[name] = 
	}
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
