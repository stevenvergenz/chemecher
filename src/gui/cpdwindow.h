#ifndef _CPDWINDOW_H
#define _CPDWINDOW_H

#include <QFrame>
#include <QMessageBox>

#include "ui_cpdwindow.h"
#include "cpd.h"
#include "mainwindow.h"
#include "mix.h"

class CpdWindow : public QWidget
{
	Q_OBJECT

public:
	friend class MainWindow;
	
	CpdWindow(Cpd* base, QWidget* parent = 0, bool isnew=false );
	~CpdWindow();
	Cpd* base(){ return baseCpd; }

public slots:
	// for new cpds
	void validate();
	void checkValidationState();
	void setBottomEnabled( bool val );
	
	// for existing cpds
	void updateForm();

signals:
	void validated();

private slots:
	//void toHTML(); //changes long name field from editable text to markup
	//void toEditable(); //changes markup to editable

private:
	Ui::cpdWindow *ui;
	Cpd* baseCpd;

};

#endif

