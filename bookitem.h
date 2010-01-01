#ifndef BOOKITEM_H
#define BOOKITEM_H

#include "hovelitem.h"

#include <QList>
#include <QDomElement>

namespace Hovel
{

	class BookItem : public HovelItem
	{
	public:
		BookItem(HovelItem * parent, QString title);

		//HovelItem functions
		HovelItem * child(int row);

		//Serialise functions
		bool isModified();
		QDomElement toQDomElement(QDomDocument&);
		void fromQDomElement(QDomElement &);

	private:
	};

}

#endif // BOOKITEM_H
