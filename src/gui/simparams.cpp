#include "simparams.h"
#include "ui_simparams.h"

SimParams::SimParams(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimParams)
{
    ui->setupUi(this);
	
	QDoubleValidator *val = new QDoubleValidator(0., 100., 30, 0);
	ui->txtPrecision->setValidator(val);
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
