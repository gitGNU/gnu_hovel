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


#include "propertiesproxymodel.h"

#include <QItemSelection>

namespace Hovel
{

	PropertiesProxyModel::PropertiesProxyModel()
		: QSortFilterProxyModel()
	{
	}

/*	QModelIndex PropertiesProxyModel::index ( int row, int column, const QModelIndex & parent ) const
	{
		return QModelIndex();
	}

	QModelIndex PropertiesProxyModel::parent(const QModelIndex &child) const
	{
		return QModelIndex();
	}*/

	int PropertiesProxyModel::rowCount(const QModelIndex &parent) const
	{
		return 5;
	}

	int PropertiesProxyModel::columnCount(const QModelIndex &parent) const
	{
		return 2;
	}

/*	QModelIndex PropertiesProxyModel::mapToSource ( const QModelIndex & proxyIndex ) const
	{
		return QModelIndex();
	}

	QModelIndex PropertiesProxyModel::mapFromSource ( const QModelIndex & sourceIndex ) const
	{
		return QModelIndex();
	}
*/

	bool PropertiesProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
	{
		QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
		if(index == _selectedIndex)
			return true;
		return false;
	}

	bool PropertiesProxyModel::filterAcceptsColumn(int sourceColumn, const QModelIndex &sourceParent) const
	{
		return true;
	}

	void PropertiesProxyModel::selectionChanged( const QItemSelection & selected, const QItemSelection & deselected )
	{
		_selectedIndex = selected.indexes()[0];
		invalidateFilter();
	}

}
