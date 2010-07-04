#include <QApplication>
#include <QDebug>
#include <iostream>

#include "mainwindow.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	app.setApplicationName("Chemecher 4.0");

	MainWindow ui;
	ui.show();

	//test something
	QString test = "a;sldkfj";
	std::cout << "test val: " << test.toInt() << endl;

	return app.exec();
}
