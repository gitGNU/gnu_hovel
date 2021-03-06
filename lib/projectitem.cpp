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


#include "projectitem.h"
#include "bookitem.h"
#include "folderitem.h"

namespace Hovel
{

	ProjectItem::ProjectItem()
		: HovelItem()
	{
		_parentItem = 0;
		//Set the column data. As this is the root item, this will be the column header
		_roleData[Qt::DisplayRole] = "Title";
		_canModify = true;
	}

	HovelItem * ProjectItem::child ( int row ) const
	{
		return _childItems.value ( row );
	}

	QDomElement ProjectItem::toQDomElement(QDomDocument& doc)
	{
		QDomElement e = doc.createElement("Project");
		e.setAttribute("CanModify", QVariant(_canModify).toString());

		foreach(HovelItem * item, _childItems) {
			e.appendChild(item->toQDomElement(doc));
		}

		return e;
	}

	void ProjectItem::fromQDomElement(QDomElement &el)
	{
		QDomNode n = el.firstChild();
		_canModify = QVariant(el.attribute("CanModify", "true")).toBool();
		while (!n.isNull()) {
			HovelItem *newItem = 0;
			if (n.isElement()) {
				QDomElement childElement = n.toElement();
				if(childElement.tagName() == "Book")
					newItem = new BookItem(this, childElement.attribute("Title"), childElement.attribute("Author"));
				else if(childElement.tagName() == "Folder")
					newItem = new FolderItem(this, childElement.attribute("Title"));
				else continue;

				newItem->fromQDomElement(childElement);
				_childItems.append(newItem);
			}
			n = n.nextSibling();
		}
	}

	QList<BookItem *> ProjectItem::bookItems()
	{
		QList<BookItem *> books;

		for ( int i=0; i<childCount(); ++i ) {
			BookItem * bookItem = dynamic_cast<BookItem *>(child(i));
			if( bookItem )
				books.append ( bookItem );
		}

		return books;
	}

}
