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
