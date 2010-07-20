#include "prefswindow.h"

PrefsWindow::PrefsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrefsWindow)
{
    ui->setupUi(this);
	
	// connect apply button
	connect( ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(apply(QAbstractButton*)) );
	
	#define ADDPREF_INT(key, widget, signal) { \
		connect( widget)
	
	// connect preference stuff
	connect( ui->spinAnswer, SIGNAL(valueChanged(int)),  );
}

void PrefsWindow::accept()
{
	apply(0);
	QDialog::accept();
}

void PrefsWindow::apply( QAbstractButton *button )
{
	if( button!=0 && button->text()!="Apply" )
		return;
	
	// loop through all available preferences
	// set them
}

void PrefsWindow::setData(QVariant val)
{
	QMessageBox::information( this, "blah", val.toString() );
}

void PrefsWindow::commitPref(QString key)
{
	
}

PrefsWindow::~PrefsWindow()
{
    delete ui;
}
