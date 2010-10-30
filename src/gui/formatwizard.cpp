#include "formatwizard.h"
#include "ui_formatwizard.h"

FormatWizard::FormatWizard(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FormatWizard)
{
    ui->setupUi(this);
}

FormatWizard::~FormatWizard()
{
    delete ui;
}

void FormatWizard::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
