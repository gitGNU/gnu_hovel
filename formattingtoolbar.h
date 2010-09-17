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

#ifndef FORMATTINGTOOLBAR_H
#define FORMATTINGTOOLBAR_H

#include <QToolButton>
#include <QFontComboBox>
#include <QComboBox>

namespace Hovel
{

	class FormattingToolBar : public QWidget
	{
		Q_OBJECT

	public:
		explicit FormattingToolBar(QWidget *parent = 0);

	public slots:
		void boldButtonPressed ( bool checked );
		void italicButtonPressed ( bool checked );
		void underlineButtonPressed ( bool checked );
		void strikeThroughButtonPressed ( bool checked );

		void fontFamilySelected ( const QString& );
		void fontSizeSelected ( const QString& );

		void setCheckedBoldButton ( bool checked );
		void setCheckedItalicButton ( bool checked );
		void setCheckedunderlineButton ( bool checked );
		void setCheckedstrikeThroughButton ( bool checked );
		void setCurrentFontFamily ( const QString& );
		void setCurrentFontSize ( const QString& );

	signals:
		void boldButtonToggled ( bool checked );
		void italicButtonToggled ( bool checked );
		void underlineButtonToggled ( bool checked );
		void strikeThroughButtonToggled ( bool checked );
		void fontFamilyChanged ( const QString& );
		void fontSizeChanged ( const QString& );

	private:
		QToolButton		* _boldButton;
		QToolButton		* _italicButton;
		QToolButton		* _underlineButton;
		QToolButton		* _strikeThroughButton;
		QFontComboBox	* _fontComboBox;
		QComboBox		* _fontSizeComboBox;
	};

}

#endif // FORMATTINGTOOLBAR_H
