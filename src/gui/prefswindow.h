#ifndef PREFS_H
#define PREFSWINDOW_H

#include <QDialog>
#include <QSettings>
#include <QMessageBox>
#include <QAbstractButton>
#include <QVariant>
#include <QSignalMapper>

#include "ui_prefswindow.h"
namespace Ui {
    class PrefsWindow;
}

class PrefsWindow : public QDialog {
    Q_OBJECT
public:
    PrefsWindow(QWidget *parent = 0);
    ~PrefsWindow();

public slots:
	void accept();
	void apply( QAbstractButton *button );
	
private slots:
	void setData( int intval );
	void setData( double doubleval );
	void setData( QString stringval );
	void setData( bool boolval );
	void commitPref( QString key );
	
private:
	union{
		int intval;
		double doubleval;
		QString *stringval;
		bool boolval;
	} val;
	QSignalMapper prefMapper;
	
    Ui::PrefsWindow *ui;
};

#endif // PREFSWINDOW_H
