#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>
#include <QPersistentModelIndex>

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

	public slots:
		void contentChanged ( );

	private:
		QPersistentModelIndex _index;
		QString _title;
	};

}

#endif // TEXTEDIT_H
