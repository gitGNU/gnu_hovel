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
#include <QTextCharFormat>

#include "hovelmodel.h"
#include "projecttreeview.h"
#include "propertiesview.h"
#include "propertiesproxymodel.h"
#include "formattingtoolbar.h"

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
		QMdiSubWindow * itemIsOpen(const QModelIndex&);

		void closeEvent(QCloseEvent *event);

	private slots:
		void newProject();
		void newBook();
		void newChapter();
		void newScene();
		void newCharacter();
		void openProject();
		void openItem(const QModelIndex &);
		void openScene(const QModelIndex &);
		void openCharacter(const QModelIndex &);
		void saveProject();
		bool saveProjectAs();
		void toggleProjectDock();
		void togglePropertiesDock();
		void textEditContentsChanged( QPersistentModelIndex&, QString& );
		void exportHtmlFile();
		void toggleFullScreen();
		void deleteProjectNode();
		void textBold ( bool checked );
		void textItalic ( bool checked );
		void textUnderline ( bool checked );
		void textStrikeThrough ( bool checked );
		void fontFamilyChanged ( const QString& );
		void fontSizeChanged ( const QString& );
		void currentCharFormatChanged ( const QTextCharFormat& format );
		void textEditClosing();

	protected:
		void keyPressEvent ( QKeyEvent * event );

	private:
		void mergeFormat(const QTextCharFormat &format);

		QString _fileName;
		HovelModel *_projectModel;
		ProjectTreeView *_projectTreeView;

		QMdiArea *_mdiArea;
		QDockWidget *_projectDockWidget;
		QDockWidget *_propertiesDockWidget;
		PropertiesView *_propertiesView;
		PropertiesProxyModel *_propertiesProxyModel;


		QToolBar *_mainToolBar;
		QToolButton *_projectToolButton;
		QToolButton *_addToolButton;
		QToolButton *_fullScreenToolButton;
		QToolButton *_propertiesToolButton;
		FormattingToolBar *_formattingToolBar;

		QMenu *_projectMenu;
		QMenu *_nodeMenu;
		QMenu *_exportMenu;

		QAction *_newProjectAction;
		QAction *_newBookAction;
		QAction *_newChapterAction;
		QAction *_newSceneAction;
		QAction *_newCharacterAction;
		QAction *_openProjectAction;
		QAction *_saveProjectAction;
		QAction *_saveProjectAsAction;
		QAction *_exitAction;
		QAction *_exportHtmlFileAction;
		QAction *_deleteProjectNodeAction;

		bool _fullScreen;

		QByteArray _windowState;
		QByteArray _windowGeometry;
	};

}

#endif // MAINWINDOW_H
