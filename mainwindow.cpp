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


#include "mainwindow.h"
#include "bookitem.h"
#include "chapteritem.h"
#include "textitem.h"
#include "textedit.h"
#include "hovelitem.h"

#include <QSettings>
#include <QtGui>

namespace Hovel
{

	/*!
	  Creates a MainWindow.
	 */
	MainWindow::MainWindow()
	{
		projectModel = new HovelModel();

		mdiArea = new QMdiArea(this);
		setCentralWidget(mdiArea);

		createActions();
		createMenus();
		createToolBars();
		createDockWidgets();

		readSettings();
	}

	/*!
	  Creates the DockWidgets needed for the MainWindow GUI.
	 */
	void MainWindow::createDockWidgets()
	{
		//Create the project tree view dock widget
		projectDockWidget = new QDockWidget(tr("Project"), this);
		projectDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		addDockWidget(Qt::LeftDockWidgetArea, projectDockWidget);

		projectTreeView = new ProjectTreeView();
		projectTreeView->setModel(projectModel);
		connect(projectTreeView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(openScene(const QModelIndex&)));
		projectDockWidget->setWidget(projectTreeView);

		//Create the properties dock widget
		propertiesDockWidget = new QDockWidget(tr("Properties"), this);
		propertiesDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		addDockWidget(Qt::RightDockWidgetArea, propertiesDockWidget);

		propertiesProxyModel = new PropertiesProxyModel();
		propertiesProxyModel->setSourceModel(projectModel);
		propertiesView = new PropertiesView();
		propertiesView->setModel(propertiesProxyModel);
		propertiesDockWidget->setWidget(propertiesView);
	}

	/*!
	  Creates the QActions needed for the MainWindow.
	 */
	void MainWindow::createActions()
	{
		//New items actions
		newBookAction = new QAction(tr("&New book"), this);
		newBookAction->setShortcut(tr("Ctrl+N,B"));
		connect(newBookAction, SIGNAL(triggered()), this, SLOT(newBook()));

		newChapterAction = new QAction(tr("New chapter"), this);
		newChapterAction->setShortcut(tr("Ctrl+N,C"));
		connect(newChapterAction, SIGNAL(triggered()), this, SLOT(newChapter()));

		newSceneAction = new QAction(tr("New scene"), this);
		newSceneAction->setShortcut(tr("Ctrl+N,S"));
		connect(newSceneAction, SIGNAL(triggered()), this, SLOT(newScene()));

		newProjectAction = new QAction(tr("&New project"), this);
		newProjectAction->setShortcut(tr("Ctrl+N,P"));
		connect(newProjectAction, SIGNAL(triggered()), this, SLOT(newProject()));

		openProjectAction = new QAction(tr("&Open project"), this);
		openProjectAction->setShortcut(tr("Ctrl+O"));
		openProjectAction->setStatusTip(tr("Open an existing project"));
		openProjectAction->setToolTip(tr("Open an existing project"));
		connect(openProjectAction, SIGNAL(triggered()), this, SLOT(openProject()));

		saveProjectAction = new QAction(tr("&Save project"), this);
		saveProjectAction->setShortcut(tr("Ctrl+S"));
		connect(saveProjectAction, SIGNAL(triggered()), this, SLOT(saveProject()));

		saveProjectAsAction = new QAction(tr("Save project &as"), this);
		connect(saveProjectAsAction, SIGNAL(triggered()), this, SLOT(saveProjectAs()));

		exitAction = new QAction(tr("E&xit"), this);
		exitAction->setShortcut(tr("Ctrl+Q"));
		connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
	}

	/*!
	  Creates the Menus needed for the MainWindow GUI.
	 */
	void MainWindow::createMenus()
	{
		projectMenu = new QMenu(tr("Project"));
		projectMenu->addAction(newProjectAction);
		projectMenu->addAction(openProjectAction);
		projectMenu->addSeparator();
		projectMenu->addAction(saveProjectAction);
		projectMenu->addAction(saveProjectAsAction);
		projectMenu->addSeparator();
		projectMenu->addAction(exitAction);

		addMenu = new QMenu(tr("Add"));
		addMenu->addAction(newBookAction);
		addMenu->addAction(newChapterAction);
		addMenu->addAction(newSceneAction);
	}

	/*!
	  Creates the ToolBars needed for the MainWindow GUI.
	 */
	void MainWindow::createToolBars()
	{
		mainToolBar = addToolBar(tr("Main"));
		mainToolBar->setIconSize(QSize(32, 32));

		projectToolButton = new QToolButton();
		projectToolButton->setIcon(QIcon(tr(":/images/tree")));
		projectToolButton->setMenu(projectMenu);
		projectToolButton->setPopupMode(QToolButton::MenuButtonPopup);
		connect(projectToolButton, SIGNAL(clicked()), this, SLOT(toggleProjectDock()));
		mainToolBar->addWidget(projectToolButton);

		addToolButton = new QToolButton();
		addToolButton->setIcon(QIcon(tr(":/images/add")));
		addToolButton->setMenu(addMenu);
		addToolButton->setPopupMode(QToolButton::InstantPopup);
		mainToolBar->addWidget(addToolButton);

	}

	QMdiSubWindow * MainWindow::sceneIsOpen(const QModelIndex& index)
	{
		foreach(QMdiSubWindow* win, mdiArea->subWindowList()) {
			TextEdit *te = (TextEdit*)win->widget();
			if(!te) continue;
			if(index == te->index())
				return win;
		}

		return 0;
	}

	/*!
	  Read persistent application settings from storage.
	  This enables the application to replicate its' previous state on start up.
	 */
	void MainWindow::readSettings()
	{
		QSettings settings("AppliedEschatology", "Hovel");

		settings.beginGroup("MainWindow");
		if(settings.value("maximised", QVariant(false)).toBool())
			setWindowState(windowState() ^ Qt::WindowMaximized);
		else
		{
			resize(settings.value("size", QSize(400, 400)).toSize());
			move(settings.value("position", QPoint(200, 200)).toPoint());
		}
		settings.endGroup();

		settings.beginGroup("Files");
		fileName = settings.value("lastFile", QVariant()).toString();
		if(fileName.length() > 0)
			projectModel->open(fileName);
		else {
			projectModel->newProject();
			fileName.clear();
		}
		settings.endGroup();
	}

	/*!
	  Write persistent application settings to storage.
	  This enables the application to replicate its' current state on next start up.
	 */
	void MainWindow::writeSettings()
	{
		QSettings settings("AppliedEschatology", "Hovel");

		settings.beginGroup("MainWindow");
		settings.setValue("size", size());
		settings.setValue("position", pos());
		settings.setValue("maximised", isMaximized());
		settings.endGroup();

		settings.beginGroup("Files");
		settings.setValue("lastFile", fileName);
		settings.endGroup();
	}

	/*!
	  Runs when the application closes.
	 */
	void MainWindow::closeEvent(QCloseEvent *event)
	{
		writeSettings();
		event->accept();
	}

	/*!
	  Starts a new Hovel project.
	  This will prompt the user to save the current project if it is modified.
	 */
	void MainWindow::newProject()
	{
		//Prompt user to save if current project is modified.
		if(projectModel->isModified()) {
			QMessageBox prompt;
			prompt.setText("The project has been modified.");
			prompt.setInformativeText("Do you want to save your changes?");
			prompt.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
			prompt.setDefaultButton(QMessageBox::Save);
			switch(prompt.exec()) {
				case QMessageBox::Save:
					saveProject();
					break;
				case QMessageBox::Discard:
					break;
				case QMessageBox::Cancel:
					return;
					break;
			}
		}

		delete projectModel;
		projectModel = new HovelModel();
		projectTreeView->setModel(projectModel);
	}

	/*!
	  Creates a new book in the project.
	  This will be added after the last book.
	 */
	void MainWindow::newBook()
	{
		int lastBookRow = projectModel->lastBook();
		projectModel->newBook(lastBookRow+1);
	}

	/*!
	  Add a new chapter to a book. If there is only one book, the chapter will be added to this.
	  Otherwise, the chapter will be added to the book in which the current selection resides.
	  In any other case, no chapter will be added.
	 */
	void MainWindow::newChapter()
	{
		QModelIndex currentBookIndex = projectModel->currentBook(projectTreeView->selectionModel()->selectedIndexes());
		if(!currentBookIndex.isValid()) return;

		projectModel->newChapter(currentBookIndex, projectModel->rowCount(currentBookIndex));
	}

	/*!
	  Add a new scene to a chapter. The new scene will be added to either the currently selected
	  chapter, or to the parent chapter if a scene is selected.
	 */
	void MainWindow::newScene()
	{
		QModelIndex currentChapterIndex = projectModel->currentChapter(projectTreeView->selectionModel()->selectedIndexes());
		if(!currentChapterIndex.isValid()) return;

		projectModel->newScene(currentChapterIndex, projectModel->rowCount(currentChapterIndex));
	}

	/*!
	  Open a Hovel project. If the current project is modified, the user will be prompted to
	  save the current project first.
	  \todo Implement this function
	 */
	void MainWindow::openProject()
	{
	}

	/*!
	  Open a scene for editing.
	 */
	void MainWindow::openScene(const QModelIndex & index)
	{
		QMdiSubWindow * sw = sceneIsOpen(index);
		if(sw) {
			mdiArea->setActiveSubWindow(sw);
			return;
		}

		HovelItem *childItem = static_cast<HovelItem*>(index.internalPointer());
		TextItem *textItem = dynamic_cast<TextItem *>(childItem);
		if(!textItem) return;

		TextEdit *textEdit = new TextEdit(index, this, textItem->data(HovelItem::TextRole).toString());
		connect(textEdit, SIGNAL(contentChanged(QPersistentModelIndex&,QString&)), this, SLOT(textEditContentsChanged(QPersistentModelIndex&,QString&)));
		mdiArea->addSubWindow(textEdit);
		textEdit->showMaximized();
	}

	/*!
	  Save the current project to disk, prompting for a file name if neccessary.
	 */
	void MainWindow::saveProject()
	{
		if ( fileName.isEmpty ( ) )
			if ( !saveProjectAs ( ) ) return;

		projectModel->save ( fileName );
	}

	/*!
	  Save the current project, allowing the user to specify a file name.
	 */
	bool MainWindow::saveProjectAs()
	{
		QFileDialog dialog ( this );
		dialog.setFileMode ( QFileDialog::AnyFile );
		dialog.setAcceptMode ( QFileDialog::AcceptSave );
		dialog.setNameFilter ( tr ( "Hovel Project Files (*.xml)" ) );
		if ( dialog.exec ( ) ) {
			fileName = dialog.selectedFiles ( ).first ( );
			return true;
		}

		return false;
	}

	/*!
	  Toggle the visibilty of the project dock window.
	 */
	void MainWindow::toggleProjectDock()
	{
		if(projectDockWidget->isVisible())
			projectDockWidget->hide();
		else
			projectDockWidget->show();
	}

	void MainWindow::textEditContentsChanged( QPersistentModelIndex& index, QString& newText)
	{
		HovelItem *item = static_cast<HovelItem*>(index.internalPointer());
		TextItem *textItem = dynamic_cast<TextItem *>(item);
		if(!textItem) return;

		item->setData(newText, HovelItem::TextRole);
	}

}
