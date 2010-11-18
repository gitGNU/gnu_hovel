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



#ifndef EXPORT_H
#define EXPORT_H

#include <QWidget>

#include "hovelmodel.h"
#include "bookitem.h"

namespace Hovel
{

	class Export : public QObject
	{
		Q_OBJECT

	public:
		explicit Export(QWidget *, HovelModel *);

		bool getExportFilename(QString nameFilter);

		bool toHtmlFile(BookItem *);
		bool toManuscriptPDF ( BookItem * );

	private:
		void loadExportTemplates();

		QWidget				*_parent;
		HovelModel			*_model;
		QString				_fileName;

		QString				_htmlHeadTemplate;
		QString				_htmlBodyTemplate;
	};

}

#endif // EXPORT_H
