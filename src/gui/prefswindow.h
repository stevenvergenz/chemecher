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
	
	void setUpPreferences();
	
public slots:
	//void accept();
	//void apply( QAbstractButton *button );
	
private slots:
	void setData( int val );
	void setData( bool val );
	void setData( double val );
	void setData( QString val );
	void commitPref( QString key );
	
private:
	union {
		int intval;
		double doubleval;
		QString *stringval;
		bool boolval;
	} data;
	enum {v_int, v_bool, v_double, v_string} data_type;
	QSignalMapper *prefMapper;
	
    Ui::PrefsWindow *ui;
};

// define the macro that adds a preference
#define ADD_PREF( key, type, widget, signal ) do { \
	connect( widget, signal, this, SLOT(setData(type)) ); \
	prefMapper->setMapping( widget, key ); \
	connect( widget, signal, prefMapper, SLOT(map()) ); \
} while(0);

#endif // PREFSWINDOW_H
