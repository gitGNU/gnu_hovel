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


#include "propertiesview.h"
#include "statuscomboboxitemdelegate.h"
#include "hovelitem.h"
#include "propertiesproxymodel.h"

#include <QHeaderView>

namespace Hovel
{

	PropertiesView::PropertiesView()
	{
		horizontalHeader()->hide();
		setAlternatingRowColors(true);
	}

	void PropertiesView::setUpView()
	{
		dataChanged(QModelIndex(), QModelIndex());
	}

	void PropertiesView::currentChanged ( const QModelIndex & current, const QModelIndex & previous )
	{
		reset();
		QAbstractItemView::currentChanged(current, previous);
	}

	/*void PropertiesView::selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected )
	{
		if(selected.indexes().count() == 0) return;

		//Set the correct editing delegate if required.
		PropertiesProxyModel *proxyModel = (PropertiesProxyModel*)model();
		QModelIndex selectedIndex = selected.indexes()[0];
		QModelIndex currentRowTitleIndex = proxyModel->index(selectedIndex.row(), 0, selectedIndex.parent());
		if(currentRowTitleIndex.data().toString() == "Status") {
			StatusComboBoxItemDelegate *delegate = new StatusComboBoxItemDelegate(this);
			setItemDelegateForRow(selectedIndex.row(), delegate);
		}
	}*/

	void PropertiesView::dataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight )
	{
		//Set the correct editing delegate if required.
		PropertiesProxyModel *proxyModel = (PropertiesProxyModel*)model();

		for( int i=0; i < proxyModel->rowCount(); ++i) {
			QModelIndex currentIndex = proxyModel->index(i, 1);
			QModelIndex currentRowTitleIndex = proxyModel->index(i, 0);
			if(currentRowTitleIndex.data().toString() == "Status") {
				StatusComboBoxItemDelegate *delegate = new StatusComboBoxItemDelegate(this);
				setItemDelegateForRow(i, delegate);
			}
		}
		QTableView::dataChanged( topLeft, bottomRight );
	}

}
