/**************************************************************************
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

#include "stringcomboboxitemdelegate.h"
#include "hovelitem.h"
#include "propertiesproxymodel.h"
#include "utilities.h"

#include <QComboBox>
#include <QPainter>
#include <QEvent>

namespace Hovel
{

	StringComboBoxItemDelegate::StringComboBoxItemDelegate ( int role, QObject *parent ) :
		QStyledItemDelegate ( parent ),
		_role ( role )
	{
	}

	/*!
	  Create and populate the QComboBox used for editing the role.
	 */
	QWidget *StringComboBoxItemDelegate::createEditor ( QWidget *parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
	{
		QComboBox *editor = new QComboBox ( parent );
		connect ( editor, SIGNAL ( currentIndexChanged ( const QString & ) ), this, SLOT ( valueChanged ( const QString & ) ) );

		PropertiesProxyModel *model = (PropertiesProxyModel*)index.model ();
		editor->insertItems ( 0, model->values ( _role ) );

		return editor;
	}

	/*!
	  Set the initial data shown in the editor to the current value of the item.
	 */
	void StringComboBoxItemDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const
	{
		PropertiesProxyModel *proxyModel = (PropertiesProxyModel*)index.model ();
		HovelItem *item = static_cast<HovelItem *> ( proxyModel->mapToSource ( index ).internalPointer () );
		QString value = item->data ( _role ).toString ();

		QComboBox *comboBox = static_cast<QComboBox*> ( editor );
		comboBox->setCurrentIndex ( comboBox->findText ( value ) );
	}

	/*!
	  Set the data in the underlying model.
	 */
	void StringComboBoxItemDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
	{
		QComboBox * comboBox = static_cast<QComboBox*> ( editor );
		QString value = comboBox->currentText ();

		model->setData ( index, value, _role );
	}

	/*!
	  Resize the combobox's geometry to fit in the table cell.
	 */
	void StringComboBoxItemDelegate::updateEditorGeometry ( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const
	{
		editor->setGeometry ( option.rect );
	}

	/*!
	  Makes sure that the newly selected value is committed to the model. For some reason, data is not
	  committed until the focus moves away from the combo box otherwise.
	 */
	void StringComboBoxItemDelegate::valueChanged ( const QString & text )
	{
		QComboBox *editor = (QComboBox*)sender ();
		emit commitData ( editor );
	}

}
