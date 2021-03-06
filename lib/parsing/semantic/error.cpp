#include "error.h"

namespace parsing
{
	Token Error::getSource()
	{
		return source;
	}

	string Error::getExpectedType()
	{
		return etype;
	}

	string Error::getExpectationID()
	{
		return eid;
	}
}
