#include <QtGui/QApplication>
#include "mainwindow.h"

#include <QFile>
#include <QTextStream>

void loadStyleSheet() {
	QFile data(":/styles/default_style");
	QString style;
	if(data.open(QFile::ReadOnly)) {
		QTextStream styleIn(&data);
		style = styleIn.readAll();
		data.close();
		qApp->setStyleSheet(style);
	}
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	loadStyleSheet();
	Hovel::MainWindow w;
	w.show();

	return a.exec();
}

