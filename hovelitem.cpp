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


#include "hovelitem.h"

namespace Hovel
{

	HovelItem::~HovelItem()
	{
		qDeleteAll(_childItems);
	}

	int HovelItem::row() const
	{
		if (_parentItem)
			return _parentItem->_childItems.indexOf(const_cast<HovelItem*>(this));
		return 0;
	}

	int HovelItem::childCount() const
	{
		return _childItems.count();
	}

	int HovelItem::columnCount() const
	{
		return 1;
	}

	int HovelItem::propertyCount() const
	{
		return _roleData.count();
	}

	/*!
	  Returns the \a role data of this item.
	  \a role is defined by the #DataRole enumeration.
	 */
	QVariant HovelItem::data(int role) const
	{
		if(_roleData.contains(role))
			return _roleData.value(role);

		return QVariant();
	}

	QVariant HovelItem::propertyData(int role) const
	{
		return QVariant();
	}

	QString HovelItem::propertyTitle(int role) const
	{
		switch (role) {
		case TitleRole:
			return "Title";
		case TextRole:
			return "Text";
		default:
			return QString();
		}
	}

	const QList<DataRole> HovelItem::displayableProperties() const
	{
		QList<DataRole> p;
		p << TitleRole;

		return p;
	}

	void HovelItem::appendChild(HovelItem * child)
	{
		_childItems.append(child);
	}

	void HovelItem::insertChild(HovelItem * child, int row)
	{
		_childItems.insert(row, child);
	}

	/*!
	  Set the \a role data of this item to \a value.
	 */
	bool HovelItem::setData(const QVariant &value, int role)
	{
		_roleData[role] = value;
		if(role == Qt::EditRole)
			_roleData[TitleRole] = value;
		return true;
	}

}
