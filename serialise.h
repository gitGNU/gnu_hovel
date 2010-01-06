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


#ifndef SERIALISE_H
#define SERIALISE_H

class QDomElement;
class QDomDocument;

namespace Hovel
{
	/*!
	  This virtual class provides an interface for classes that serialise
	  data.
	 */
	class Serialise
	{
	public:
		//! Destroys the object
		virtual ~Serialise() {}

		//! Reports on modification status.
		virtual bool isModified() = 0;

		//! Returns pointer to self represented by a QDomElement
		virtual QDomElement toQDomElement(QDomDocument&) = 0;

		//! Populates self from a QDomElement
		virtual void fromQDomElement(QDomElement&) = 0;

	};

}


#endif // SERIALISE_H
