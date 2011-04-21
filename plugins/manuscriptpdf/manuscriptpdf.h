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

#ifndef MANUSCRIPTPDF_H
#define MANUSCRIPTPDF_H

#include <QObject>
#include <QPrinter>

#include "exportinterface.h"

class QTextDocument;

class ManuscriptPdf :	public QObject,
						public Hovel::ExportInterface
{
	Q_OBJECT
	Q_INTERFACES ( Hovel::ExportInterface )
public:
	QString name () const { return "ManuscriptPdf"; }
	QString menuText () const { return "to manuscript PDF"; }
	bool exportBook ( const Hovel::BookItem * );

private:
	void convertToUnderlineEmphasis ( QTextDocument& doc );
	void setupPrinter ( QPrinter & printer, QPrinter::OutputFormat outputFormat );
};

#endif // MANUSCRIPTPDF_H
