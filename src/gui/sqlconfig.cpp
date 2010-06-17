#include "sqlconfig.h"
#include "ui_sqlconfig.h"

SqlConfig::SqlConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SqlConfig)
{
    ui->setupUi(this);
	
	// connect the apply button
	connect( ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()),
			 this, SLOT(apply()) );
	connect( ui->hostname, SIGNAL(textChanged(QString)),
			 this, SLOT(changed()) );
	connect( ui->username, SIGNAL(textChanged(QString)),
			 this, SLOT(changed()) );
	connect( ui->password, SIGNAL(textChanged(QString)),
			 this, SLOT(changed()) );
			
	ui->hostname->setText(db->getHostname());
	ui->username->setText(db->getUsername());
	ui->password->setText(db->getPassword());	
}

void SqlConfig::apply()
{
	db->setHostName(ui->hostname->text());
	db->setUserName(ui->username->text());
	db->setPassword(ui->password->text());
	changed();
}

void SqlConfig::changed()
{
	if( ui->hostname->text()==db->hostName() &&
		ui->username->text()==db->userName() &&
		ui->password->text()==db->password() )
		ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
	else
		ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
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
