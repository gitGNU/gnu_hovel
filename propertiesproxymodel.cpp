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

#include <QItemSelection>

#include "propertiesproxymodel.h"
#include "hovelitem.h"

namespace Hovel
{

	PropertiesProxyModel::PropertiesProxyModel()
		: QAbstractProxyModel()
	{
	}

	QModelIndex PropertiesProxyModel::index(int row, int column, const QModelIndex &parent) const
	{
		return createIndex(row, column, parent.internalPointer());
	}

	QModelIndex PropertiesProxyModel::parent(const QModelIndex &child) const
	{
		return QModelIndex();
	}

	int PropertiesProxyModel::rowCount(const QModelIndex &parent) const
	{
		if( !_selectedItem.isValid() )
			return 0;
		HovelItem *item = static_cast<HovelItem *>(mapToSource(_selectedItem).internalPointer());
		return item->propertyCount();
	}

	int PropertiesProxyModel::columnCount(const QModelIndex &parent) const
	{
		return 2;
	}

	QModelIndex PropertiesProxyModel::mapToSource(const QModelIndex &proxyIndex) const
	{
		return (sourceModel()&&proxyIndex.isValid())
					? sourceModel()->index(proxyIndex.row(), proxyIndex.column(), proxyIndex.parent())
					: QModelIndex();
	}

	QModelIndex PropertiesProxyModel::mapFromSource(const QModelIndex &proxyIndex) const
	{
		return index(proxyIndex.row(), proxyIndex.column(), proxyIndex.parent());
	}

	QVariant PropertiesProxyModel::data ( const QModelIndex &proxyIndex, int role) const
	{
		if (!_selectedItem.isValid())
			return QVariant();

		HovelItem *item = static_cast<HovelItem *>(mapToSource(_selectedItem).internalPointer());

		if ( role != Qt::DisplayRole )
			return QVariant();

		return item->data(Qt::DisplayRole);
	}

	void PropertiesProxyModel::selectionChanged(const QItemSelection& newSelection, const QItemSelection& previousSelection)
	{
		_selectedItem = newSelection.indexes()[0];
		emit layoutAboutToBeChanged();
		emit layoutChanged();
	}
}
