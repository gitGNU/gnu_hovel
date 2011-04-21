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
#include <QTextDocument>
#include <QTextFragment>
#include <QTextCursor>
#include <QPainter>

#include "manuscriptpdf.h"
#include "bookitem.h"
#include "manuscriptpdfdocumentlayout.h"

bool ManuscriptPdf::exportBook ( const Hovel::BookItem * book )
{
	using namespace Hovel;

	QPrinter printer;
	setupPrinter ( printer, QPrinter::PdfFormat );

	QTextDocument doc;
	doc.setPageSize ( QSizeF ( printer.pageRect ().width (), printer.pageRect ().height () ) );

	//Reformat the text, suitable for output.
	QTextCursor cursor ( &doc );
	QTextFrameFormat frameFormat;
	frameFormat.setPageBreakPolicy ( QTextFormat::PageBreak_AlwaysAfter );

	//Format for text paragraphs.
	QTextBlockFormat blockFormat;
	blockFormat.setAlignment ( Qt::AlignLeft );

	//Format for the separator.
	QTextBlockFormat centeredBlockFormat;
	centeredBlockFormat.setAlignment ( Qt::AlignHCenter );

	//Create the document contents, one chaper per frame.
	foreach ( ChapterItem * chapterItem, book->chapterItems() ) {
		QTextFrame * mainFrame = cursor.currentFrame();
		cursor.insertFrame ( frameFormat );

		//Add the chapter title
		cursor.setBlockFormat ( centeredBlockFormat );
		cursor.insertHtml ( chapterItem->data ( Hovel::TitleRole ).toString () );

		//Add the scenes
		cursor.insertBlock ();
		cursor.setBlockFormat ( blockFormat );

		foreach ( TextItem * textItem, chapterItem->textItems() ) {
			cursor.insertHtml ( textItem->data ( TextRole ).toString () );
			cursor.setBlockFormat ( blockFormat );
			if ( !textItem->IsLastItem () ) {
				cursor.insertBlock ();
				cursor.setBlockFormat ( centeredBlockFormat );
				cursor.insertHtml ( "*" );
				cursor.insertBlock ();
				cursor.setBlockFormat ( blockFormat );
			}
		}
		cursor = mainFrame->lastCursorPosition ();
	}

	//Set output font style.
	QTextCharFormat fmt;
	fmt.setFontFamily ( "Courier" );
	fmt.setFontPointSize ( 12 );
	cursor.select ( QTextCursor::Document );
	cursor.mergeCharFormat ( fmt );

	convertToUnderlineEmphasis ( doc );

	ManuscriptPDFDocumentLayout * layout = new ManuscriptPDFDocumentLayout ( &doc );
	doc.setDocumentLayout ( layout );
	QPainter painter ( &printer );
	QAbstractTextDocumentLayout::PaintContext paintContext;

	int pageNumber = 0;
	QRectF pageRect = printer.pageRect ();

	qreal left, top, right, bottom;
	printer.getPageMargins ( &left, &top, &right,&bottom , QPrinter::DevicePixel );
	painter.setFont ( QFont ("Courier", 12 ) );

	while ( pageNumber < doc.pageCount () ) {
		painter.save ();
		painter.translate ( 0, -( pageNumber * pageRect.height () ) );

		if ( layout->pageRequiresHeader ( pageNumber ) ) {
			QString header = QString ("%1/%2/%3")	.arg ( book->data ( Hovel::AuthorRole ).toString () )
													.arg ( book->data ( Hovel::TitleRole ).toString () )
													.arg ( pageNumber + 1 );

			QRectF headerRect ( 0, pageNumber * pageRect.height () -top + 15,
								pageRect.width (), pageRect.height () );
			painter.drawText ( headerRect, Qt::AlignRight, header);
		}

		QRectF view ( 0, pageNumber * pageRect.height (), pageRect.width (), pageRect.height () );
		painter.setClipRect( view );

		doc.documentLayout ()->draw ( &painter, paintContext );

		painter.restore ();

		++pageNumber;
		if ( pageNumber < doc.pageCount () )
			printer.newPage ();
	}

	return true;
}

/*!
  Converts empahsised text ( bold and italic ) in a document to normal,
  underlined text.
 */
void ManuscriptPdf::convertToUnderlineEmphasis ( QTextDocument& doc )
{
	QTextCharFormat underlineFormat;
	underlineFormat.setFontWeight ( QFont::Normal );
	underlineFormat.setFontUnderline ( true );
	underlineFormat.setFontItalic ( false );

	for ( QTextBlock currentBlock = doc.begin (); currentBlock != doc.end (); currentBlock = currentBlock.next () ) {
		for ( QTextBlock::iterator fragmentIterator = currentBlock.begin (); ! fragmentIterator.atEnd (); ++fragmentIterator ) {
			QTextFragment currentFragment = fragmentIterator.fragment ();
			if ( currentFragment.isValid () ) {
				if (	currentFragment.charFormat ().fontWeight () == QFont::Bold ||
						currentFragment.charFormat ().fontItalic () ) {
					QTextCursor cursor ( &doc );
					cursor.setPosition ( currentFragment.position () );
					cursor.movePosition ( QTextCursor::NextCharacter, QTextCursor::KeepAnchor, currentFragment.length () );
					cursor.mergeCharFormat ( underlineFormat );
				}
			}
		}
	}
}

void ManuscriptPdf::setupPrinter ( QPrinter & printer, QPrinter::OutputFormat outputFormat )
{
	printer.setOutputFormat ( outputFormat );
	printer.setPaperSize ( QPrinter::A4 );
	printer.setPageMargins ( 25.4, 25.4, 25.4, 25.4, QPrinter::Millimeter );

	if ( outputFormat == QPrinter::NativeFormat ) {
	}
	else {
		QString filter;
		QString suffix;

		if ( outputFormat == QPrinter::PdfFormat ) {
			filter = tr ( "PDF files (*.pdf)" );
			suffix = "pdf";
		}
		else if ( outputFormat == QPrinter::PostScriptFormat ) {
			filter = tr ( "Postscript files (*.ps)" );
			suffix = "ps";
		}
		else
			return;

		QString fileName =
				QFileDialog::getSaveFileName ( 0, QString ( "Export" ), QString (), filter );

		QRegExp regexpSuffix ( "\\." + suffix + "$$", Qt::CaseInsensitive );
		if ( regexpSuffix.indexIn ( fileName ) == -1 )
			fileName += suffix;

		printer.setOutputFileName ( fileName );
	}
}

Q_EXPORT_PLUGIN2 ( manuscriptpdf, ManuscriptPdf )
