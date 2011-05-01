#include <QApplication>
#include <QDebug>
#include <iostream>
#include <cstring>
#include "mainwindow.h"

using namespace std;

int mainNoGUI(int argc, char** argv);
void printUsage();

int main(int argc, char** argv)
{
	bool isCLI = false;
	
	// parse over the arguments for the initialization param
	for(int i=1; i<argc; i++){
		if( strcmp(argv[i], "--non-interactive")==0 ){
			isCLI = true;
			break;
		}

		if( strcmp(argv[i], "--help")==0 ){
			printUsage();
			return 0;
		}
	}
	
	if( isCLI ){
		// non-interactive flag is set, don't launch GUI
		return mainNoGUI(argc, argv);
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


// secondary main function
// handles the few basic operations required to begin calculation
int mainNoGUI(int argc, char** argv)
{
	QString input, output;
	int argcount = 0;
	
	// get the two necessary arguments: input and output files
	for(int i=1; i<argc; i++){
		if( argv[i][0] == '-' ) continue;

		argcount++;
		if( argcount == 1 )
			input = argv[i];
		else if( argcount == 2 )
			output = argv[i];
		else{
			cout << "Too many filename arguments!" << endl;
			printUsage();
			return 1;
		}
	}
	
	// make sure that both arguments were specified
	if( argcount < 2 ){
		cout << "Must specify both an input and an output file!" << endl;
		printUsage();
		return 1;
	}

	// parse input
	if( !iomgr->loadFromCM4(input) ){
		cout << "Input filename: " << input.toStdString() << endl;
		cout << "Error parsing input: " << iomgr->getMessage().toStdString() << endl;
		return 1;
	}

	// set up the output files
	iomgr->outputFile = output;
	cout << "Run started: " << output.toStdString() << endl;
	
	// begin calculation!
	mix->run();
	
	return 0;
}

void printUsage()
{
	cout << "Chemecher 4.0.0, developed for the University of North Florida" << endl;
	cout << "Usage: chemecher [--help] [--non-interactive] <input filename> <output filename>" << endl;
}

