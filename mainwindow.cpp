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
#include "characteritem.h"
#include "export.h"
#include "utilities.h"

#include <QSettings>
#include <QtGui>

namespace Hovel
{

	/*!
	  Creates a MainWindow.
	 */
	MainWindow::MainWindow()
	{
		_projectModel = new HovelModel();

		_mdiArea = new QMdiArea(this);
		setCentralWidget(_mdiArea);
		_fullScreen = false;
		setObjectName("MainWindow");

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
		_projectDockWidget = new QDockWidget(tr("Project"), this);
		_projectDockWidget->setObjectName("ProjectDock");
		_projectDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		addDockWidget(Qt::LeftDockWidgetArea, _projectDockWidget);

		_projectTreeView = new ProjectTreeView();
		_projectTreeView->setModel(_projectModel);
		connect(_projectTreeView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(openItem(const QModelIndex&)));
		_projectDockWidget->setWidget(_projectTreeView);

		//Create the properties dock widget
		_propertiesDockWidget = new QDockWidget(tr("Properties"), this);
		_propertiesDockWidget->setObjectName("PropertiesDock");
		_propertiesDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		addDockWidget(Qt::RightDockWidgetArea, _propertiesDockWidget);

		_propertiesProxyModel = new PropertiesProxyModel();
		_propertiesProxyModel->setSourceModel(_projectModel);
		connect(_propertiesProxyModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), _projectTreeView, SLOT(currentChanged(QModelIndex,QModelIndex)));
		_propertiesView = new PropertiesView();
		_propertiesView->setModel(_propertiesProxyModel);

		connect(_projectTreeView, SIGNAL(newItemSelected(const QItemSelection&, const QItemSelection&)),
				_propertiesProxyModel, SLOT(selectionChanged(const QItemSelection&, const QItemSelection&)));
		connect(_projectModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), _propertiesView, SLOT(currentChanged(QModelIndex,QModelIndex)));
		connect(_propertiesProxyModel, SIGNAL(layoutChanged()), _propertiesView, SLOT(setUpView()));
		_propertiesDockWidget->setWidget(_propertiesView);
	}

	/*!
	  Creates the QActions needed for the MainWindow.
	 */
	void MainWindow::createActions()
	{
		//New items actions
		_newBookAction = new QAction(tr("&New book"), this);
		_newBookAction->setShortcut(tr("Ctrl+N,B"));
		connect(_newBookAction, SIGNAL(triggered()), this, SLOT(newBook()));

		_newChapterAction = new QAction(tr("New chapter"), this);
		_newChapterAction->setShortcut(tr("Ctrl+N,C"));
		connect(_newChapterAction, SIGNAL(triggered()), this, SLOT(newChapter()));

		_newSceneAction = new QAction(tr("New scene"), this);
		_newSceneAction->setShortcut(tr("Ctrl+N,S"));
		connect(_newSceneAction, SIGNAL(triggered()), this, SLOT(newScene()));

		_newCharacterAction = new QAction(tr("New character"), this);
		_newCharacterAction->setShortcut(tr("Ctrl+N,H"));
		connect(_newCharacterAction, SIGNAL(triggered()), this, SLOT(newCharacter()));

		_newProjectAction = new QAction(tr("&New project"), this);
		_newProjectAction->setShortcut(tr("Ctrl+N,P"));
		connect(_newProjectAction, SIGNAL(triggered()), this, SLOT(newProject()));

		_openProjectAction = new QAction(tr("&Open project"), this);
		_openProjectAction->setShortcut(tr("Ctrl+O"));
		_openProjectAction->setStatusTip(tr("Open an existing project"));
		_openProjectAction->setToolTip(tr("Open an existing project"));
		connect(_openProjectAction, SIGNAL(triggered()), this, SLOT(openProject()));

		_saveProjectAction = new QAction(tr("&Save project"), this);
		_saveProjectAction->setShortcut(tr("Ctrl+S"));
		connect(_saveProjectAction, SIGNAL(triggered()), this, SLOT(saveProject()));

		_saveProjectAsAction = new QAction(tr("Save project &as"), this);
		connect(_saveProjectAsAction, SIGNAL(triggered()), this, SLOT(saveProjectAs()));

		_exportHtmlFileAction = new QAction(tr("to single &Html file"), this);
		connect(_exportHtmlFileAction, SIGNAL(triggered()), this, SLOT(exportHtmlFile()));

		_exitAction = new QAction(tr("E&xit"), this);
		_exitAction->setShortcut(tr("Ctrl+Q"));
		connect(_exitAction, SIGNAL(triggered()), this, SLOT(close()));

		_deleteProjectNodeAction = new QAction ( tr ( "Delete node" ), this );
		_deleteProjectNodeAction->setShortcut ( tr ( "Ctrl+Alt+D" ) );
		connect ( _deleteProjectNodeAction, SIGNAL (triggered() ), this, SLOT ( deleteProjectNode() ) );
	}

	/*!
	  Creates the Menus needed for the MainWindow GUI.
	 */
	void MainWindow::createMenus()
	{
		_exportMenu = new QMenu(tr("Export . . ."));
		_exportMenu->addAction(_exportHtmlFileAction);

		_projectMenu = new QMenu(tr("Project"));
		_projectMenu->addAction(_newProjectAction);
		_projectMenu->addAction(_openProjectAction);
		_projectMenu->addSeparator();
		_projectMenu->addAction(_saveProjectAction);
		_projectMenu->addAction(_saveProjectAsAction);
		_projectMenu->addMenu(_exportMenu);
		_projectMenu->addSeparator();
		_projectMenu->addAction(_exitAction);

		_nodeMenu = new QMenu ( tr ( "Add" ) );
		_nodeMenu->addAction ( _newBookAction );
		_nodeMenu->addAction ( _newChapterAction );
		_nodeMenu->addAction ( _newSceneAction );
		_nodeMenu->addSeparator();
		_nodeMenu->addAction ( _newCharacterAction );
		_nodeMenu->addSeparator();
		_nodeMenu->addAction ( _deleteProjectNodeAction );
	}

	/*!
	  Creates the ToolBars needed for the MainWindow GUI.
	 */
	void MainWindow::createToolBars()
	{
		_mainToolBar = addToolBar(tr("Main"));
		_mainToolBar->setObjectName("MainToolBar");

		QWidget *containingWidget = new QWidget;
		QHBoxLayout *layout = new QHBoxLayout(containingWidget);
		layout->setMargin(0);

		_projectToolButton = new QToolButton();
		_projectToolButton->setIcon(QIcon(tr(":/images/folder32")));
		_projectToolButton->setIconSize(QSize(32, 32));
		_projectToolButton->setMenu(_projectMenu);
		_projectToolButton->setPopupMode(QToolButton::MenuButtonPopup);
		connect(_projectToolButton, SIGNAL(clicked()), this, SLOT(toggleProjectDock()));
		layout->addWidget(_projectToolButton);

		_addToolButton = new QToolButton();
		_addToolButton->setIcon(QIcon(tr(":/images/nodes")));
		_addToolButton->setIconSize(QSize(32, 32));
		_addToolButton->setMenu(_nodeMenu);
		_addToolButton->setPopupMode(QToolButton::InstantPopup);
		layout->addWidget(_addToolButton);

		_fullScreenToolButton = new QToolButton();
		_fullScreenToolButton->setIcon(QIcon(tr(":/images/fullscreen")));
		_fullScreenToolButton->setIconSize(QSize(32, 32));
		_fullScreenToolButton->setCheckable(true);
		connect(_fullScreenToolButton, SIGNAL(clicked()), this, SLOT(toggleFullScreen()));
		layout->addWidget(_fullScreenToolButton);

		_formattingToolBar = new FormattingToolBar();
		layout->addWidget ( _formattingToolBar );
		_formattingToolBar->setEnabled ( false );
		connect ( _formattingToolBar, SIGNAL ( boldButtonToggled ( bool )), this, SLOT ( textBold ( bool )));
		connect ( _formattingToolBar, SIGNAL ( italicButtonToggled ( bool )), this, SLOT ( textItalic ( bool )));
		connect ( _formattingToolBar, SIGNAL ( underlineButtonToggled ( bool )), this, SLOT ( textUnderline ( bool )));
		connect ( _formattingToolBar, SIGNAL ( strikeThroughButtonToggled ( bool )), this, SLOT ( textStrikeThrough ( bool )));
		connect ( _formattingToolBar, SIGNAL ( fontFamilyChanged ( const QString& )), this, SLOT ( fontFamilyChanged ( const QString& )));
		connect ( _formattingToolBar, SIGNAL ( fontSizeChanged ( const QString& )), this, SLOT ( fontSizeChanged ( const QString& )));

		_propertiesToolButton = new QToolButton();
		_propertiesToolButton->setIcon(QIcon(tr(":/images/properties")));
		_propertiesToolButton->setIconSize(QSize(32, 32));
		_propertiesToolButton->setCheckable(true);
		connect(_propertiesToolButton, SIGNAL(clicked()), this, SLOT(togglePropertiesDock()));
		layout->addStretch();
		layout->addWidget(_propertiesToolButton);

		_mainToolBar->addWidget(containingWidget);
	}

	/*!
	  Returns a pointer to an open sub-window associated with \a index, or 0
	  if an associated sub-window is not already open.
	 */
	QMdiSubWindow * MainWindow::itemIsOpen(const QModelIndex& index)
	{
		foreach(QMdiSubWindow* win, _mdiArea->subWindowList()) {
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
		_fileName = settings.value("lastFile", QVariant()).toString();
		if(_fileName.length() > 0)
			_projectModel->open(_fileName);
		else {
			_projectModel->newProject();
			_fileName.clear();
		}
		settings.endGroup();

		settings.beginGroup("Docks");
		settings.value("ProjectDock", QVariant(true)).toBool() ? _projectDockWidget->show() : _projectDockWidget->hide();
		settings.value("PropertiesDock", QVariant(true)).toBool() ? _propertiesDockWidget->show() : _propertiesDockWidget->hide();
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
		settings.setValue("lastFile", _fileName);
		settings.endGroup();

		settings.beginGroup("Docks");
		settings.setValue("ProjectDock", _projectDockWidget->isVisible());
		settings.setValue("PropertiesDock", _propertiesDockWidget->isVisible());
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
		if(_projectModel->isModified()) {
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

		delete _projectModel;
		_projectModel = new HovelModel();
		_propertiesProxyModel->setSourceModel(_projectModel);
		_projectTreeView->setModel(_projectModel);
		_fileName.clear();
	}

	/*!
	  Creates a new book in the project.
	  This will be added after the last book.
	 */
	void MainWindow::newBook()
	{
		int lastBookRow = _projectModel->lastBook();
		_projectModel->newBook(lastBookRow+1);
	}

	/*!
	  Add a new chapter to a book. If there is only one book, the chapter will be added to this.
	  Otherwise, the chapter will be added to the book in which the current selection resides.
	  In any other case, no chapter will be added.
	 */
	void MainWindow::newChapter()
	{
		QModelIndex currentBookIndex = _projectModel->currentBook(_projectTreeView->selectionModel()->selectedIndexes());
		if(!currentBookIndex.isValid()) return;

		_projectModel->newChapter(currentBookIndex, _projectModel->rowCount(currentBookIndex));
	}

	/*!
	  Add a new scene to a chapter. The new scene will be added to either the currently selected
	  chapter, or to the parent chapter if a scene is selected.
	 */
	void MainWindow::newScene()
	{
		QModelIndex currentChapterIndex = _projectModel->currentChapter(_projectTreeView->selectionModel()->selectedIndexes());
		if(!currentChapterIndex.isValid()) return;

		_projectModel->newScene(currentChapterIndex, _projectModel->rowCount(currentChapterIndex));
	}

	/*!
	  Add a new character to a project. The new scene will be added to the characters folder.
	 */
	void MainWindow::newCharacter()
	{
		_projectModel->newCharacter();
	}

	/*!
	  Delete the currently selected node from the project.
	 */
	void MainWindow::deleteProjectNode()
	{
		QModelIndex selectedIndex;
		if( _projectTreeView->selectionModel()->currentIndex().isValid() )
			selectedIndex = _projectTreeView->currentIndex();
		else
			selectedIndex = QModelIndex();

		if( !selectedIndex.isValid() ) return;

		HovelItem *selectedItem = static_cast<HovelItem*>( selectedIndex.internalPointer() );
		if ( !selectedItem->canModify() ) return;

		_projectModel->deleteNode ( selectedIndex );
	}

	/*!
	  Sets up a QTextCharFormat for bold text.
	 */
	void MainWindow::textBold ( bool checked )
	{
		QTextCharFormat fmt;
		fmt.setFontWeight ( checked ? QFont::Bold : QFont::Normal );
		mergeFormat( fmt );
	}

	/*!
	  Sets up a QTextCharFormat for italic text.
	 */
	void MainWindow::textItalic ( bool checked )
	{
		QTextCharFormat fmt;
		fmt.setFontItalic ( checked );
		mergeFormat ( fmt );
	}

	/*!
	  Sets up a QTextCharFormat for underlined text.
	 */
	void MainWindow::textUnderline ( bool checked )
	{
		QTextCharFormat fmt;
		fmt.setFontUnderline ( checked );
		mergeFormat ( fmt );
	}

	/*!
	  Sets up a QTextCharFormat for strike through text.
	 */
	void MainWindow::textStrikeThrough ( bool checked )
	{
		QTextCharFormat fmt;
		fmt.setFontStrikeOut ( checked );
		mergeFormat ( fmt );
	}

	void MainWindow::fontFamilyChanged ( const QString& family )
	{
		QTextCharFormat fmt;
		fmt.setFontFamily ( family );
		mergeFormat ( fmt );
	}

	void MainWindow::fontSizeChanged ( const QString& size )
	{
		QTextCharFormat fmt;
		fmt.setFontPointSize ( size.toInt () );
		mergeFormat ( fmt );
	}

	/*!
	  Applies format to text in current active window.
	 */
	void MainWindow::mergeFormat(const QTextCharFormat &format)
	{
		TextEdit * textEdit = dynamic_cast<TextEdit *> ( _mdiArea->activeSubWindow()->widget() );
		if ( !textEdit ) return;

		QTextCursor cursor = textEdit->textCursor();
		cursor.mergeCharFormat(format);
		textEdit->mergeCurrentCharFormat(format);
	}

	/*!
	  Open a Hovel project. If the current project is modified, the user will be prompted to
	  save the current project first.
	 */
	void MainWindow::openProject()
	{
		QFileDialog dialog ( this );
		dialog.setFileMode ( QFileDialog::ExistingFile );
		dialog.setNameFilter ( tr ( "Hovel Project Files (*.xml)" ) );
		if ( dialog.exec ( ) ) {
			_fileName = dialog.selectedFiles ( ).first ( );
			_projectModel->open ( _fileName );
		}
	}

	/*!
	 Determine the item type associated with \a index, and open the
	 item for editing.
	*/
	void MainWindow::openItem ( const QModelIndex & index )
	{
		HovelItem *childItem = static_cast<HovelItem*> ( index.internalPointer() );
		if ( dynamic_cast<TextItem *> ( childItem ) )
			openScene ( index );
		else if ( dynamic_cast<CharacterItem *> ( childItem ) )
			openCharacter ( index );
	}

	/*!
	  Open a scene for editing.
	 */
	void MainWindow::openScene ( const QModelIndex & index )
	{
		QMdiSubWindow * sw = itemIsOpen ( index );
		if ( sw ) {
			_mdiArea->setActiveSubWindow ( sw );
			return;
		}

		HovelItem *childItem = static_cast<HovelItem*> ( index.internalPointer() );
		TextItem *textItem = dynamic_cast<TextItem *> ( childItem );
		if ( !textItem ) return;

		TextEdit *textEdit = new TextEdit ( index, this, textItem->data ( TextRole ).toString() );
		textEdit->document()->setDocumentMargin(4);
		connect ( textEdit, SIGNAL ( contentChanged ( QPersistentModelIndex&,QString& )), this, SLOT ( textEditContentsChanged(QPersistentModelIndex&,QString& )));
		connect ( textEdit, SIGNAL( currentCharFormatChanged ( const QTextCharFormat& )), this, SLOT ( currentCharFormatChanged ( const QTextCharFormat& )));
		connect ( textEdit, SIGNAL ( closing() ), this, SLOT ( textEditClosing() ));

		sw = _mdiArea->addSubWindow ( textEdit );
		_mdiArea->setActiveSubWindow ( sw );

		textEdit->show();

		_formattingToolBar->setEnabled ( true );
	}

	/*!
	  Open a character for editing.
	 */
	void MainWindow::openCharacter ( const QModelIndex & index )
	{
		QMdiSubWindow * sw = itemIsOpen ( index );
		if ( sw ) {
			_mdiArea->setActiveSubWindow ( sw );
			return;
		}

		HovelItem *childItem = static_cast<HovelItem*> ( index.internalPointer() );
		CharacterItem *characterItem = dynamic_cast<CharacterItem *> ( childItem );
		if ( !characterItem ) return;

		TextEdit *textEdit = new TextEdit ( index, this, characterItem->data ( TextRole ).toString() );
		textEdit->document()->setDocumentMargin(4);
		connect ( textEdit, SIGNAL ( contentChanged ( QPersistentModelIndex&,QString& )), this, SLOT ( textEditContentsChanged(QPersistentModelIndex&,QString& )));
		connect ( textEdit, SIGNAL( currentCharFormatChanged ( const QTextCharFormat& )), this, SLOT ( currentCharFormatChanged ( const QTextCharFormat& )));
		connect ( textEdit, SIGNAL ( closing() ), this, SLOT ( textEditClosing() ));

		sw = _mdiArea->addSubWindow ( textEdit );
		_mdiArea->setActiveSubWindow ( sw );

		textEdit->show();

		_formattingToolBar->setEnabled ( true );
	}

	/*!
	  Keeps the state of the formatting toolbar synchronised with the text
	  under the cursor.
	 */
	void MainWindow::currentCharFormatChanged ( const QTextCharFormat& format )
	{
		//Don't change the formatting toolbar when text is selected.
		TextEdit * te = dynamic_cast<TextEdit *>( _mdiArea->activeSubWindow ()->widget () );
		if ( !te ) return;

		bool cosmeticChange = false;
		if ( te->textCursor ().selectedText ().length () > 0 ) cosmeticChange = true;

		//Now update the formatting toolbar.
		QFont f = format.font ();
		_formattingToolBar->setCheckedBoldButton ( f.bold(), cosmeticChange );
		_formattingToolBar->setCheckedItalicButton ( f.italic (), cosmeticChange );
		_formattingToolBar->setCheckedunderlineButton ( f.underline (), cosmeticChange );
		_formattingToolBar->setCheckedstrikeThroughButton ( f.strikeOut (), cosmeticChange );
		_formattingToolBar->setCurrentFontFamily ( format.fontFamily (), cosmeticChange );
		_formattingToolBar->setCurrentFontSize ( QString ( "%1" ).arg ( format.fontPointSize () ), cosmeticChange );
	}

	/*!
	  Performs and actions necessary when a text edit window is closed.
	 */
	void MainWindow::textEditClosing()
	{
		//If this is the last text edit open, disable the formatting toolbar.
		if ( _mdiArea->subWindowList ().count () == 1 )
			_formattingToolBar->setEnabled ( false );
	}

	/*!
	  Save the current project to disk, prompting for a file name if neccessary.
	 */
	void MainWindow::saveProject()
	{
		if ( _fileName.isEmpty ( ) )
			if ( !saveProjectAs ( ) ) return;

		_projectModel->save ( _fileName );
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
			_fileName = dialog.selectedFiles ( ).first ( );
			saveProject ( );
			return true;
		}

		return false;
	}

	/*!
	  Toggle the visibilty of the project dock window.
	 */
	void MainWindow::toggleProjectDock()
	{
		if(_projectDockWidget->isVisible())
			_projectDockWidget->hide();
		else
			_projectDockWidget->show();
	}

	/*!
	  Toggle the visibilty of the properties dock window.
	 */
	void MainWindow::togglePropertiesDock()
	{
		if(_propertiesDockWidget->isVisible())
			_propertiesDockWidget->hide();
		else
			_propertiesDockWidget->show();
	}

	/*!
	  Updates the HovelItem associated with a TextEdit when the text changes in that TextEdit.
	 */
	void MainWindow::textEditContentsChanged( QPersistentModelIndex& index, QString& newText )
	{
		HovelItem *item = static_cast<HovelItem*>(index.internalPointer());
		if ( !dynamic_cast<TextItem *>(item) && !dynamic_cast<CharacterItem *>(item) )
			return;

		item->setData(newText, TextRole);
	}

	/*!
	  Exports the project to an HTML file.
	 */
	void MainWindow::exportHtmlFile()
	{
		Export exporter(this, _projectModel);
		QModelIndex currentBookIndex = _projectModel->currentBook(_projectTreeView->selectionModel()->selectedIndexes());
		if ( !currentBookIndex.isValid() )
			return;

		BookItem * currentBook = static_cast<BookItem *>(currentBookIndex.internalPointer());
		exporter.toHtmlFile ( currentBook );
	}

	/*!
	  Acts on key presses if necesary.
	 */
	void MainWindow::keyPressEvent(QKeyEvent *event)
	{
		if ( event->key() == Qt::Key_Escape) {
			if ( _fullScreen )
				toggleFullScreen();
			return;
		}

		QWidget::keyPressEvent( event );
	}

	/*!
	  Switches between windowed and full screen modes.
	 */
	void MainWindow::toggleFullScreen()
	{
		if(_fullScreen) {
			showNormal();
			_fullScreen = false;

			TextEdit * activeTextEdit = dynamic_cast<TextEdit *> ( _mdiArea->activeSubWindow()->widget() );
			activeTextEdit->setNormalState();
		}
		else {
			if ( !_mdiArea->activeSubWindow() ) {
				_fullScreenToolButton->setChecked(false);
				return;
			}
			TextEdit *activeTextEdit = dynamic_cast<TextEdit *>(_mdiArea->activeSubWindow()->widget() );
			TextEdit *te = new TextEdit(activeTextEdit->index(), 0, "");
			QTextCursor cursor = activeTextEdit->textCursor();
			cursor.setPosition( activeTextEdit->textCursor().position() );
			connect ( te, SIGNAL(exitFullScreenPressed()), this, SLOT(toggleFullScreen()) );
			te->setDocument( activeTextEdit->document() );
			te->setFullScreenState();
			te->setTextCursor ( cursor );
			_fullScreen = true;
			hide();
		}
	}

}
