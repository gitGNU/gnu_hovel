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
		setDragEnabled(true);
		setAcceptDrops(true);
		setDropIndicatorShown(true);
	}

	void PropertiesView::setUpView()
	{
		dataChanged(QModelIndex(), QModelIndex());
	}

	void PropertiesView::statusComboboxClosed ( QWidget * comboBox )
	{
		comboBox->close();
	}

	void PropertiesView::currentChanged ( const QModelIndex & current, const QModelIndex & previous )
	{
		QModelIndex selectedCurrent = current;
		QModelIndex selectedPrevious = previous;
		reset();
		QAbstractItemView::currentChanged(selectedCurrent, selectedPrevious);
	}

	void PropertiesView::dataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight )
	{
		//Set the correct editing delegate if required.
		PropertiesProxyModel *proxyModel = (PropertiesProxyModel*)model();

		for( int i=0; i < proxyModel->rowCount(); ++i) {
			QModelIndex currentIndex = proxyModel->index(i, 1);
			QModelIndex currentRowTitleIndex = proxyModel->index(i, 0);
			if(currentRowTitleIndex.data().toString() == "Status") {
				StatusComboBoxItemDelegate *delegate = new StatusComboBoxItemDelegate(this);
				connect ( delegate, SIGNAL(closeEditor(QWidget*)), this, SLOT(statusComboboxClosed(QWidget*)) );
				setItemDelegateForRow(i, delegate);
			}
			else {	//Set the default delegate.
				QStyledItemDelegate *delegate = new QStyledItemDelegate ( this );
				setItemDelegateForRow ( i, delegate );
			}
		}
		QTableView::dataChanged( topLeft, bottomRight );

		resizeColumnToContents(0);
		int viewWidth = size().width();
		int titleColumnWidth = columnWidth(0);
		horizontalHeader()->resizeSection(1, viewWidth - titleColumnWidth - 10);
	}

}
