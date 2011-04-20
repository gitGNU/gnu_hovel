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

	BookItem::BookItem(HovelItem * parent, QString title, QString author)
		: HovelItem()
	{
		_parentItem = parent;
		_roleData[TitleRole] = title;
		_roleData[AuthorRole] = author;
		_roleData[Qt::DecorationRole] = QIcon(QObject::tr(":/images/book"));
		_canModify = true;
	}

	HovelItem * BookItem::child(int row)
	{
		return _childItems.value(row);
	}

	/*!
	  Return the number of displayable properties.
	 */
	int BookItem::propertyCount() const
	{
		return displayableProperties().count();
	}

	QVariant BookItem::propertyData(int role) const
	{
		if(_roleData.contains(role) && displayableProperties().contains(DataRole(role)))
			return _roleData.value(role);

		return QVariant();
	}

	const QList<DataRole> BookItem::displayableProperties() const
	{
		QList<DataRole> p;
		p << TitleRole << AuthorRole;

		return p;
	}

	QDomElement BookItem::toQDomElement(QDomDocument& doc)
	{
		QDomElement e = doc.createElement("Book");
		e.setAttribute("Title", _roleData[TitleRole].toString());
		e.setAttribute("Author", _roleData[AuthorRole].toString());
		e.setAttribute("CanModify", QVariant(_canModify).toString());

		foreach(HovelItem * item, _childItems) {
			e.appendChild(item->toQDomElement(doc));
		}

		return e;
	}

	void BookItem::fromQDomElement(QDomElement &el)
	{
		QDomNode n = el.firstChild();
		_canModify = QVariant(el.attribute("CanModify", "true")).toBool();
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

	QList<ChapterItem *> BookItem::chapterItems()
	{
		QList<ChapterItem *> chapters;

		for ( int i=0; i<childCount(); ++i ) {
			ChapterItem * chapterItem = dynamic_cast<ChapterItem *>(child(i));
			if( chapterItem )
				chapters.append ( chapterItem );
		}

		return chapters;
	}

}
