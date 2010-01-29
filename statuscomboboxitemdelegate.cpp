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

#include <QComboBox>

namespace Hovel
{

	StatusComboBoxItemDelegate::StatusComboBoxItemDelegate(QObject *parent)
		: QItemDelegate(parent)
	{
	}

	QWidget *StatusComboBoxItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		QComboBox *editor = new QComboBox(parent);

		QStringList items;
		items << "New" << "Outline" << "Draft" << "Revising" << "Complete";
		editor->insertItems(0, items);
		return editor;
	}

	void StatusComboBoxItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
	{
		Status status = (Status)index.model()->data(index, StatusRole).toInt();

		QComboBox *comboBox = static_cast<QComboBox*>(editor);
		comboBox->setCurrentIndex(status);
	}

	void StatusComboBoxItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
	{
		QComboBox *comboBox = static_cast<QComboBox*>(editor);
		Status status = (Status)comboBox->currentIndex();

		model->setData(index, status, StatusRole);
	}

	void StatusComboBoxItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		editor->setGeometry(option.rect);
	}

}
