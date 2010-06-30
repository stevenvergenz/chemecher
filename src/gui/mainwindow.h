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

class MainWindow;

#include "stepwindow.h"
#include "cpdwindow.h"
#include "mechdb.h"
#include "mix.h"
#include "draglistwidget.h"

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0);
	
	QMdiArea* mdi;

public slots:
	
	// cpd editing stuff
	void showCpdWindow( QListWidgetItem* item = 0 );
	//void editCpdWindow(QListWidgetItem*);
	void removeCpd();
	void moveCpdUp();
	void moveCpdDown();
	void updateCpdList();
	
	// step editing stuff
	void addStep();
	void editStepWindow(QListWidgetItem*);
	void removeStep();
	void moveStepUp();
	void moveStepDown();
	void updateStepList();
	
	// saving/loading
	void saveMechDb();
	void loadMechDb();

signals:

private:
	Ui::mainWindow ui;
	DragListWidget *lstCpds;
	//QList<QMdiSubWindow*> windowlist;

};

#endif

