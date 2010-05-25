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


#include "hovelmodel.h"
#include "hovelitem.h"
#include "bookitem.h"
#include "textitem.h"
#include "folderitem.h"
#include "chapteritem.h"
#include "hovelitemmimedata.h"

#include <QFile>
#include <QTextStream>
#include <QDomDocument>

namespace Hovel
{

	HovelModel::HovelModel(QObject *parent)
		: QAbstractItemModel(parent)
	{
		_rootItem = 0;
		newProject();
	}

	/*!
	  Creates a new, empty project, with a basic project skeleton.
	 */
	void HovelModel::newProject()
	{
		delete _rootItem;
		_rootItem = new ProjectItem();
		QModelIndex rootItemIndex = index(0, 0, QModelIndex());
		BookItem *bi = new BookItem(_rootItem, "New book");
		ChapterItem *chapter = new ChapterItem(bi, "New chapter");
		FolderItem *characters = new FolderItem(_rootItem, "Characters");
		characters->setCanModify( false );
		FolderItem *locations = new FolderItem(_rootItem, "Locations");
		locations->setCanModify( false );
		TextItem *scene = new TextItem(chapter, "New scene", "This is the text.");

		insertItem(bi, rootItemIndex, 0);
		QModelIndex bookItemIndex = index(0, 0, rootItemIndex);
		insertItem(chapter, bookItemIndex, 0);
		QModelIndex chapterItemIndex = index(0, 0, bookItemIndex);
		insertItem(scene, chapterItemIndex, 0);
		insertItem(characters, rootItemIndex, 1);
		insertItem(locations, rootItemIndex, 2);
	}

	/*!
	  Returns a QModelIndex of an item, specified by the \a row and \a column of \a parent.
	 */
	QModelIndex HovelModel::index ( int row, int column, const QModelIndex & parent ) const
	{
		if (!hasIndex(row, column, parent))
			return QModelIndex();

		HovelItem *parentItem;

		if (!parent.isValid())
			parentItem = _rootItem;
		else
			parentItem = static_cast<HovelItem*>(parent.internalPointer());

		HovelItem *childItem = parentItem->child(row);
		if (childItem)
			return createIndex(row, column, childItem);
		else
			return QModelIndex();
	}

	/*!
	  Returns the QModelIndex of the parent of the item represented by \a index.
	 */
	QModelIndex HovelModel::parent(const QModelIndex &index) const
	{
		if (!index.isValid())
			return QModelIndex();

		HovelItem *childItem = static_cast<HovelItem*>(index.internalPointer());
		HovelItem *parentItem = childItem->parent();

		if (parentItem == _rootItem)
			return QModelIndex();

		return createIndex(parentItem->row(), 0, parentItem);
	}

	int HovelModel::rowCount(const QModelIndex &parent) const
	{
		HovelItem *parentItem;
		if (parent.column() > 0)
			return 0;

		if (!parent.isValid())
			parentItem = _rootItem;
		else
			parentItem = static_cast<HovelItem*>(parent.internalPointer());

		return parentItem->childCount();
	}

	int HovelModel::columnCount(const QModelIndex &parent) const
	{
		if (parent.isValid())
			return static_cast<HovelItem*>(parent.internalPointer())->columnCount();
		else
			return _rootItem->columnCount();
	}

	QVariant HovelModel::data(const QModelIndex &index, int role) const
	{
		if (!index.isValid())
			return QVariant();

		HovelItem *item = static_cast<HovelItem*>(index.internalPointer());

		switch(role) {
		case Qt::EditRole:
			return item->data(TitleRole);
		case Qt::DecorationRole:
		case TitleRole:
			return item->data(role);
		default:
			return QVariant();
		}
	}

	QVariant HovelModel::headerData(int section, Qt::Orientation orientation, int role) const
	{
		if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
			return _rootItem->data((Qt::ItemDataRole)section);

		return QVariant();
	}

	bool HovelModel::hasChildren (const QModelIndex & parent) const
	{
		HovelItem *parentItem;

		if (!parent.isValid())
			parentItem = _rootItem;
		else
			parentItem = static_cast<HovelItem*>(parent.internalPointer());

		if(parentItem->childCount() > 0)
			return true;
		return false;
	}

	Qt::ItemFlags HovelModel::flags(const QModelIndex &index) const
	{
		if (!index.isValid())
			return Qt::ItemIsEnabled;

		Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

		HovelItem* item = static_cast<HovelItem*>(index.internalPointer());

		if( item->canModify() )
			flags = flags | Qt::ItemIsEditable;

		if ( dynamic_cast<BookItem *>(item) || dynamic_cast<ChapterItem *>(item) )
			flags = flags | Qt::ItemIsDropEnabled;

		if ( dynamic_cast<ChapterItem *>(item) || dynamic_cast<TextItem *>(item) )
			flags = flags | Qt::ItemIsDragEnabled;

		return flags;
	}

	bool HovelModel::setData(const QModelIndex &index, const QVariant &value, int role)
	{
		if (!index.isValid()) return false;

		HovelItem *item = static_cast<HovelItem*>(index.internalPointer());

		item->setData(value, role);

		emit dataChanged(index, index);
		return true;
	}

	/*!
	  Returns encoded mime data for the given model indexes.
	 */
	QMimeData * HovelModel::mimeData ( const QModelIndexList & indexes ) const
	{
		HovelItemMimeData *mimeData = new HovelItemMimeData();

		foreach (QModelIndex index, indexes) {
			if (index.isValid()) {
				HovelItem *item = static_cast<HovelItem*>(index.internalPointer ( ) );
				mimeData->appendItem(item);
			}
		}

		return mimeData;
	}

	/*!
	  Defines the mime types that this model supports.
	 */
	QStringList HovelModel::mimeTypes() const
	{
		QStringList types;
		types << "application/hovelitems";
		return types;
	}

	/*!
	  Defines the drop actions that this model supports.
	 */
	Qt::DropActions HovelModel::supportedDropActions () const
	{
		return Qt::MoveAction;
	}

	/*!
	  Occurs when mime data is dropeed on to the view representing the model.
	 */
	bool HovelModel::dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent )
	{
		if ( action != Qt::MoveAction ) return false;
		if ( !data->hasFormat("application/hovelitems") ) return false;

		//In the event that an item is dropped directly on it's parent item.
		if ( row == -1 && parent.isValid()) {
			HovelItem * parentItem = static_cast<HovelItem*>(parent.internalPointer());
			row = parentItem->childCount();
		}

		const HovelItemMimeData * hovelItemMimeData = dynamic_cast<const HovelItemMimeData *>( data );
		foreach ( HovelItem * item, hovelItemMimeData->items() ) {
			insertTree(item, parent, row, column);
		}

		return true;
	}

	/*!
	  Insert a hierarchy of HovelItems into the model.
	 */
	void HovelModel::insertTree ( const HovelItem * rootItem, const QModelIndex & parent, int row, int column )
	{
		insertRows ( row, 1, parent );
		QModelIndex newRootIndex = parent.child(row, 0);
		HovelItem * newRootItem = static_cast<HovelItem *>(newRootIndex.internalPointer());

		for ( int i=0; i < LastRole; ++i)
			if( rootItem->data(i) != QVariant() )
				newRootItem->setData( rootItem->data(i), i );

		int childRow = 0;
		foreach(HovelItem * item, rootItem->childItems())
			insertTree ( item, newRootIndex, childRow++, 0 );	//Look out, recursion.
	}

	/*!
	  Remove multiple rows from the model.
	 */
	bool HovelModel::removeRows ( int row, int count, const QModelIndex & parent )
	{
		beginRemoveRows ( parent, row, row + count - 1 );

		HovelItem * parentItem = static_cast<HovelItem*>(parent.internalPointer ( ) );
		//If parent item is not valid, then this is a top level item in the project.
		if ( !parentItem )
			parentItem = _rootItem;

		for ( int i = row; i < ( row + count ); i++ )
			parentItem->removeChildAt ( i );

		endRemoveRows ( );
		return true;
	}

	/*!
	  Insert multiple rows in to the model. These will need to be populated with data.
	 */
	bool HovelModel::insertRows ( int row, int count, const QModelIndex & parent )
	{
		beginInsertRows ( parent, row, row + count - 1 );

		HovelItem * parentItem = static_cast<HovelItem*>(parent.internalPointer ( ) );
		for ( int i = row; i < ( row + count ); i++ ) {
			HovelItem * newItem;
			if ( dynamic_cast<BookItem *>(parentItem) )
				newItem = new ChapterItem ( parentItem, "" );
			else if ( dynamic_cast<ChapterItem *>(parentItem) )
				newItem = new TextItem ( parentItem, "", "" );
			else return false;
			parentItem->insertChild ( newItem, i );
		}

		endInsertRows ( );
		return true;
	}

	/*!
	  Inserts a new BookItem into the project at \a row.
	 */
	bool HovelModel::newBook(int row)
	{
		return insertItem(new BookItem(_rootItem, "New book"), QModelIndex(), row);
	}

	/*!
	  Adds a new ChapterItem to \a parentBook at \a row.
	 */
	bool HovelModel::newChapter(QModelIndex parentBook, int row)
	{
		BookItem *currentBook = static_cast<BookItem*>(parentBook.internalPointer());
		return insertItem(new ChapterItem(currentBook, "New chapter"), parentBook, row);
	}

	/*!
	  Adds a new TextItem to \a parentChapter at \a row>.
	 */
	bool HovelModel::newScene(QModelIndex parentChapter, int row)
	{
		ChapterItem *currentChapter = static_cast<ChapterItem*>(parentChapter.internalPointer());
		return insertItem(new TextItem(currentChapter, "New scene"), parentChapter, row);
	}

	/*!
	  Remove a node from the project.
	 */
	bool HovelModel::deleteNode ( QModelIndex node )
	{
		return removeRow ( node.row(), node.parent() );
	}

	/*!
	  Returns the row number of the last book in the project.
	 */
	int HovelModel::lastBook()
	{
		int lastBookRow = -1;

		foreach(HovelItem* item, _rootItem->children()) {
			if(dynamic_cast<BookItem*>(item))
				lastBookRow = item->row();
		}

		return lastBookRow;
	}

	/*!
	  Returns the \c QModelIndex of the current book.
	  If there are no books in the project, an invalid \c QModelIndex() is returned.
	  If there is one book in the project, the index of that book is returned.
	  If there is more than one book, the index of the currently selected book, or the parent
	  book of the currently selected item, is returned. If the selection lies outside a book or
	  its' children, an invalid \c QModelIndex is returned.
	 */
	QModelIndex HovelModel::currentBook(QModelIndexList selectedItems)
	{
		QModelIndex selectedIndex;
		if(selectedItems.count())
			selectedIndex = selectedItems.value(0);
		else
			selectedIndex = QModelIndex();

		switch (bookCount()) {
			case 0:
				return QModelIndex();
			case 1:
				BookItem* bookItem;
				foreach(HovelItem* item, _rootItem->children()) {
					if ((bookItem = dynamic_cast<BookItem*>(item)))
						return createIndex(bookItem->row(), 0, bookItem);
				}
			default:
				if (!selectedIndex.isValid())
					return QModelIndex();
				HovelItem *currentItem = static_cast<HovelItem*>(selectedIndex.internalPointer());
				while (currentItem != _rootItem) {
					if ((dynamic_cast<BookItem*>(currentItem)))
						return createIndex(currentItem->row(), 0, currentItem);
					currentItem = currentItem->parent();
				}

		}

		return QModelIndex();
	}

	/*!
	  Returns the QModelIndex of the current chapter.
	  This is either the currently selected book, or the parent chapter of the currently
	  selected scene.
	  If no chapter or scene is selected an invalid QModelIndex is returned.
	 */
	QModelIndex HovelModel::currentChapter(QModelIndexList selectedItems)
	{
		QModelIndex selectedIndex;
		if(selectedItems.count())
			selectedIndex = selectedItems.value(0);
		else
			return QModelIndex();

		HovelItem *currentItem = static_cast<HovelItem*>(selectedIndex.internalPointer());
		if((dynamic_cast<ChapterItem*>(currentItem)))
			return createIndex(currentItem->row(), 0, currentItem);
		else if((dynamic_cast<TextItem*>(currentItem))) {
			ChapterItem *chapterItem = dynamic_cast<ChapterItem*>(currentItem->parent());
			if(chapterItem)
				return createIndex(chapterItem->row(), 0, chapterItem);
		}

		return QModelIndex();
	}

	/*!
	  Returns the number of books in the project.
	 */
	int HovelModel::bookCount()
	{
		int bookCount = 0;
		foreach(HovelItem* item, _rootItem->children()) {
			if(dynamic_cast<BookItem*>(item))
				++bookCount;
		}
		return bookCount;
	}

	bool HovelModel::insertItem(HovelItem* newItem, const QModelIndex& parent, int row)
	{
		beginInsertRows(parent, row, row);

		HovelItem *parentItem = 0;
		if (!parent.isValid())
			parentItem = _rootItem;
		else
			parentItem = static_cast<HovelItem*>(parent.internalPointer());

		parentItem->insertChild(newItem, row);

		endInsertRows();

		return true;
	}

	bool HovelModel::isModified()
	{
		if(_rootItem->isModified()) return true;

		return false;
	}

	bool HovelModel::open ( QString fileName )
	{
		QFile* inFile = new QFile ( fileName );
		if ( !inFile->open ( QIODevice::ReadOnly | QIODevice::Text ) ) return false;

		QDomDocument doc("Project");
		doc.setContent(inFile);
		inFile->close();

		QDomElement projectElement = doc.documentElement();
		if(_rootItem)
			delete _rootItem;

		_rootItem = new ProjectItem();
		_rootItem->fromQDomElement(projectElement);

		reset();

		return true;
	}

	/*! Write a representation of the model to disk.
	\param fileName a string representing the filename and full path of the file tosave to.
	*/
	bool HovelModel::save ( QString fileName )
	{
		//Set project title if necessary
		QFile* outFile = new QFile ( fileName );
		if ( !outFile->open ( QIODevice::WriteOnly | QIODevice::Text ) ) return false;

		QTextStream stream( outFile );

		stream << toString();

		outFile->close();

		delete outFile;

		return true;
	}

	/*! Returns a string containing a representation of the model in xml format.
	 */
	QString HovelModel::toString ( )
	{
		QDomDocument doc("HovelProject");
		doc.appendChild(_rootItem->toQDomElement(doc));
		return doc.toString(4);
	}

	ProjectItem * HovelModel::projectItem() const
	{
		return _rootItem;
	}

}
