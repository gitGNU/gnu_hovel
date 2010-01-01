#include <QtGui/QApplication>
#include "mainwindow.h"

#include <QTextEdit>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Hovel::MainWindow w;
	w.show();

	return a.exec();
}

