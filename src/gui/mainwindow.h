/*
 * Main Window
 * This window houses the primary components of CheMecher v4
 * Created by Steven Vergenz
 * On April 3, 2010
 */

#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QList>
#include <QUrl>
#include <QFileDialog>
#include <QTableWidgetItem>
#include <QMap>
#include <QDir>

class MainWindow;

#include "stepwindow.h"
#include "cpdwindow.h"
#include "mechdb.h"
#include "mix.h"
//#include "draglistwidget.h"
#include "iomanager.h"

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0);
	
	QMdiArea* mdi;

public slots:
	
	// cpd editing stuff
	void showCpdWindow( QTableWidgetItem* item = 0 );
	void removeCpd();
	void moveCpdUp();
	void moveCpdDown();
	void updateCpdList();
	void setCpdInitConc( QTableWidgetItem* item );
	
	// step editing stuff
	void showStepWindow( QTableWidgetItem* item = 0 );
	void removeStep();
	void moveStepUp();
	void moveStepDown();
	void updateStepList();
	
	void updateEditButtonEnabled();
	
	// saving/loading
	void saveAs();
	void saveMechDb();
	void loadMechDb();

signals:

private:
	Ui::mainWindow ui;
	QTableWidget *lstCpds;
	
	typedef enum{CPDWIN,STEPWIN} SubWindowType;
	QMap<QMdiSubWindow*, SubWindowType> windowtypes;
	void purgeWindowTypes();
	

};

#endif

