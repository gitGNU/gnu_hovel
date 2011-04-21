/**************************************************************************
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

#include "locationitem.h"

namespace Hovel
{

	LocationItem::LocationItem ( HovelItem * parent, QString name )
		: HovelItem ()
	{
		_parentItem = parent;
		_roleData[TitleRole] = name;
		_roleData[TextRole] = "";
		_roleData[Qt::DecorationRole] = QIcon ( QObject::tr ( ":/images/text" ) );
		_roleData[SummaryRole] = QString ();
		_canModify = true;
	}

	HovelItem * LocationItem::child ( int row ) const
	{
		return _childItems.value ( row );
	}

	/*!
	  Return the number of displayable properties.
	 */
	int LocationItem::propertyCount () const
	{
		return displayableProperties ().count ();
	}

	QVariant LocationItem::propertyData ( int role ) const
	{
		if ( _roleData.contains ( role ) && displayableProperties ().contains ( DataRole ( role ) ) )
			return _roleData.value ( role );

		return QVariant ();
	}

	const QList<DataRole> LocationItem::displayableProperties () const
	{
		QList<DataRole> p;
		p << TitleRole << SummaryRole;

		return p;
	}

	QDomElement LocationItem::toQDomElement ( QDomDocument& doc )
	{
		QDomElement e = doc.createElement ( "Location" );
		e.setAttribute ( "Title", _roleData[TitleRole].toString () );
		e.setAttribute ( "CanModify", QVariant ( _canModify ).toString () );

		QDomElement summaryElement = doc.createElement ( "Summary" );
		QDomText summaryTextNode = doc.createTextNode ( _roleData[SummaryRole].toString () );
		summaryElement.appendChild ( summaryTextNode );
		e.appendChild ( summaryElement );

		QString text = _roleData[TextRole].toString ();
		QDomText textNode = doc.createTextNode ( text );
		e.appendChild ( textNode );

		return e;
	}

	void LocationItem::fromQDomElement ( QDomElement &el )
	{
		_canModify = QVariant ( el.attribute ( "CanModify", "true" ) ).toBool ();

		QDomNode n = el.firstChild ();
		while ( !n.isNull () ) {
			if ( n.isElement() ) {		//Child elements
				QDomElement childElement = n.toElement ();
				if ( childElement.tagName () == "Summary") {
					QDomText dt = childElement.firstChild ().toText ();
					_roleData[SummaryRole] = dt.data ();
				}
				else continue;
			}
			else if ( n.isText () ) {		//Child text nodes
				QDomText dt = n.toText ();
				_roleData[TextRole] = dt.data ();
			}
			n = n.nextSibling ();
		}
	}

}
