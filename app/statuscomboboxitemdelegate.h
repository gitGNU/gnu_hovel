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


#ifndef STATUSCOMBOBOXITEMDELEGATE_H
#define STATUSCOMBOBOXITEMDELEGATE_H

#include <QStyledItemDelegate>

namespace Hovel
{

	class StatusComboBoxItemDelegate : public QStyledItemDelegate
	{
		Q_OBJECT

	public:
		explicit StatusComboBoxItemDelegate(QObject *parent = 0);

		QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
		void setEditorData(QWidget *editor, const QModelIndex &index) const;
		void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
		void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
		void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

	private slots:
		void statusChanged(int index);
	};

}
#endif // STATUSCOMBOBOXITEMDELEGATE_H
