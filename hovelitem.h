#ifndef HOVELITEM_H
#define HOVELITEM_H

#include <QList>
#include <QVariant>
#include <QMap>
#include <QIcon>
#include <QModelIndex>

#include "serialise.h"

namespace Hovel
{

	class HovelItem : public Serialise
	{
	public:
		enum DataRole { TitleRole = 0, TextRole = 32 };

		virtual ~HovelItem();

		virtual HovelItem * child(int row) = 0;
		HovelItem *parent() { return _parentItem; }
		int row() const;
		int childCount() const;
		int columnCount() const;
		QVariant data(int role) const;
		void appendChild(HovelItem * child);
		void insertChild(HovelItem * child, int row);
		bool setData(const QVariant &value, int role);

	protected:
		QList<HovelItem *>	_childItems;
		HovelItem			*_parentItem;
		QMap<int, QVariant>	_roleData;
		bool				_isModified;
	};

}

#endif // HOVELITEM_H
