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


#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

#include <QTreeView>

namespace Hovel
{

	class ProjectTreeView : public QTreeView
	{
	Q_OBJECT

	public:
		ProjectTreeView();

	protected:
		void keyPressEvent ( QKeyEvent * event );
		void selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected );
<<<<<<< HEAD:projecttreeview.h
		void dragMoveEvent ( QDragMoveEvent * event );
		DropIndicatorPosition dropIndicatorPosition (const QPoint &pos, const QRect &rect) const;
=======
>>>>>>> 642d1bf96169a9fc7df6be4cdb5fa328292da2fe:projecttreeview.h

	signals:
		void newItemSelected ( const QItemSelection & selected, const QItemSelection & deselected );
	};

}

#endif // PROJECTTREEVIEW_H
