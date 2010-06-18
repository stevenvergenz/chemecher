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
	connect( ui->hostname, SIGNAL(returnPressed()),
			 this, SLOT(apply()) );
	connect( ui->username, SIGNAL(returnPressed()),
			 this, SLOT(apply()) );
	connect( ui->password, SIGNAL(returnPressed()),
			 this, SLOT(apply()) );
			
	ui->hostname->setText(db.getHostName());
	ui->username->setText(db.getUserName());
	ui->password->setText(db.getPassword());	
}

void SqlConfig::apply()
{
	db.setHostName(ui->hostname->text());
	db.setUserName(ui->username->text());
	db.setPassword(ui->password->text());
	changed();
}

void SqlConfig::changed()
{
	if( getHostName()==db.getHostName() &&
		getUserName()==db.getUserName() &&
		getPassword()==db.getPassword() )
		ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
	else
		ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
}

// info getters
QString SqlConfig::getHostName()
{return ui->hostname->text();}
QString SqlConfig::getUserName()
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
