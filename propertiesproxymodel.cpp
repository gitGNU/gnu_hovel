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
		if (sourceModel() && proxyIndex.isValid())
			return _selectedItem;
		else
			return QModelIndex();
	}

	QModelIndex PropertiesProxyModel::mapFromSource(const QModelIndex &proxyIndex) const
	{
		return index(proxyIndex.row(), proxyIndex.column(), proxyIndex.parent());
	}

	/*!
	  Supplies data from the model to fill the properties view.
	 */
	QVariant PropertiesProxyModel::data ( const QModelIndex &proxyIndex, int role) const
	{
		if (!_selectedItem.isValid())
			return QVariant();

		if ( role != Qt::DisplayRole)
			return QVariant();

		HovelItem *item = static_cast<HovelItem *>(mapToSource(_selectedItem).internalPointer());
		int roleIndex = -1;

		for (int i=0; i<LastRole; ++i) {
			if (item->propertyData(i) != QVariant()) {
				++roleIndex;
				if (proxyIndex.row() == roleIndex) {
					if(proxyIndex.column() == 1) return item->propertyData(i);
					else return item->propertyTitle(i);
				}
			}
		}

		return QVariant();
	}

	Qt::ItemFlags PropertiesProxyModel::flags(const QModelIndex &index) const
	{
		if (!index.isValid())
			return Qt::ItemIsEnabled;

		if( index.column() == 1 )
			return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
		else
			return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	}

	bool PropertiesProxyModel::setData(const QModelIndex &index, const QVariant &value, int role)
	{
		QModelIndex sourceIndex = mapToSource(index);
		if (!sourceIndex.isValid()) return false;

		HovelItem *item = static_cast<HovelItem*>(sourceIndex.internalPointer());

		item->setData(value, role);

		emit dataChanged(sourceIndex, sourceIndex);
		return true;
	}

	void PropertiesProxyModel::selectionChanged(const QItemSelection& newSelection, const QItemSelection& previousSelection)
	{
		_selectedItem = newSelection.indexes()[0];
		HovelItem *item = static_cast<HovelItem *>(mapToSource(_selectedItem).internalPointer());

		//Items that cannont be modified do not need properties displayed.
		if(!item->canModify())
			_selectedItem = QModelIndex();

		emit layoutAboutToBeChanged();
		emit layoutChanged();
	}
}
