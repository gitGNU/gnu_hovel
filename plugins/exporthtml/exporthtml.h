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

#ifndef EXPORTHTML_H
#define EXPORTHTML_H

#include <QObject>

#include "exportinterface.h"

class ExportHtml :	public QObject,
					public Hovel::ExportInterface
{
    Q_OBJECT
	Q_INTERFACES ( Hovel::ExportInterface )
public:
	QString name () const { return "ExportHTML"; }
	QString menuText () const { return "to single HTML file"; }

	bool exportBook ( const Hovel::BookItem * );

private:
	QString loadTextResource ( QString resource ) const;

	QString				_htmlHeadTemplate;
	QString				_htmlBodyTemplate;
};

#endif // EXPORTHTML_H
