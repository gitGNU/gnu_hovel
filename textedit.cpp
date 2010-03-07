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


#include <QApplication>

#include "textedit.h"

namespace Hovel
{

	TextEdit::TextEdit( const QModelIndex & idx, QWidget * parent, QString text )
		: QTextEdit ( text, parent ), _index ( idx )
	{
		document()->setDocumentMargin(30);
		connect(this, SIGNAL(textChanged()), this, SLOT(textChanged()));
	}

	void TextEdit::textChanged ( )
	{
		QString html = toHtml();
		emit contentChanged( _index, html );
	}

	void TextEdit::setFullScreenState ( )
	{
		setStyleSheet ( "background: dimgrey; color: lightsteelblue; border: none;" );
		setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

		QTextCursor cursor = textCursor();
		int originalPosition = cursor.position();

		cursor.movePosition(QTextCursor::Start);
		cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);

		QTextCharFormat format;
		format.setFontPointSize(13);
		cursor.setCharFormat(format);

		cursor.setPosition(originalPosition);
		setTextCursor(cursor);

		showMaximized();

		int horizontalMargin = viewport()->width() / 4;
		int verticalMargin = viewport()->height() / 7;
		setViewportMargins ( horizontalMargin, verticalMargin, horizontalMargin, verticalMargin );
	}

	void TextEdit::setNormalState ( )
	{
		setStyleSheet ("");
		setVerticalScrollBarPolicy ( Qt::ScrollBarAsNeeded );

		QTextCursor cursor = textCursor();

		cursor.movePosition ( QTextCursor::Start );
		cursor.movePosition ( QTextCursor::End, QTextCursor::KeepAnchor );

		QTextCharFormat format;
		format.setFontPointSize ( QApplication::font().pointSize() );
		cursor.setCharFormat ( format );

		showNormal();
		setViewportMargins ( 0, 0, 0, 0 );
	}

}
