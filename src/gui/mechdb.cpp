#include "mechdb.h"
#include "ui_mechdb.h"

mechdb::mechdb(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mechdb)
{
    ui->setupUi(this);
}

mechdb::~mechdb()
{
    delete ui;
}

void mechdb::changeEvent(QEvent *e)
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
