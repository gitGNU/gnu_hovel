/**************************************************************************
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

#include "manuscriptpdfdocumentlayout.h"

#include <QTextBlock>
#include <QPainter>

namespace Hovel
{

	/*!
	  \class ManuscriptPDFDocumentLayout
	  \brief The ManuscriptPDFDocumentLayout class implements a layout for standard manuscript formatting.
	 */

	ManuscriptPDFDocumentLayout::ManuscriptPDFDocumentLayout ( QTextDocument * doc )
		:	QAbstractTextDocumentLayout ( doc ),
		_pageCount ( 0 )
	{
	}

	void ManuscriptPDFDocumentLayout::documentChanged ( int /*position*/, int /*charsRemoved*/, int /*charsAdded*/ )
	{
		QTextDocument *doc = document ();
		QTextFrame * rootFrame = doc->rootFrame ();
		qreal ySize =  doc->pageSize ().height ();
		qreal lineSpacing = 2;
		qreal bottomOfPageY = ySize;
		qreal yPosition = ySize / 2 ;
		int pageNumber = 0;
		_pagesWithHeader.clear ();

		//Each chapter has been placed in its own frame in export.cpp. Now iterate through them.
		foreach ( QTextFrame* frame, rootFrame->childFrames () ) {

			//Iterate through each block (paragraph) in the current frame.
			QTextFrame::iterator currentBlockIterator = frame->begin ();

			while ( currentBlockIterator != frame->end () ) {
				QTextBlock currentBlock = currentBlockIterator.currentBlock ();
				if ( !currentBlock.isValid () ) {
					++currentBlockIterator;
					continue;
				}

				bool newParagraph = true;
				QTextBlockFormat blockformat = currentBlock.blockFormat ();
				QTextLayout * textLayout = currentBlock.layout ();
				qreal leftIndent = 0;

				//Create each line in the paragraph
				textLayout->beginLayout ();
				forever {
					QTextLine line = textLayout->createLine ();
					if ( !line.isValid () )
						break;

					qreal lineWidth = doc->pageSize ().width ();

					if ( newParagraph ) {
						leftIndent = 30;
						lineWidth -= 30;
						newParagraph = false;
					}
					else {
						leftIndent = 0;
					}

					//Fill the line with text
					line.setLineWidth ( lineWidth );

					//Justify centred text.
					if ( blockformat.alignment () == Qt::AlignHCenter )
						leftIndent = ( lineWidth - line.naturalTextWidth () ) / 2;

					line.setPosition ( QPointF ( leftIndent, yPosition ) );

					yPosition += ( line.height () * lineSpacing );

					//Eject page if not enough room for next line.
					if ( yPosition + line.height () > bottomOfPageY ) {
						yPosition += ( bottomOfPageY - yPosition );
						bottomOfPageY = ( static_cast<int>(yPosition) / static_cast<int>(ySize) + 1 ) * ySize;
						_pagesWithHeader.push_back ( ++pageNumber );
					}

				}

				textLayout->endLayout ();

				++currentBlockIterator;
			}

			//If the frame requires a page eject.
			if ( frame->frameFormat ().pageBreakPolicy () & QTextFormat::PageBreak_AlwaysAfter ) {
				bottomOfPageY = ( static_cast<int>(yPosition) / static_cast<int>(ySize) + 1 ) * ySize;
				yPosition += ( bottomOfPageY - yPosition + ySize / 2 );
				bottomOfPageY = ( static_cast<int>(yPosition) / static_cast<int>(ySize) + 1 ) * ySize;
				++pageNumber;
			}
		}

		//Calculate page count.
		_pageCount = static_cast<int>(yPosition) / static_cast<int>(ySize);

		emit pageCountChanged ( _pageCount );
		emit documentSizeChanged ( QSizeF ( doc->pageSize ().width (), yPosition ) );
		emit update ();

	}

	void ManuscriptPDFDocumentLayout::draw ( QPainter *painter, const PaintContext & /*context*/ )
	{
		QTextDocument *doc = document ();

		QTextBlock block = doc->begin ();

		while ( 1 ) {
			if ( !block.isValid () ) break;
			QTextLayout *tl = block.layout ();
			tl->draw ( painter, QPoint ( 0, 0 ) );
			block = block.next ();
		}
	}

	/*!
	  Since the document this layout is for will never be displayed, this function need
	  not be implemented.
	 */
	int ManuscriptPDFDocumentLayout::hitTest ( const QPointF &point, Qt::HitTestAccuracy accuracy ) const
	{
		return -1;
	}

	int ManuscriptPDFDocumentLayout::pageCount () const
	{
		return _pageCount;
	}

	QSizeF ManuscriptPDFDocumentLayout::documentSize () const
	{
		return QSizeF();
	}

	QRectF ManuscriptPDFDocumentLayout::frameBoundingRect ( QTextFrame * frame ) const
	{
		return QRectF();
	}

	QRectF ManuscriptPDFDocumentLayout::blockBoundingRect ( const QTextBlock & block ) const
	{
		return QRectF();
	}

	/*!
	  Determines if \a page requires a header to be output.
	 */
	bool ManuscriptPDFDocumentLayout::pageRequiresHeader ( int page )
	{
		return _pagesWithHeader.contains ( page ) ? true : false;
	}

}
