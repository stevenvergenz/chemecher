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
#include <QSignalMapper>
#include <QDesktopServices>
#include <QPrinter>
#include <QPrintDialog>

class MainWindow;

#include "stepwindow.h"
#include "cpdwindow.h"
#include "mechdb.h"
#include "mix.h"
#include "iomanager.h"
#include "about.h"
#include "simparams.h"
#include "prefswindow.h"

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0);
	
	QMdiArea* mdi;

public slots:
	
	void initTestMech();
	
	// cpd editing stuff
	void showCpdWindow( QString action );
	void cpdListDoubleClicked( int r, int c );
	void deleteCpd( int pos = -1 );
	void deleteAllCpds();
	void moveCpdUp();
	void moveCpdDown();
	void updateCpdList();
	void setCpdInitConc( QTableWidgetItem* item );
	
	// step editing stuff
	void showStepWindow( QString action );
	void deleteStep( int pos = -1 );
	void deleteAllSteps();
	void moveStepUp();
	void moveStepDown();
	void updateStepList();
	
	void updateEditButtonEnabled();
	
	void editSimParams();
	
	void cpdContextMenu ( QPoint pos );
	void stepContextMenu( QPoint pos );
	
	// file menu
	void newMech( bool val = true, bool ask = true );
	bool confirmNewMech();
	void save();
	void saveToCM4();
	void loadFromCM4();
	void saveToCM3();
	void loadFromCM3();
	void saveMechDb();
	void loadMechDb();
	void print();
	
	void editPrefs();
	
	void reportBug();
	void suggestFeature();
	void showAboutWindow();
	
signals:

private:
	Ui::mainWindow ui;
	
	QSignalMapper *stepMapper, *cpdMapper;
	
	typedef enum{CPDWIN,STEPWIN} SubWindowType;
	QMap<QMdiSubWindow*, SubWindowType> windowtypes;
	void purgeWindowTypes();
	

};

#endif

