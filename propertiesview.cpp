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
#include "povcomboboxitemdelegate.h"
#include "hovelitem.h"
#include "propertiesproxymodel.h"

#include <QHeaderView>

namespace Hovel
{

	PropertiesView::PropertiesView()
		: QTableView ()
	{
		horizontalHeader()->hide();
		setAlternatingRowColors(true);
		setEditTriggers ( DoubleClicked | SelectedClicked | EditKeyPressed );
	}

	void PropertiesView::setUpView()
	{
		dataChanged(QModelIndex(), QModelIndex());
	}

	void PropertiesView::comboboxClosed ( QWidget * comboBox )
	{
		comboBox->close();
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
				connect ( delegate, SIGNAL(closeEditor(QWidget*)), this, SLOT(comboboxClosed(QWidget*)) );
				setItemDelegateForRow(i, delegate);
			}
			else if ( currentRowTitleIndex.data ().toString () == "POV character" ) {
				POVComboBoxItemDelegate *delegate = new POVComboBoxItemDelegate ( this );
				connect ( delegate, SIGNAL ( closeEditor ( QWidget* ) ), this, SLOT ( comboboxClosed ( QWidget* ) ) );
				setItemDelegateForRow(i, delegate);
			}
			else {	//Set the default delegate.
				QStyledItemDelegate *delegate = new QStyledItemDelegate ( this );
				connect ( delegate, SIGNAL ( closeEditor ( QWidget *, QAbstractItemDelegate::EndEditHint ) ),
						  this, SLOT ( finishedEditing ( QWidget *, QAbstractItemDelegate::EndEditHint ) ) );
				setItemDelegateForRow ( i, delegate );
			}
		}
		QTableView::dataChanged( topLeft, bottomRight );

		resizeColumnToContents(0);
		int viewWidth = size().width();
		int titleColumnWidth = columnWidth(0);
		horizontalHeader()->resizeSection(1, viewWidth - titleColumnWidth - 10);
	}

	void PropertiesView::finishedEditing ( QWidget * editor, QAbstractItemDelegate::EndEditHint hint )
	{
		closeEditor ( editor, QAbstractItemDelegate::NoHint );
	}

}
