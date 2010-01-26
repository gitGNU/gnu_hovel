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


#include "chapteritem.h"
#include "textitem.h"

namespace Hovel
{

	ChapterItem::ChapterItem(HovelItem * parent, QString title)
		: HovelItem()
	{
		_parentItem = parent;
		_roleData[TitleRole] = title;
		_roleData[Qt::DecorationRole] = QIcon(QObject::tr(":/images/chapter"));
		_canModify = true;;
	}

	HovelItem * ChapterItem::child(int row)
	{
		return _childItems.value(row);
	}

	/*!
	  Return the number of displayable properties.
	 */
	int ChapterItem::propertyCount() const
	{
		return displayableProperties().count();
	}

	QVariant ChapterItem::propertyData(int role) const
	{
		if(_roleData.contains(role) && displayableProperties().contains(DataRole(role)))
			return _roleData.value(role);

		return QVariant();
	}

	const QList<DataRole> ChapterItem::displayableProperties() const
	{
		QList<DataRole> p;
		p << TitleRole;

		return p;
	}

	bool ChapterItem::isModified()
	{
		return true;
	}

	QDomElement ChapterItem::toQDomElement(QDomDocument& doc)
	{
		QDomElement e = doc.createElement("Chapter");
		e.setAttribute("Title", _roleData[TitleRole].toString());
		e.setAttribute("CanModify", QVariant(_canModify).toString());

		foreach(HovelItem * item, _childItems) {
			e.appendChild(item->toQDomElement(doc));
		}

		return e;
	}

	void ChapterItem::fromQDomElement(QDomElement &el)
	{
		QDomNode n = el.firstChild();
		_canModify = QVariant(el.attribute("CanModify", "true")).toBool();
		while (!n.isNull()) {
			HovelItem *newItem = 0;
			if (n.isElement()) {
				QDomElement childElement = n.toElement();
				if(childElement.tagName() == "Text") {
					newItem = new TextItem(this, childElement.attribute("Title"));
				}
				else continue;

				newItem->fromQDomElement(childElement);
				_childItems.append(newItem);
			}
			n = n.nextSibling();
		}
	}
}
