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


#include "bookitem.h"
#include "folderitem.h"
#include "textitem.h"
#include "chapteritem.h"

namespace Hovel
{

	BookItem::BookItem(HovelItem * parent, QString title)
		: HovelItem()
	{
		_parentItem = parent;
		_roleData[TitleRole] = title;
		_roleData[Qt::DecorationRole] = QIcon(QObject::tr(":/images/book"));
	}

	HovelItem * BookItem::child(int row)
	{
		return _childItems.value(row);
	}

	bool BookItem::isModified()
	{
		return true;
	}

	QDomElement BookItem::toQDomElement(QDomDocument& doc)
	{
		QDomElement e = doc.createElement("Book");
		e.setAttribute("Title", _roleData[TitleRole].toString());

		foreach(HovelItem * item, _childItems) {
			e.appendChild(item->toQDomElement(doc));
		}

		return e;
	}

	void BookItem::fromQDomElement(QDomElement &el)
	{
		QDomNode n = el.firstChild();
		while (!n.isNull()) {
			HovelItem *newItem = 0;
			if (n.isElement()) {
				QDomElement childElement = n.toElement();
				if(childElement.tagName() == "Chapter")
					newItem = new ChapterItem(this, childElement.attribute("Title"));
				else if(childElement.tagName() == "Text")
					newItem = new TextItem(this, childElement.attribute("Title"));
				else continue;

				newItem->fromQDomElement(childElement);
				_childItems.append(newItem);
			}
			n = n.nextSibling();
		}
	}

}
