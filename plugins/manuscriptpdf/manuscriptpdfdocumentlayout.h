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

#ifndef MANUSCRIPTPDFDOCUMENTLAYOUT_H
#define MANUSCRIPTPDFDOCUMENTLAYOUT_H

#include <QAbstractTextDocumentLayout>

namespace Hovel
{

	class ManuscriptPDFDocumentLayout : public QAbstractTextDocumentLayout
	{
	public:
		explicit ManuscriptPDFDocumentLayout ( QTextDocument * );

		void draw ( QPainter *, const PaintContext & );
		int hitTest ( const QPointF &, Qt::HitTestAccuracy ) const;

		int pageCount () const;
		QSizeF documentSize () const;

		QRectF frameBoundingRect ( QTextFrame * ) const;
		QRectF blockBoundingRect ( const QTextBlock & ) const;

		bool pageRequiresHeader ( int );


	protected:
		void documentChanged ( int position, int charsRemoved, int charsAdded );

	private:
		int				_pageCount;
		QVector<int>	_pagesWithHeader;
	};

}

#endif // MANUSCRIPTPDFDOCUMENTLAYOUT_H
