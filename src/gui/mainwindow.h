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

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0);

public slots:
	
	// cpd editing stuff
	void addCpd();
	void editCpdWindow(QListWidgetItem*);
	void removeCpd();
	void moveCpdUp();
	void moveCpdDown();
	
	void updateCpdList();
	void updateStepList();
	
	// saving/loading
	void saveMechDb();
	void loadMechDb();

signals:

private:
	Ui::mainWindow ui;
	//QList<QMdiSubWindow*> windowlist;
	QMdiArea* mdi;

};

#endif

