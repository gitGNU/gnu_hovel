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
