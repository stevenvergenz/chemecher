#include "mechproperties.h"
#include "ui_mechproperties.h"

MechProperties::MechProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MechProperties)
{
    ui->setupUi(this);
    
}

MechProperties::~MechProperties()
{
    delete ui;
}

QString MechProperties::name(){
	return ui->txtName->text();
}

QString MechProperties::description(){
	return ui->txtDescription->toPlainText();
}

void MechProperties::setName(QString n){
	ui->txtName->setText(n);
}

void MechProperties::setDescription(QString d){
	ui->txtDescription->setPlainText(d);
}
