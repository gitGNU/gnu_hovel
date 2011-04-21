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

#include <QtPlugin>
#include <QTextStream>
#include <QTextDocument>

#include "exporthtml.h"
#include "bookitem.h"

bool ExportHtml::exportBook ( const Hovel::BookItem * bookItem )
{
	using namespace Hovel;

	QString fileName =
			QFileDialog::getSaveFileName ( 0, QString ( "Export" ), QString (), tr ( "Html files (*.html *.htm)" ) );

	QRegExp regexpSuffix("\\.htm$|\\.html$", Qt::CaseInsensitive);
	if ( regexpSuffix.indexIn ( fileName ) == -1 )
		fileName += ".html";
	QFile* outFile = new QFile ( fileName );
	if ( !outFile->open ( QIODevice::WriteOnly | QIODevice::Text ) ) return false;

	QTextStream stream ( outFile );

	_htmlHeadTemplate = loadTextResource( ":/htmltemplates/head" );
	_htmlBodyTemplate = loadTextResource( ":/htmltemplates/body" );

	QString	headText = QString (_htmlHeadTemplate).arg
							( bookItem->data ( TitleRole ).toString () );
	QString bodyText = ( QString ( "<h1><center>%1</center></h1>" ).arg
							( bookItem->data ( TitleRole ).toString () ) );

	foreach ( ChapterItem * chapterItem, bookItem->chapterItems () ) {
		bodyText.append ( QString ( "<h2><center>%1</center></h2>" ).arg
						  ( chapterItem->data ( TitleRole ).toString () ) );
		foreach ( TextItem * textItem, chapterItem->textItems () ) {
			bodyText.append ( textItem->toHtmlParagraphs () );
		}
	}

	QTextDocument document;
	document.setHtml ( headText + bodyText );
	stream << document.toHtml ();

	outFile->close ();

	delete outFile;

	return true;
}

QString ExportHtml::loadTextResource ( QString resource ) const
{
	QFile * file = new QFile ( resource );
	file->open( QIODevice::ReadOnly );
	QTextStream stream ( file );
	QString contents = stream.readAll();
	file->close();
	delete file;
	return contents;
}


Q_EXPORT_PLUGIN2 ( exporthtml, ExportHtml )
