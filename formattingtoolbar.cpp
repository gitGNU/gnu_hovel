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

#include "formattingtoolbar.h"

#include <QHBoxLayout>

namespace Hovel
{

	FormattingToolBar::FormattingToolBar(QWidget *parent)
	 : QWidget(parent)
	{
		_boldButton = new QToolButton();
		_boldButton->setIcon ( QIcon ( tr ( ":/images/bold" ) ) );
		_boldButton->setIconSize ( QSize ( 16, 16 ) );
		_boldButton->setCheckable ( true );
		connect ( _boldButton, SIGNAL ( toggled ( bool ) ), this, SLOT ( boldButtonPressed ( bool ) ) );

		_italicButton = new QToolButton();
		_italicButton->setIcon ( QIcon ( tr ( ":/images/italic" ) ) );
		_italicButton->setIconSize ( QSize ( 16, 16 ) );
		_italicButton->setCheckable ( true );
		connect ( _italicButton, SIGNAL ( toggled ( bool ) ), this, SLOT ( italicButtonPressed ( bool ) ) );

		_underlineButton = new QToolButton();
		_underlineButton->setIcon ( QIcon ( tr ( ":/images/underline" ) ) );
		_underlineButton->setIconSize ( QSize ( 16, 16 ) );
		_underlineButton->setCheckable ( true );
		connect ( _underlineButton, SIGNAL ( toggled ( bool ) ), this, SLOT ( underlineButtonPressed ( bool ) ) );

		_strikeThroughButton = new QToolButton();
		_strikeThroughButton->setIcon ( QIcon ( tr ( ":/images/strikeThrough" ) ) );
		_strikeThroughButton->setIconSize ( QSize ( 16, 16 ) );
		_strikeThroughButton->setCheckable ( true );
		connect ( _strikeThroughButton, SIGNAL ( toggled ( bool ) ), this, SLOT ( strikeThroughButtonPressed ( bool ) ) );

		QHBoxLayout * stylesLayout = new QHBoxLayout();
		stylesLayout->addWidget ( _boldButton );
		stylesLayout->addWidget ( _italicButton );
		stylesLayout->addWidget ( _underlineButton );
		stylesLayout->addWidget ( _strikeThroughButton );
		stylesLayout->addStretch();
		stylesLayout->setSpacing ( 1 );

		_fontComboBox = new QFontComboBox();
		_fontSizeComboBox = new QComboBox();
		QHBoxLayout * fontStyleLayout = new QHBoxLayout();
		fontStyleLayout->addWidget( _fontComboBox );
		fontStyleLayout->addWidget( _fontSizeComboBox );
		fontStyleLayout->addStretch();
		fontStyleLayout->setSpacing ( 1 );

		QVBoxLayout * layout = new QVBoxLayout();
		layout->addLayout ( stylesLayout );
		layout->addLayout ( fontStyleLayout );
		layout->setSpacing ( 0 );
		layout->setContentsMargins(0,0,0,1);

		setLayout ( layout );
	}

	void FormattingToolBar::setCheckedBoldButton ( bool checked )
	{
		_boldButton->setChecked ( checked );
	}

	void FormattingToolBar::setCheckedItalicButton ( bool checked )
	{
		_italicButton->setChecked ( checked );
	}

	void FormattingToolBar::setCheckedunderlineButton ( bool checked )
	{
		_underlineButton->setChecked ( checked );
	}

	void FormattingToolBar::setCheckedstrikeThroughButton ( bool checked )
	{
		_strikeThroughButton->setChecked ( checked );
	}

	void FormattingToolBar::boldButtonPressed ( bool checked )
	{
		emit boldButtonToggled ( checked );
	}

	void FormattingToolBar::italicButtonPressed ( bool checked )
	{
		emit italicButtonToggled ( checked );
	}

	void FormattingToolBar::underlineButtonPressed ( bool checked )
	{
		emit underlineButtonToggled ( checked );
	}

	void FormattingToolBar::strikeThroughButtonPressed ( bool checked )
	{
		emit strikeThroughButtonToggled ( checked );
	}

}
