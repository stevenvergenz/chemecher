/*
 * Step Window
 * This is the MDI subwindow that opens when you doubleclick a step entry
 * Created by Steven Vergenz
 * On April 6, 2010
 */

#ifndef _STEPWINDOW_H
#define _STEPWINDOW_H

#include <QWidget>
#include <QStringList>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QPaintEngine>
#include <QComboBox>

#include "ui_stepwindow.h"
#include "step.h"
#include "mix.h"
#include "mainwindow.h"

class StepWindow : public QWidget
{
	Q_OBJECT

public:
	StepWindow(Step* base, MainWindow *main, QWidget* parent = 0, bool isnew=false);
	
	MainWindow *mainparent;
	
	typedef enum {Reactant, Product} ReagentType;
	
public slots:
	void updateCpdLists();
	
	void addCpd( ReagentType t );
	void addReac();
	void addProd();

signals:

protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	
private:
	Ui::stepWindow ui;
	Step* baseStep;
	
	QList<QComboBox*> lstComboReacs;
	QList<QComboBox*> lstComboProds;
	QList<QPushButton*> lstPushRemReacs;
	QList<QPushButton*> lstPushRemProds;
	QList<QLabel*> lstLblPlusReacs;
	QList<QLabel*> lstLblPlusProds;

};

#endif

