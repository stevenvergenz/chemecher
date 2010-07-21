#include "prefswindow.h"

void PrefsWindow::setUpPreferences()
{
	/** Add preferences here ********
	* 
	* Syntax: ADD_PREF( <key_name>, <data_type>, <widget>, <changesignal>, <initslot> );
	*       Ex. ADD_PREF( "an_int_preference", int, ui->spinIntParam, SIGNAL(valueChanged(int)), SLOT(setValue(int)) );
	* 
	*   <key_name>     = The string used to refer to the preference elsewhere in the program
	*   <data_type>    = The type of the data
	*       MUST BE ONE OF FOLLOWING: int, bool, double, QString
	*   <widget>       = A pointer to the widget associated with changing the preference's value
	*   <changesignal> = The signal emitted from <widget> that sets the preference's value
	*                       Note: include SIGNAL macro and data type, ex. "SIGNAL(valueChanged(int))"
	*   <initslot>     = The slot for <widget> that is used to set initialize its value
	*                       Note: include SLOT macro and data type, ex. "SLOT(setValue(int))"
	* 
	* To reference a setting later, see the documentation on QSettings::value.
	* 
	**/
	
	ADD_PREF( "iocm3/line_up_whitespace", bool, ui->chkLineUpWhitespace, SIGNAL(toggled(bool)), SLOT(setChecked(bool)) );
}

PrefsWindow::PrefsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrefsWindow)
{
    ui->setupUi(this);
	
	// connect signal mapper
	prefMapper = new QSignalMapper(this);
	connect( prefMapper, SIGNAL(mapped(QString)), this, SLOT(commitPref(QString)) );
	
	// set up all the macro-defined preferences
	setUpPreferences();
	
	// initialize the widgets
	QSettings settings;
	foreach( QString key, prefs.keys() ) {
		switch( prefs[key].data_type ) {
		case v_int:
			connect( this, SIGNAL(initPref(int)), prefs[key].widget, prefs[key].slot );
			emit initPref( settings.value(key, 0).toInt() );
			disconnect( this, SIGNAL(initPref(int)), prefs[key].widget, prefs[key].slot );
			break;
		case v_bool:
			connect( this, SIGNAL(initPref(bool)), prefs[key].widget, prefs[key].slot );
			emit initPref( settings.value(key, 0).toBool() );
			disconnect( this, SIGNAL(initPref(bool)), prefs[key].widget, prefs[key].slot );
			break;
		case v_double:
			connect( this, SIGNAL(initPref(double)), prefs[key].widget, prefs[key].slot );
			emit initPref( settings.value(key, 0).toDouble() );
			disconnect( this, SIGNAL(initPref(double)), prefs[key].widget, prefs[key].slot );
			break;
		case v_QString:
			connect( this, SIGNAL(initPref(QString)), prefs[key].widget, prefs[key].slot );
			emit initPref( settings.value(key, 0).toString() );
			disconnect( this, SIGNAL(initPref(QString)), prefs[key].widget, prefs[key].slot );
			break;
		}
	}
	
}

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
	data_type = v_QString;
	data.QStringval = new QString(val);
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
	case v_QString:
		settings.setValue( key, QVariant(*data.QStringval) );
		qDebug() << *data.QStringval;
		delete data.QStringval;
		break;
	}
}



PrefsWindow::~PrefsWindow()
{
    delete ui;
	delete prefMapper;
}
