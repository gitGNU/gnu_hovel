#ifndef PROPERTIESPROXYMODEL_H
#define PROPERTIESPROXYMODEL_H

#include <QAbstractProxyModel>

namespace Hovel
{

	class PropertiesProxyModel : public QAbstractProxyModel
	{
	public:
		PropertiesProxyModel();

		QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
		QModelIndex parent(const QModelIndex &child) const;
		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;
		QModelIndex mapToSource ( const QModelIndex & proxyIndex ) const;
		QModelIndex mapFromSource ( const QModelIndex & sourceIndex ) const;

	};

}

#endif // PROPERTIESPROXYMODEL_H
