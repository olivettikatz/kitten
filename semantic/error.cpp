#include "error.h"

namespace taurus
{
	Token Error::getSource()
	{
		return source;
	}

	unsigned int Error::getExpectedType()
	{
		return etype;
	}

	unsigned int Error::getExpectationID()
	{
		return eid;
	}
}
