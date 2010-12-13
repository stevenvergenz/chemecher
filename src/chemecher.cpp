#include <QApplication>
#include <QDebug>
#include <iostream>
#include <cstring>
#include "mainwindow.h"

int main(int argc, char** argv)
{
	bool isCLI = false;
	
	// parse over the arguments for the initialization param
	for(int i=0; i<argc; i++){
		if( strcmp(argv[i], "--non-interactive")==0 ){
			isCLI = true;
			break;
		}
	}
	
	if( isCLI ){
		// non-interactive flag is set, don't launch GUI
		
	}
	else {
		// normal behavior: launch GUI
		QApplication app(argc, argv);
		app.setApplicationName("Chemecher");
		app.setApplicationVersion("4.0.0");
	
		MainWindow ui;
		ui.show();
		return app.exec();
	}
}

