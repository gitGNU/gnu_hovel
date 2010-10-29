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

#include "povcomboboxitemdelegate.h"
#include "hovelitem.h"
#include "propertiesproxymodel.h"
#include "utilities.h"
#include <QComboBox>
#include <QPainter>
#include <QEvent>

namespace Hovel
{

	POVComboBoxItemDelegate::POVComboBoxItemDelegate ( QObject *parent ) :
			QStyledItemDelegate ( parent )
	{
	}

	/*!
	  Create and populate the QComboBox used for editing the POV character.
	 */
	QWidget *POVComboBoxItemDelegate::createEditor ( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const
	{
		QComboBox *editor = new QComboBox ( parent );
		connect ( editor, SIGNAL ( currentIndexChanged ( const QString & ) ), this, SLOT ( povChanged ( const QString & ) ) );

		PropertiesProxyModel *model = (PropertiesProxyModel*)index.model ();
		editor->insertItems ( 0, model->characters () );

		return editor;
	}

	/*!
	  Set the initial data shown in the editor to the current value of the item's POV.
	 */
	void POVComboBoxItemDelegate::setEditorData ( QWidget *editor, const QModelIndex &index ) const
	{
		PropertiesProxyModel *proxyModel = (PropertiesProxyModel*)index.model ();
		HovelItem *item = static_cast<HovelItem *> ( proxyModel->mapToSource ( index ).internalPointer () );
		QString pov = item->data ( POVRole ).toString ();

		QComboBox *comboBox = static_cast<QComboBox*> ( editor );
		comboBox->setCurrentIndex ( comboBox->findText ( pov ) );
	}

	/*!
	  Set the data in the underlying model.
	 */
	void POVComboBoxItemDelegate::setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
	{
		QComboBox *comboBox = static_cast<QComboBox*> ( editor );
		QString pov = comboBox->currentText ();

		model->setData ( index, pov, POVRole );
	}

	/*!
	  Resize the combobox's geometry to fit in the table cell.
	 */
	void POVComboBoxItemDelegate::updateEditorGeometry ( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const
	{
		editor->setGeometry ( option.rect );
	}

	/*!
	  Makes sure that the newly selected POV is committed to the model. For some reason, data is not
	  committed until the focus moves away from the combo box otherwise.
	 */
	void POVComboBoxItemDelegate::povChanged ( const QString & text )
	{
		QComboBox *editor = (QComboBox*)sender ();
		emit commitData ( editor );
	}

}
