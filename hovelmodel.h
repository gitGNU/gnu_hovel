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


#ifndef HOVELMODEL_H
#define HOVELMODEL_H

#include <QAbstractItemModel>

#include "projectitem.h"

namespace Hovel
{

	/*!
	  A HovelModel represents a Hovel project.
	 */

	class HovelModel : public QAbstractItemModel
	{
	public:
		HovelModel(QObject *parent = 0);

		void newProject();

		QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
		QModelIndex parent(const QModelIndex &child) const;
		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;
		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role) const;
		bool hasChildren ( const QModelIndex & parent = QModelIndex() ) const;
		Qt::ItemFlags flags(const QModelIndex &index) const;
		bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
		QMimeData * mimeData ( const QModelIndexList & indexes ) const;
		QStringList mimeTypes() const;
		Qt::DropActions supportedDropActions () const;
		bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex ( ) );
		bool insertRows ( int row, int count, const QModelIndex & parent = QModelIndex ( ) );
		bool dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent );
		void insertTree ( const HovelItem * rootItem, const QModelIndex & parent, int row, int column );

		bool newBook(int row);
		bool newChapter(QModelIndex parentBook, int row);
		bool newScene(QModelIndex parentChapter, int row);
		bool deleteNode ( QModelIndex node );
		int lastBook();
		int bookCount();
		QModelIndex currentBook(QModelIndexList selectedItems);
		QModelIndex currentChapter(QModelIndexList selectedItems);

		bool isModified();

		bool open ( QString fileName );
		bool save ( QString fileName );
		QString toString ( );

		ProjectItem * projectItem() const;

	private:
		bool insertItem(HovelItem* newItem, const QModelIndex& parent = QModelIndex(), int row = 0);

	private:
		ProjectItem *_rootItem;
	};

}

#endif // HOVELMODEL_H
