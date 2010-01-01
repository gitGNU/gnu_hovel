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
