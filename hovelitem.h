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


#ifndef HOVELITEM_H
#define HOVELITEM_H

#include <QList>
#include <QVariant>
#include <QMap>
#include <QIcon>
#include <QModelIndex>

#include "serialise.h"

namespace Hovel
{

	//! \see Qt::ItemDataRole
	enum DataRole { TitleRole = 0, TextRole = 32, LastRole };

	class HovelItem : public Serialise
	{
	public:
		virtual ~HovelItem();

		virtual HovelItem * child(int row) = 0;
		HovelItem *parent() { return _parentItem; }
		int row() const;
		int childCount() const;
		int columnCount() const;
		virtual int propertyCount() const;
		QVariant data(int role) const;
		virtual QVariant propertyData(int role) const;
		QString propertyTitle(int role) const;
		const QList<DataRole> displayableProperties() const;
		void appendChild(HovelItem * child);
		void insertChild(HovelItem * child, int row);
		bool setData(const QVariant &value, int role);
		bool canModify() { return _canModify; }
		void setCanModify( bool modify ) { _canModify = modify; }

	protected:
		QList<HovelItem *>	_childItems;
		HovelItem			*_parentItem;
		QMap<int, QVariant>	_roleData;
		bool				_isModified;
		bool				_canModify;
	};

}

#endif // HOVELITEM_H
