/****************************************************************************

Copyright (C) 2010 Jonathan Crowe.

This file is part of Hovel.

Hovel is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Hovel is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Hovel.  If not, see <http://www.gnu.org/licenses/>.

****************************************************************************/


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
