/*
 * Step Window
 * This is the MDI subwindow that opens when you doubleclick a step entry
 * Created by Steven Vergenz
 * On April 6, 2010
 */

#ifndef _STEPWINDOW_H
#define _STEPWINDOW_H

#include <QFrame>
#include <QStringList>

#include "ui_stepwindow.h"
#include "step.h"
#include "mix.h"

class StepWindow : public QFrame
{
	Q_OBJECT

public:
	StepWindow(Step* step, QWidget* parent = 0);

public slots:
	void updateReagentBoxes();
	void setBaseKPlus();
	void setBaseKMinus();
	void setWindowKPlus(double);
	void setWindowKMinus(double);

signals:

private:
	Ui::stepWindow ui;
	Step* baseStep;

};

#endif

