#include "sqlconfig.h"
#include "ui_sqlconfig.h"

SqlConfig::SqlConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SqlConfig)
{
    ui->setupUi(this);
	connect( ui->hostname, SIGNAL(textChanged(QString)), &db, SLOT(setHostname(QString)));
	connect( ui->username, SIGNAL(textChanged(QString)), &db, SLOT(setUsername(QString)));
	connect( ui->password, SIGNAL(textChanged(QString)), &db, SLOT(setPassword(QString)));
}

// info getters
QString SqlConfig::getHostname()
{return ui->hostname->text();}
QString SqlConfig::getUsername()
{return ui->username->text();}
QString SqlConfig::getPassword()
{return ui->password->text();}

SqlConfig::~SqlConfig()
{
    delete ui;
}

void SqlConfig::changeEvent(QEvent *e)
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
