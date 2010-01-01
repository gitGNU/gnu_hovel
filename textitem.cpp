#include "textitem.h"

namespace Hovel
{

	TextItem::TextItem(HovelItem * parent, QString title, QString text)
		: HovelItem()
	{
		_parentItem = parent;
		_roleData[TitleRole] = title;
		_roleData[TextRole] = text;
		_roleData[Qt::DecorationRole] = QIcon(QObject::tr(":/images/text"));
	}

	HovelItem * TextItem::child(int row)
	{
		return _childItems.value(row);
	}

	bool TextItem::isModified()
	{
		return true;
	}

	QDomElement TextItem::toQDomElement(QDomDocument& doc)
	{
		QDomElement e = doc.createElement("Text");
		e.setAttribute("Title", _roleData[TitleRole].toString());

		foreach(HovelItem * item, _childItems) {
			e.appendChild(item->toQDomElement(doc));
		}

		return e;
	}

	void TextItem::fromQDomElement(QDomElement &)
	{
	}

}
