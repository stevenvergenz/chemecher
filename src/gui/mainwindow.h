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

#include "stepwindow.h"
#include "cpdwindow.h"
#include "newcpddialog.h"
#include "mechdb.h"

#include "ui_main.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0);

public slots:
	void addNewCpd();
	void showCpdWindow(QListWidgetItem*);
	void saveMechDb();
	void loadMechDb();

signals:

private:
	Ui::mainWindow ui;
	//QList<QMdiSubWindow*> windowlist;
	QMdiArea* mdi;

};

#endif

