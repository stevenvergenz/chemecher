#ifndef _CPDWINDOW_H
#define _CPDWINDOW_H

#include <QFrame>
#include <QMessageBox>

#include "ui_cpdwindow.h"
#include "cpd.h"
#include "mainwindow.h"

class CpdWindow : public QFrame
{
	Q_OBJECT

public:
	CpdWindow(Cpd* base, MainWindow *main, QWidget* parent = 0, bool isnew=false );
	Cpd* base(){ return baseCpd; }
	
	MainWindow *mainparent;

public slots:
	// for new cpds
	void validate();
	void checkValidationState();
	
	void enableBottom();
	void disableBottom();
	
	// for existing cpds
	void updateForm();

signals:

private slots:
	//void toHTML(); //changes long name field from editable text to markup
	//void toEditable(); //changes markup to editable

private:
	Ui::cpdWindow ui;
	Cpd* baseCpd;

};

#endif

