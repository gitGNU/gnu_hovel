/****************************************************************************

Copyright (C) 2010 Jonathan Crowe.

This file is part of Hovel.

Hovel is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Hovel is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Hovel.  If not, see <http://www.gnu.org/licenses/>.

****************************************************************************/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QMdiArea>
#include <QToolButton>

#include "hovelmodel.h"
#include "projecttreeview.h"
#include "propertiesview.h"
#include "propertiesproxymodel.h"

namespace Hovel
{

	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		MainWindow();

	private:
		void readSettings();
		void writeSettings();
		void createDockWidgets();
		void createActions();
		void createMenus();
		void createToolBars();
		QMdiSubWindow * sceneIsOpen(const QModelIndex&);

		void closeEvent(QCloseEvent *event);

	private slots:
		void newProject();
		void newBook();
		void newChapter();
		void newScene();
		void openProject();
		void openScene(const QModelIndex &);
		void saveProject();
		bool saveProjectAs();
		void toggleProjectDock();
		void textEditContentsChanged( QPersistentModelIndex&, QString& );

	private:
		QString fileName;
		HovelModel *projectModel;
		ProjectTreeView *projectTreeView;

		QMdiArea *mdiArea;
		QDockWidget *projectDockWidget;
		QDockWidget *propertiesDockWidget;
		PropertiesView *propertiesView;
		PropertiesProxyModel *propertiesProxyModel;


		QToolBar *mainToolBar;
		QToolButton *projectToolButton;
		QToolButton *addToolButton;

		QMenu *projectMenu;
		QMenu *addMenu;

		QAction *newProjectAction;
		QAction *newBookAction;
		QAction *newChapterAction;
		QAction *newSceneAction;
		QAction *openProjectAction;
		QAction *saveProjectAction;
		QAction *saveProjectAsAction;
		QAction *exitAction;

	};

}

#endif // MAINWINDOW_H
