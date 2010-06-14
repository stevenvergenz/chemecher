/*
 * Main Window
 * This window houses the primary components of CheMecher v4
 * Created by Steven Vergenz
 * On April 3, 2010
 */

#ifndef _CPDWINDOW_H
#define _CPDWINDOW_H

#include <QFrame>

#include "ui_cpd.h"
#include "cpd.h"

class CpdWindow : public QFrame
{
	Q_OBJECT

public:
	CpdWindow(Cpd* base, QWidget* parent = 0);
	Cpd* base(){ return baseCpd; }

public slots:
	void hideOnHomo(int newstate);
	void hideOnTrans(int newtrans);

signals:

private slots:
	void toHTML(); //changes long name field from editable text to markup
	void toEditable(); //changes markup to editable

private:
	Ui::cpdWindow ui;
	Cpd* baseCpd;

};

#endif

