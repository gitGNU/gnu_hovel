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
