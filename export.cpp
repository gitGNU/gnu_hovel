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
#include <QTextDocument>
#include <QPainter>
#include <QAbstractTextDocumentLayout>
#include <QTextBlock>

#include "export.h"
#include "utilities.h"
#include "manuscriptpdfdocumentlayout.h"

namespace Hovel
{

	/*!
	  Construct an \c Export object for \a model.
	 */
	Export::Export ( QWidget * parent, HovelModel * model )
		:	_parent ( parent ),
			_model ( model ),
			_convertToUnderlineEmphasis ( true )
	{
		loadExportTemplates ();
	}

	bool Export::getExportFilename ( QString nameFilter )
	{
		_fileName = QFileDialog::getSaveFileName ( 0, QString ( "Export" ), QString (), nameFilter );
		if ( _fileName.length () > 0 ) return true;

		return false;
	}

	/*!
	  Exports a \c BookItem as a single HTML file.
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

		QTextDocument document;
		document.setHtml ( headText + bodyText );
		stream << document.toHtml ();

		outFile->close ();

		delete outFile;

		return true;
	}

	bool Export::toPrinter ( BookItem * book, QPrinter::OutputFormat outputFormat )
	{
		QPrinter printer;
		setupPrinter ( printer, outputFormat );

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
		QTextBlockFormat separatorBlockFormat;
		separatorBlockFormat.setAlignment ( Qt::AlignHCenter );

		//Create the document contents, one chaper per frame.
		foreach ( ChapterItem * chapterItem, book->chapterItems() ) {
			QTextFrame * mainFrame = cursor.currentFrame();
			cursor.insertFrame ( frameFormat );
			foreach ( TextItem * textItem, chapterItem->textItems() ) {
				cursor.insertHtml ( textItem->data ( TextRole ).toString () );
				cursor.setBlockFormat ( blockFormat );
				if ( !textItem->IsLastItem () ) {
					cursor.insertBlock ();
					cursor.setBlockFormat ( separatorBlockFormat );
					cursor.insertHtml ( "*" );
					cursor.insertBlock ();
					cursor.setBlockFormat ( blockFormat );
				}
			}
			cursor = mainFrame->lastCursorPosition ();
		}

		QTextCharFormat fmt;
		fmt.setFontFamily ( "Courier" );
		fmt.setFontPointSize ( 12 );
		cursor.select ( QTextCursor::Document );
		cursor.mergeCharFormat ( fmt );

		//Convert bold/italic to underline if requested.
		if ( _convertToUnderlineEmphasis ) {
			convertToUnderlineEmphasis ( doc );
		}

		doc.setDocumentLayout ( new ManuscriptPDFDocumentLayout ( &doc ) );
		QPainter painter ( &printer );
		QAbstractTextDocumentLayout::PaintContext paintContext;

		int pageNumber = 0;
		QRectF pageRect = printer.pageRect ();

		while ( pageNumber < doc.pageCount () ) {
			painter.save ();
			painter.translate ( 0, -( pageNumber * pageRect.height () ) );
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

	void Export::setupPrinter ( QPrinter & printer, QPrinter::OutputFormat outputFormat )
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

			if ( !getExportFilename ( filter ) )
				return;
			QRegExp regexpSuffix ( "\\." + suffix + "$$", Qt::CaseInsensitive );
			if ( regexpSuffix.indexIn ( _fileName ) == -1 )
				_fileName += suffix;

			printer.setOutputFileName ( _fileName );
		}
	}

	void Export::convertToUnderlineEmphasis ( QTextDocument& doc )
	{
		QTextCharFormat underlineFormat;
		underlineFormat.setFontWeight ( QFont::Normal );
		underlineFormat.setFontUnderline ( true );
		underlineFormat.setFontItalic ( false );

		for ( QTextBlock currentBlock = doc.begin (); currentBlock != doc.end (); currentBlock = currentBlock.next () ) {
			for (QTextBlock::iterator fragmentIterator = currentBlock.begin(); ! fragmentIterator.atEnd (); ++fragmentIterator) {
				QTextFragment currentFragment = fragmentIterator.fragment();
				if (currentFragment.isValid()) {
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

	/*!
	  Exports a \c BookItem in Manuscript format as a PDF file.
	 */
	bool Export::toManuscriptPDF ( BookItem * bookItem )
	{
		if( !getExportFilename( tr ( "PDF files (*.pdf)" ) ) )
			return false;

		QRegExp regexpSuffix("\\.pdf$$", Qt::CaseInsensitive);
		if(regexpSuffix.indexIn(_fileName) == -1)
			_fileName += ".pdf";

		QPrinter printer;
		printer.setOutputFileName ( _fileName );
		printer.setOutputFormat ( QPrinter::PdfFormat );
		QPainter painter ( &printer );
		QRectF pageRect ( printer.pageRect() );
		QRectF body = QRectF ( 0, 0, pageRect.width (), pageRect.height () );

		foreach ( ChapterItem * chapterItem, bookItem->chapterItems() ) {
			QTextDocument doc;
			QTextCursor cursor ( &doc );
			doc.setPageSize ( body.size () );
			int pageNumber = 1;

			foreach ( TextItem * textItem, chapterItem->textItems() ) {
				cursor.insertHtml ( textItem->data ( TextRole ).toString () );
				cursor.insertHtml ( "<BR><BR><center>*</center><BR><BR>" );
			}

			QTextDocument * clonedDoc = doc.clone ();
			ManuscriptPDFDocumentLayout * manLayout = new ManuscriptPDFDocumentLayout ( clonedDoc );
			clonedDoc->setDocumentLayout ( manLayout );

			for ( QTextBlock currentBlock = doc.begin (); currentBlock != doc.end (); currentBlock = currentBlock.next () ) {
				for (QTextBlock::iterator fragmentIterator = currentBlock.begin(); !( fragmentIterator.atEnd () ); ++fragmentIterator) {
					QTextFragment currentFragment = fragmentIterator.fragment();
					if (currentFragment.isValid()) {
						QTextCursor clonedDocCursor ( clonedDoc );
						clonedDocCursor.setPosition ( currentFragment.position () );
						clonedDocCursor.setPosition ( currentFragment.position () + currentFragment.length (), QTextCursor::KeepAnchor );
						QTextCharFormat fmt;
						fmt.setFontFamily ( "Courier" );
						fmt.setFontPointSize ( 12 );
						fmt.setFontWeight ( QFont::Normal );
						clonedDocCursor.setCharFormat ( fmt );
					}
				}
			}

			while ( pageNumber - 1 < clonedDoc->pageCount () ) {
				QAbstractTextDocumentLayout * layout = clonedDoc->documentLayout ();
				QAbstractTextDocumentLayout::PaintContext paintContext;
				painter.save ();
				painter.translate ( body.left (), body.top () - ( pageNumber - 1 ) * body.height () );
				QRectF view ( 0, ( pageNumber - 1 ) * body.height (), body.width (), body.height () );
				painter.setClipRect( view );
				paintContext.clip = view;
				paintContext.palette.setColor ( QPalette::Text, Qt::black );
				layout->draw ( &painter, paintContext );
				painter.restore ();

				++pageNumber;
				printer.newPage ();
			}
		}

		return true;
	}

	void Export::loadExportTemplates()
	{
		_htmlHeadTemplate = loadTextResource( ":/htmltemplates/head" );
		_htmlBodyTemplate = loadTextResource( ":/htmltemplates/body" );
	}

}
