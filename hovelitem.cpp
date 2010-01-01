#include "hovelitem.h"

namespace Hovel
{

	HovelItem::~HovelItem()
	{
		qDeleteAll(_childItems);
	}

	int HovelItem::row() const
	{
		if (_parentItem)
			return _parentItem->_childItems.indexOf(const_cast<HovelItem*>(this));
		return 0;
	}

	int HovelItem::childCount() const
	{
		return _childItems.count();
	}

	int HovelItem::columnCount() const
	{
		return 1;
	}

	QVariant HovelItem::data(int role) const
	{
		if(role == Qt::EditRole)
			role = TitleRole;
		return _roleData.value(role);
	}

	void HovelItem::appendChild(HovelItem * child)
	{
		_childItems.append(child);
	}

	void HovelItem::insertChild(HovelItem * child, int row)
	{
		_childItems.insert(row, child);
	}

	bool HovelItem::setData(const QVariant &value, int role)
	{
		_roleData[role] = value;
		if(role == Qt::EditRole)
			_roleData[TitleRole] = value;
		return true;
	}

}
