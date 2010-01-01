#ifndef FOLDERITEM_H
#define FOLDERITEM_H

#include "hovelitem.h"

#include <QList>
#include <QDomElement>

namespace Hovel
{

	class FolderItem : public HovelItem
	{
	public:
		FolderItem(HovelItem * parent, QString title);

		//HovelItem functions
		HovelItem * child(int row);
		QVariant data(int role) const;

		//Serialise functions
		bool isModified();
		QDomElement toQDomElement(QDomDocument&);
		void fromQDomElement(QDomElement &);

	private:
	};

}

#endif // FOLDERITEM_H
