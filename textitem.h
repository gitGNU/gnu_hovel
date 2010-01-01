#ifndef TEXTITEM_H
#define TEXTITEM_H

#include "hovelitem.h"
#include <QDomElement>

namespace Hovel
{

	class TextItem : public HovelItem
	{
	public:
		TextItem(HovelItem * parent, QString title, QString text = "");

		//HovelItem functions
		HovelItem * child(int row);

		//Serialise functions
		bool isModified();
		QDomElement toQDomElement(QDomDocument&);
		void fromQDomElement(QDomElement &);
	};

}

#endif // TEXTITEM_H
