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


#include "folderitem.h"
#include "textitem.h"

namespace Hovel
{

	FolderItem::FolderItem(HovelItem * parent, QString title)
	{
		_parentItem = parent;
		_roleData[Qt::DisplayRole] = title;
		_roleData[Qt::DecorationRole] = QIcon(QObject::tr(":/images/folder"));
	}

	HovelItem * FolderItem::child(int row)
	{
		return _childItems.value(row);
	}

	QVariant FolderItem::data(int role) const
	{
		return _roleData.value(role);
	}

	bool FolderItem::isModified()
	{
		return false;
	}

	QDomElement FolderItem::toQDomElement(QDomDocument& doc)
	{
		QDomElement e = doc.createElement("Folder");
		e.setAttribute("Title", _roleData[TitleRole].toString());

		foreach(HovelItem * item, _childItems) {
			e.appendChild(item->toQDomElement(doc));
		}

		return e;
	}

	void FolderItem::fromQDomElement(QDomElement &)
	{
	}

}
