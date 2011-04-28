#include "runwindow.h"
#include "ui_runwindow.h"

RunWindow::RunWindow(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::RunWindow),
    timer(new QTimer)
{
    ui->setupUi(this);
    ui->progressBar->setMinimum(mix->startTime);
    ui->progressBar->setMaximum(mix->endTime);
    
    // manage the clocks
    timer->setInterval(200);
    timer->setSingleShot(false);
    connect( timer,             SIGNAL(timeout()), this, SLOT(updateIndicators()) );
    connect( ui->pushCalculate, SIGNAL(clicked()), this, SLOT(startCalculation()) );
    connect( mix,               SIGNAL(finished()),this, SLOT(finishCalculation()) );
}

RunWindow::~RunWindow()
{
    timer->stop();
    delete timer;
    delete ui;
}

void RunWindow::updateIndicators()
{
	// only update if calculation is running
	//if( endTime < startTime ) return;
	
	int elapsed = startTime.elapsed()/1000;

	ui->lblStartTime->setText( startTime.toString());
	ui->lblElapsedTime->setText(
		QString("%1:%2:%3")
			.arg(elapsed/3600,    2, 10, QChar('0'))
			.arg(elapsed%3600/60, 2, 10, QChar('0'))
			.arg(elapsed%60,      2, 10, QChar('0'))
	);
	
	// based on progress
	// estimated total length of process in seconds
	int calcLength = elapsed * (mix->endTime - mix->startTime) / (mix->time - mix->startTime);
	
	ui->lblETA->setText( startTime.addSecs(calcLength).toString() );
	ui->lblTimeRemaining->setText(
		QString("%1:%2:%3")
			.arg((calcLength-elapsed)/3600,    2, 10, QChar('0'))
			.arg((calcLength-elapsed)%3600/60, 2, 10, QChar('0'))
			.arg((calcLength-elapsed)%60,      2, 10, QChar('0'))
	);
	
	ui->progressBar->setValue(mix->time);
}

void RunWindow::startCalculation()
{
	startTime = QTime::currentTime();
	ui->pushCalculate->setText("Abort");
	disconnect(ui->pushCalculate,SIGNAL(clicked()));
	connect( ui->pushCalculate, SIGNAL(clicked()), this, SLOT(abortCalculation()) );
	updateIndicators();
	timer->start(200);
	
	// begin calculation
	mix->start();
}

void RunWindow::abortCalculation()
{
	mix->cancel = true;
}

void RunWindow::finishCalculation()
{
	disconnect( ui->pushCalculate, SIGNAL(clicked()) );
	connect( ui->pushCalculate, SIGNAL(clicked()), this, SLOT(startCalculation()) );
	ui->pushCalculate->setText("Calculate!");
	timer->stop();
	endTime = QTime::currentTime();
	QApplication::beep();
	mix->cancel = false;
}
