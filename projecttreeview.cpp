#include "projecttreeview.h"

namespace Hovel
{

	ProjectTreeView::ProjectTreeView()
	{
		setHeaderHidden(true);
		setSelectionMode(QAbstractItemView::SingleSelection);
		setEditTriggers(EditKeyPressed|SelectedClicked);
	}

}
