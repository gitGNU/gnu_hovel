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

#include <QFileDialog>
#include <QTextStream>
#include <QRegExp>
#include <QXmlStreamReader>

#include "export.h"
#include "utilities.h"

namespace Hovel
{

	/*!
	  Construct an \c Export object for \a model.
	 */
	Export::Export(QWidget * parent, HovelModel * model)
		: _parent(parent), _model ( model )
	{
		loadExportTemplates();
	}

	bool Export::getExportFilename(QString nameFilter)
	{
		QFileDialog dialog( _parent );
		dialog.setFileMode ( QFileDialog::AnyFile );
		dialog.setAcceptMode ( QFileDialog::AcceptSave );
		dialog.setNameFilter ( nameFilter );
		if ( dialog.exec ( ) ) {
			_fileName = dialog.selectedFiles ( ).first ( );
			return true;
		}

		return false;
	}

	/*!
	  Exports a \c HovelModel as a single HTML file.
	 */
	bool Export::toHtmlFile(BookItem * bookItem)
	{
		if( !getExportFilename( tr ( "Html files (*.html *.htm)" ) ) )
			return false;

		QRegExp regexpSuffix("\\.htm$|\\.html$", Qt::CaseInsensitive);
		if(regexpSuffix.indexIn(_fileName) == -1)
			_fileName += ".html";
		QFile* outFile = new QFile ( _fileName );
		if ( !outFile->open ( QIODevice::WriteOnly | QIODevice::Text ) ) return false;

		QTextStream stream( outFile );

		QString	headText = QString(_htmlHeadTemplate).arg(bookItem->data(TitleRole).toString());
		QString bodyText;
		bodyText.append(QString("<h1><center>%1</center></h1>").arg(bookItem->data(TitleRole).toString()));

		foreach ( ChapterItem * chapterItem, bookItem->chapterItems() ) {
			bodyText.append(QString("<h2><center>%1</center></h2>").arg(chapterItem->data(TitleRole).toString()));
			foreach ( TextItem * textItem, chapterItem->textItems() ) {
				bodyText.append( textItem->toHtmlParagraphs() );
			}
		}

		stream << headText << QString(_htmlBodyTemplate).arg(bodyText);

		outFile->close();

		delete outFile;

		return true;
	}

	void Export::loadExportTemplates()
	{
		_htmlHeadTemplate = loadTextResource( ":/htmltemplates/head" );
		_htmlBodyTemplate = loadTextResource( ":/htmltemplates/body" );
	}

}
