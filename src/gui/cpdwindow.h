#ifndef _CPDWINDOW_H
#define _CPDWINDOW_H

#include <QFrame>

#include "ui_cpd.h"
#include "cpd.h"

class CpdWindow : public QFrame
{
	Q_OBJECT

public:
	CpdWindow(Cpd* base, QWidget* parent = 0, bool isnew=false );
	Cpd* base(){ return baseCpd; }

public slots:
	// for new cpds
	void validateAndAdd();
	void setUpBottomHalf();
	
	// for existing cpds
	void updateForm();

signals:

private slots:
	void toHTML(); //changes long name field from editable text to markup
	void toEditable(); //changes markup to editable

private:
	Ui::cpdWindow ui;
	Cpd* baseCpd;

};

#endif

