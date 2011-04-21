#include <QtGui/QApplication>
#include "mainwindow.h"
#include "utilities.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qApp->setStyleSheet(Hovel::loadStyleSheet(":/styles/default_style"));
	Hovel::MainWindow w;
	w.show();

	return a.exec();
}

