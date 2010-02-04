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
<<<<<<< HEAD:projecttreeview.cpp
#include "bookitem.h"
#include "chapteritem.h"
#include "textitem.h"

#include "hovelitemmimedata.h"
=======
>>>>>>> 642d1bf96169a9fc7df6be4cdb5fa328292da2fe:projecttreeview.cpp

#include <QKeyEvent>

namespace Hovel
{

	ProjectTreeView::ProjectTreeView()
	{
		setHeaderHidden(true);
		setSelectionMode(QAbstractItemView::SingleSelection);
		setEditTriggers(EditKeyPressed|SelectedClicked);
		setDragEnabled(true);
		setAcceptDrops(true);
		setDropIndicatorShown(true);
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

<<<<<<< HEAD:projecttreeview.cpp
	void ProjectTreeView::dragMoveEvent ( QDragMoveEvent * event )
	{
		HovelItemMimeData * mimeData = (HovelItemMimeData *)event->mimeData();
		ChapterItem * draggedChapterItem = dynamic_cast<ChapterItem *>(mimeData->items()[0]);
		TextItem * draggedTextItem = dynamic_cast<TextItem *>(mimeData->items()[0]);
		QModelIndex currentDropTargetIndex = indexAt(event->pos());
		QAbstractItemView::DropIndicatorPosition dip;
		if (currentDropTargetIndex.isValid())
			dip = dropIndicatorPosition(event->pos(), visualRect(currentDropTargetIndex));
		HovelItem * dropTargetItem = static_cast<HovelItem *>(currentDropTargetIndex.internalPointer());
		BookItem * dropTargetBookItem = dynamic_cast<BookItem *>(dropTargetItem);
		ChapterItem * dropTargetChapterItem = dynamic_cast<ChapterItem *>(dropTargetItem);
		TextItem * dropTargetTextItem = dynamic_cast<TextItem *>(dropTargetItem);
		bool canDrop = false;

		if ( draggedTextItem ) {
			if ( dropTargetChapterItem )
				canDrop = true;
			if ( dropTargetTextItem && ( dip == AboveItem || dip == BelowItem ) )
				canDrop = true;
		}
		else if ( draggedChapterItem ) {
			if ( dropTargetBookItem )
				canDrop = true;
			if ( dropTargetChapterItem && ( dip == AboveItem || dip == BelowItem ) )
				canDrop = true;
		}

		if ( canDrop )
			event->acceptProposedAction();
		else
			event->ignore();
	}

	QAbstractItemView::DropIndicatorPosition ProjectTreeView::dropIndicatorPosition (const QPoint &pos, const QRect &rect) const
	{
		DropIndicatorPosition dip = QAbstractItemView::OnViewport;
		const int margin = 2;

		if (pos.y() - rect.top() < margin) dip = QAbstractItemView::AboveItem;
		else if (rect.bottom() - pos.y() < margin) dip = QAbstractItemView::BelowItem;
		else if (rect.contains(pos, true)) dip = QAbstractItemView::OnItem;

		return dip;
	}

=======
>>>>>>> 642d1bf96169a9fc7df6be4cdb5fa328292da2fe:projecttreeview.cpp
}
