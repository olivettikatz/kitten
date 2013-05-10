#include "error.h"

namespace ktp
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
