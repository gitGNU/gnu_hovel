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
	}

	HovelItem * ProjectItem::child(int row)
	{
		return _childItems.value(row);
	}

	bool ProjectItem::isModified()
	{
		return true;
	}

	QDomElement ProjectItem::toQDomElement(QDomDocument& doc)
	{
		QDomElement e = doc.createElement("Project");

		foreach(HovelItem * item, _childItems) {
			e.appendChild(item->toQDomElement(doc));
		}

		return e;
	}

	void ProjectItem::fromQDomElement(QDomElement &el)
	{
		QDomNode n = el.firstChild();
		while (!n.isNull()) {
			HovelItem *newItem = 0;
			if (n.isElement()) {
				QDomElement childElement = n.toElement();
				if(childElement.tagName() == "Book")
					newItem = new BookItem(this, childElement.attribute("Title"));
				else if(childElement.tagName() == "Folder")
					newItem = new FolderItem(this, childElement.attribute("Title"));
				else continue;

				newItem->fromQDomElement(childElement);
				_childItems.append(newItem);
			}
			n = n.nextSibling();
		}
	}

}
