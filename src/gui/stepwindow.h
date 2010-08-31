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
#include <QDoubleValidator>

#include "ui_stepwindow.h"
#include "step.h"
#include "mix.h"
#include "mainwindow.h"

class StepWindow : public QWidget
{
	Q_OBJECT

public:
	friend class MainWindow;
	
	StepWindow(Step* base, QWidget* parent = 0, bool isnew=false);
	
	typedef struct {
		QList<QComboBox*>   lstCombos;
		QList<QPushButton*> lstRems;
		QList<QLabel*>      lstPlus;
		QFrame*             frame;
		QGridLayout*        layout;
		QPushButton*        addButton;
	} ReagentBox_t;
	
public slots:
	// for new steps
	void validate();
	void checkValidationState();
	void setBottomEnabled( bool val );
	
	void refreshReagentBoxConnections();
	void disconnectReagentBoxes();
	void connectReagentBoxes();
	void updateCpdLists();
	
	void setReagents();
	
	void addCpd( ReagentBox_t* r, bool addtobase=true );
	void addReac();
	void addProd();
	
	void remReacButton();
	void remProdButton();
	void remCpd( ReagentBox_t* r, int i );
	void remAllCpds();
	/*void remReac1(); void remReac2(); void remReac3();
	void remProd1(); void remProd2(); void remProd3();*/

signals:
	void validated();
	
	void addCpdClicked();

protected:
	/*void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);*/
	
private:
	Ui::stepWindow ui;
	Step* baseStep;
	ReagentBox_t *reactants, *products;

};

#endif

