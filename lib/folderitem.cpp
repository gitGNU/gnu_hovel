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
#include "characteritem.h"
#include "locationitem.h"

namespace Hovel
{

	FolderItem::FolderItem(HovelItem * parent, QString title)
	{
		_parentItem = parent;
		_roleData[Qt::DisplayRole] = title;
		_roleData[Qt::DecorationRole] = QIcon(QObject::tr(":/images/folder"));
		_canModify = true;
	}

	HovelItem * FolderItem::child ( int row ) const
	{
		return _childItems.value ( row );
	}

	QVariant FolderItem::data(int role) const
	{
		return _roleData.value(role);
	}

	QDomElement FolderItem::toQDomElement(QDomDocument& doc)
	{
		QDomElement e = doc.createElement("Folder");
		e.setAttribute("Title", _roleData[TitleRole].toString());
		e.setAttribute("CanModify", QVariant(_canModify).toString());

		foreach(HovelItem * item, _childItems) {
			e.appendChild(item->toQDomElement(doc));
		}

		return e;
	}

	void FolderItem::fromQDomElement(QDomElement &el)
	{
		_canModify = QVariant(el.attribute("CanModify", "true")).toBool();
		QDomNode n = el.firstChild();
		while (!n.isNull()) {
			HovelItem *newItem = 0;
			if (n.isElement()) {
				QDomElement childElement = n.toElement();
				if(childElement.tagName() == "Character") {
					newItem = new CharacterItem ( this, childElement.attribute ( "Title" ) );
				}
				else if ( childElement.tagName () == "Location" ) {
					newItem = new LocationItem ( this, childElement.attribute ( "Title" ) );
				}
				else continue;

				newItem->fromQDomElement(childElement);
				_childItems.append(newItem);
			}
			n = n.nextSibling();
		}
	}

}
