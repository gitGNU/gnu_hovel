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

	void ManuscriptPDFDocumentLayout::documentChanged ( int from, int charsRemoved, int charsAdded )
	{
		_pageCount = 2;

		QTextDocument *doc = document ();

		qreal height = 50;
		for ( QTextBlock block = doc->begin (); block != doc->end (); block = block.next () ) {
			QTextLayout * textLayout = block.layout ();
			textLayout->beginLayout ();

			forever {
				QTextLine line = textLayout->createLine ();
				if ( !line.isValid () )
					break;

				line.setLineWidth ( 500 );
				line.setPosition( QPointF ( 50, height ) );
				height += ( line.height ()* 2 );
			}

			textLayout->endLayout ();
		}

		emit pageCountChanged ( 1 );
		emit documentSizeChanged ( QSizeF ( 500, 500 ) );
		emit update ();
	}

	void ManuscriptPDFDocumentLayout::draw ( QPainter *painter, const PaintContext &context )
	{
		QTextDocument *doc = document ();

		QTextBlock block = doc->begin ();

		while ( 1 ) {
			if ( !block.isValid () ) break;
			QTextLayout *tl = block.layout ();
			tl->draw ( painter, QPoint ( 50, 50) );
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

}
