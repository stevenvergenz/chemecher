#include "prefswindow.h"

void PrefsWindow::setUpPreferences()
{
	/** Add preferences here ********
	* 
	* Syntax: ADD_PREF( <key_name>, <data_type>, <widget>, <changesignal> );
	*       Ex. ADD_PREF( "an_int_preference", int, ui->spinIntParam, SIGNAL(valueChanged(int)) );
	* 
	*   <key_name>     = The string used to refer to the key elsewhere in the program
	*   <data_type>    = The type of the data
	*       MUST BE ONE OF FOLLOWING: int, bool, double, QString
	*   <widget>       = The widget associated with changing the preference's value
	*   <changesignal> = The signal emitted from <widget> that sets the preference's value
	*                       Note: include SIGNAL macro and data type, ex. "SIGNAL(valueChanged(int))"
	* 
	* To reference a setting later, see the documentation on QSettings::value.
	**/
	
	// not yet finished, still have to set up widget initialization
	
	ADD_PREF( "io/line_up_whitespace", bool, ui->chkLineUpWhitespace, SIGNAL(toggled(bool)) );
}

PrefsWindow::PrefsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrefsWindow)
{
    ui->setupUi(this);
	
	// connect signal mapper
	prefMapper = new QSignalMapper(this);
	connect( prefMapper, SIGNAL(mapped(QString)), this, SLOT(commitPref(QString)) );
	
	// set up all the user-defined preferences
	setUpPreferences();
	
}

/*
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
*/

// sets the data for the key
void PrefsWindow::setData( int val )
{
	data_type = v_int;
	data.intval = val;
}
void PrefsWindow::setData( bool val )
{
	data_type = v_bool;
	data.boolval = val;
}
void PrefsWindow::setData( double val )
{
	data_type = v_double;
	data.doubleval = val;
}
void PrefsWindow::setData( QString val )
{
	data_type = v_string;
	data.stringval = new QString(val);
}

void PrefsWindow::commitPref(QString key)
{
	QSettings settings;
	switch( data_type ) {
	case v_int:
		settings.setValue( key, QVariant(data.intval) );
		qDebug() << data.intval;
		break;
	case v_bool:
		settings.setValue( key, QVariant(data.boolval) );
		qDebug() << data.boolval;
		break;
	case v_double:
		settings.setValue( key, QVariant(data.doubleval) );
		qDebug() << data.doubleval;
		break;
	case v_string:
		settings.setValue( key, QVariant(*data.stringval) );
		qDebug() << *data.stringval;
		break;
	}
}

PrefsWindow::~PrefsWindow()
{
    delete ui;
	delete prefMapper;
}
