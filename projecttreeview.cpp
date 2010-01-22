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


#include "projecttreeview.h"
#include "hovelitem.h"

#include <QKeyEvent>

namespace Hovel
{

	ProjectTreeView::ProjectTreeView()
	{
		setHeaderHidden(true);
		setSelectionMode(QAbstractItemView::SingleSelection);
		setEditTriggers(EditKeyPressed|SelectedClicked);
	}

	void ProjectTreeView::keyPressEvent ( QKeyEvent * event )
	{
		if((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) && state() != QAbstractItemView::EditingState)
			emit doubleClicked(this->currentIndex());

		QAbstractItemView::keyPressEvent(event);
	}

	void ProjectTreeView::selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected )
	{
		QModelIndex i = selectedIndexes()[0];
		emit newItemSelected ( selected, deselected );
	}

}
