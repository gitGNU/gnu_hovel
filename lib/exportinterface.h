/****************************************************************************

Copyright (C) 2011 Jonathan Crowe.

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

#ifndef EXPORTINTERFACE_H
#define EXPORTINTERFACE_H

#include <QString>
#include <QFileDialog>

namespace Hovel
{
	class BookItem;

	class ExportInterface
	{
	public:
		virtual ~ExportInterface () {}

		virtual QString name () const = 0;
		virtual QString menuText () const = 0;

		virtual bool exportBook ( const BookItem * ) = 0;
	};

}

Q_DECLARE_INTERFACE ( Hovel::ExportInterface, "Hovel.ExportInterface/1.0" )

#endif // EXPORTINTERFACE_H
