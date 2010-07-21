#ifndef PREFS_H
#define PREFSWINDOW_H

#include <QDialog>
#include <QSettings>
#include <QMessageBox>
#include <QAbstractButton>
#include <QVariant>
#include <QSignalMapper>
#include <QDebug>

#include "ui_prefswindow.h"
namespace Ui {
    class PrefsWindow;
}

class PrefsWindow : public QDialog {
    Q_OBJECT
public:
    PrefsWindow(QWidget *parent = 0);
    ~PrefsWindow();

private:	
	void setUpPreferences();
	
private slots:
	void setData( int val );
	void setData( bool val );
	void setData( double val );
	void setData( QString val );
	void commitPref( QString key );
	
signals:
	void initPref( int val );
	void initPref( bool val );
	void initPref( double val );
	void initPref( QString val );
	
private:
	
	// for connecting the preferences
	union {
		int intval;
		double doubleval;
		QString *QStringval;
		bool boolval;
	} data;
	enum type_t {v_int, v_bool, v_double, v_QString} data_type;
	QSignalMapper *prefMapper;
	
	// for storing the preferences
	struct pref_t {
		type_t data_type;
		QWidget *widget;
		const char* signal;
		const char* slot;
		pref_t() {}
		pref_t( type_t _data_type, QWidget *_widget,const char*  _signal, const char* _slot )
			: data_type(_data_type), widget(_widget), 
			 signal(_signal), slot(_slot)
		{}
	};
	QMap<QString, pref_t> prefs;
	
    Ui::PrefsWindow *ui;
};

// define the macro that adds a preference
#define ADD_PREF( key, type, widget, signal, slot ) do { \
	prefs[key] = pref_t( v_##type, widget, signal, slot ); \
	connect( widget, signal, this, SLOT(setData(type)) ); \
	prefMapper->setMapping( widget, key ); \
	connect( widget, signal, prefMapper, SLOT(map()) ); \
} while(0);

#endif // PREFSWINDOW_H
