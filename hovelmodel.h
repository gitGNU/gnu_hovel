#ifndef HOVELMODEL_H
#define HOVELMODEL_H

#include <QAbstractItemModel>

#include "projectitem.h"

namespace Hovel
{

	/*!
	  A HovelModel represents a Hovel project.
	 */

	class HovelModel : public QAbstractItemModel
	{
	public:
		HovelModel(QObject *parent = 0);

		void newProject();

		QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
		QModelIndex parent(const QModelIndex &child) const;
		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;
		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role) const;
		bool hasChildren ( const QModelIndex & parent = QModelIndex() ) const;
		Qt::ItemFlags flags(const QModelIndex &index) const;
		bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

		bool newBook(int row);
		bool newChapter(QModelIndex parentBook, int row);
		bool newScene(QModelIndex parentChapter, int row);
		int lastBook();
		int bookCount();
		QModelIndex currentBook(QModelIndexList selectedItems);
		QModelIndex currentChapter(QModelIndexList selectedItems);

		bool isModified();

		bool open ( QString fileName );
		bool save ( QString fileName );

	private:
		bool insertItem(HovelItem* newItem, const QModelIndex& parent = QModelIndex(), int row = 0);

	private:
		ProjectItem *rootItem;
	};

}

#endif // HOVELMODEL_H
