#include "propertiesproxymodel.h"

namespace Hovel
{

	PropertiesProxyModel::PropertiesProxyModel()
	{
	}

	QModelIndex PropertiesProxyModel::index ( int row, int column, const QModelIndex & parent ) const
	{
		return QModelIndex();
	}

	QModelIndex PropertiesProxyModel::parent(const QModelIndex &child) const
	{
		return QModelIndex();
	}

	int PropertiesProxyModel::rowCount(const QModelIndex &parent) const
	{
		return 5;
	}

	int PropertiesProxyModel::columnCount(const QModelIndex &parent) const
	{
		return 2;
	}

	QModelIndex PropertiesProxyModel::mapToSource ( const QModelIndex & proxyIndex ) const
	{
		return QModelIndex();
	}

	QModelIndex PropertiesProxyModel::mapFromSource ( const QModelIndex & sourceIndex ) const
	{
		return QModelIndex();
	}

}
