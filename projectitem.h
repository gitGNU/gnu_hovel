#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include "hovelitem.h"
#include "serialise.h"

#include <QList>
#include <QDomElement>

namespace Hovel
{

	class ProjectItem : public HovelItem
	{
	public:
		ProjectItem();

		const QList<HovelItem *> children() const { return _childItems; }

		//HovelItem functions
		HovelItem * child(int row);

		//Serialise functions
		bool isModified();
		QDomElement toQDomElement(QDomDocument&);
		void fromQDomElement(QDomElement &);

	private:

	};

}

#endif // PROJECTITEM_H
