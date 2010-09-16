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
#include <QKeyEvent>

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

		showFullScreen();

		document()->setDocumentMargin ( 300 );
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

		//Reset the document
		document()->setDocumentMargin(4);
		setDocument(document());
	}

	/*!
	  Acts on key presses if necessary.
	 */
	void TextEdit::keyPressEvent(QKeyEvent *event)
	{
		if ( ! (windowState() & Qt::WindowFullScreen) ) {
			QTextEdit::keyPressEvent( event );
			return;
		}

		if ( event->key() == Qt::Key_Escape) {
			emit exitFullScreenPressed();
			close();
		}

		QTextEdit::keyPressEvent ( event );
	}

	/*!
	  Emits a signal that this is closing.
	 */
	void TextEdit::closeEvent ( QCloseEvent * event )
	{
		emit closing ();
		QWidget::closeEvent ( event );
	}
}
