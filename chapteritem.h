#ifndef CHAPTERITEM_H
#define CHAPTERITEM_H

#include "hovelitem.h"
#include <QDomElement>

namespace Hovel
{

	class ChapterItem : public HovelItem
	{
	public:
		ChapterItem(HovelItem * parent, QString title);

		//HovelItem functions
		HovelItem * child(int row);

		//Serialise functions
		bool isModified();
		QDomElement toQDomElement(QDomDocument&);
		void fromQDomElement(QDomElement &);

	private:
	};

}

#endif // CHAPTERITEM_H
