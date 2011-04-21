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

#include <QStringList>

#include "hovelitemmimedata.h"

namespace Hovel
{

	HovelItemMimeData::HovelItemMimeData()
	{
	}

	QStringList HovelItemMimeData::formats() const
	{
		QStringList formats;
		formats << "application/hovelitems";
		return formats;
	}

	const QList<HovelItem *> HovelItemMimeData::items() const
	{
		return _items;
	}

	void HovelItemMimeData::appendItem(HovelItem * newItem)
	{
		_items.append(newItem);
	}

}
