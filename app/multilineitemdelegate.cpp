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

#include "multilineitemdelegate.h"
#include "propertiesproxymodel.h"

#include <QTextEdit>

namespace Hovel
{

	MultiLineItemDelegate::MultiLineItemDelegate(QObject *parent)
		: QStyledItemDelegate(parent)
	{
	}

	/*!
	  Create and populate the QTextEdit used for editing the role.
	 */
	QWidget * MultiLineItemDelegate::createEditor ( QWidget *parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
	{
		QTextEdit *editor = new QTextEdit ( parent );

		PropertiesProxyModel *model = (PropertiesProxyModel*)index.model ();
		editor->setPlainText ( model->data ( index, SummaryRole ).toString () );

		return editor;
	}

	/*!
	  Set the data in the underlying model.
	 */
	void MultiLineItemDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
	{
		QTextEdit * textEdit = static_cast<QTextEdit*> ( editor );
		QString value = textEdit->toPlainText ();

		model->setData ( index, value, SummaryRole );
	}

	/*!
	  Resize the editors geometry to show more text than the table
	  cell will allow.
	 */
	void MultiLineItemDelegate::updateEditorGeometry ( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const
	{
		QRect editingRect = option.rect;
		editingRect.adjust ( 0, 0, 0, 100 );
		editor->setGeometry ( editingRect );
	}

}
