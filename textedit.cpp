#include "textedit.h"

namespace Hovel
{

	TextEdit::TextEdit( const QModelIndex & idx, QWidget * parent, QString text )
		: QTextEdit ( text, parent ), _index ( idx )
	{
		document()->setDocumentMargin(30);
	}

	void TextEdit::contentChanged ( )
	{
		setWindowModified ( true );
	}

	void TextEdit::setFullScreenState ( )
	{
		setViewportMargins ( 250, 50, 250, 50 );
	}



}
