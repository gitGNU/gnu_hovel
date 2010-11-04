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


#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>
#include <QPersistentModelIndex>

#include "formattingtoolbar.h"

namespace Hovel
{

	class TextEdit : public QTextEdit
	{
		Q_OBJECT

	public:
		TextEdit ( const QModelIndex & idx, QWidget * parent, QString text );
		QString title ( ) { return _title; }
		void setTitle ( QString title ) { _title = title; }
		QPersistentModelIndex index ( ) { return _index; }
		void setFullScreenState ( );
		void setNormalState ( );

	public slots:
		void textChanged ( );

	protected:
		void keyPressEvent ( QKeyEvent * event );

	protected slots:
		void closeEvent ( QCloseEvent * event );
		void setCursorPosition ( int );

	signals:
		void contentChanged ( QPersistentModelIndex&, QString& );
		void exitFullScreenPressed ( );
		void closing ();
		void finalCursorPosition ( int pos );

	private:
		QPersistentModelIndex _index;
		QString _title;
	};

}

#endif // TEXTEDIT_H
