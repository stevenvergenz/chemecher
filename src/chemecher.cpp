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

	return app.exec();
}
