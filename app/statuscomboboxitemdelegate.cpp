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

#include "statuscomboboxitemdelegate.h"
#include "hovelitem.h"
#include "propertiesproxymodel.h"
#include "utilities.h"
#include <QComboBox>
#include <QPainter>
#include <QEvent>

namespace Hovel
{

	StatusComboBoxItemDelegate::StatusComboBoxItemDelegate(QObject *parent)
		: QStyledItemDelegate(parent)
	{
	}

	/*!
	  Create and populate the QComboBox used for editing the Status.
	 */
	QWidget *StatusComboBoxItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		QComboBox *editor = new QComboBox(parent);
		connect(editor, SIGNAL(currentIndexChanged(int)), this, SLOT(statusChanged(int)));

		QStringList items;
		for( int i=0; i<=CompleteStatus; ++i )
			items << statusString((Status)i);
		editor->insertItems(0, items);

		return editor;
	}

	/*!
	  Set the initial data shown in the editor to the current value of the item's status.
	 */
	void StatusComboBoxItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
	{
		PropertiesProxyModel *proxyModel = (PropertiesProxyModel*)index.model();
		HovelItem *item = static_cast<HovelItem *>(proxyModel->mapToSource(index).internalPointer());
		Status status = (Status)item->data(StatusRole).toInt();

		QComboBox *comboBox = static_cast<QComboBox*>(editor);
		comboBox->setCurrentIndex(status);
	}

	/*!
	  Set the data in the underlying model.
	 */
	void StatusComboBoxItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
	{
		QComboBox *comboBox = static_cast<QComboBox*>(editor);
		Status status = (Status)comboBox->currentIndex();

		model->setData(index, status, StatusRole);
	}

	/*!
	  Resize the combobox's geometry to fit in the table cell.
	 */
	void StatusComboBoxItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		editor->setGeometry(option.rect);
	}

	/*!
	  Displays the status value in the table cell as a string.
	 */
	void StatusComboBoxItemDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
	{
		PropertiesProxyModel *proxyModel = (PropertiesProxyModel*)index.model();
		QModelIndex titleIndex = proxyModel->index(index.row(), 0, index.parent());

		if(titleIndex.data().toString() == "Status" && index.column()==1) {
			painter->save();

			QRect rect = option.rect;
			//rect.setX(rect.x() + (painter->fontMetrics().width('x') / 2));
			QPen originalPen = painter->pen ();
			painter->setPen ( Qt::NoPen );
			painter->setBrush ( option.palette.mid () );
			QModelIndex sourceIndex = proxyModel->mapToSource(index);
			HovelItem *item = static_cast<HovelItem *>(sourceIndex.internalPointer());

			if ( option.state & QStyle::State_Selected )
				painter->drawRect(rect);
			painter->setPen ( originalPen );
			painter->drawText(rect, Qt::AlignVCenter, statusString((Status)item->data(StatusRole).toInt()));

			painter->restore();
		}
		else QStyledItemDelegate::paint(painter, option, index);
	}

	/*!
	  Makes sure that the newly selected status is committed to the model. For some reason, data is not
	  committed until the focus moves away from the combo box otherwise.
	 */
	void StatusComboBoxItemDelegate::statusChanged(int index)
	{
		QComboBox *editor = (QComboBox*)sender();
		emit commitData ( editor );
	}
}
