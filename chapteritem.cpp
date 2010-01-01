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
	}

	HovelItem * ChapterItem::child(int row)
	{
		return _childItems.value(row);
	}

	bool ChapterItem::isModified()
	{
		return true;
	}

	QDomElement ChapterItem::toQDomElement(QDomDocument& doc)
	{
		QDomElement e = doc.createElement("Chapter");
		e.setAttribute("Title", _roleData[TitleRole].toString());

		foreach(HovelItem * item, _childItems) {
			e.appendChild(item->toQDomElement(doc));
		}

		return e;
	}

	void ChapterItem::fromQDomElement(QDomElement &el)
	{
		QDomNode n = el.firstChild();
		while (!n.isNull()) {
			HovelItem *newItem = 0;
			if (n.isElement()) {
				QDomElement childElement = n.toElement();
				if(childElement.tagName() == "Text")
					newItem = new TextItem(this, childElement.attribute("Title"));
				else continue;

				newItem->fromQDomElement(childElement);
				_childItems.append(newItem);
			}
			n = n.nextSibling();
		}
	}
}
