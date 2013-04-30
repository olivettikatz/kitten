#ifndef __TAURUS_SEMANTIC_ERROR_H
#define __TAURUS_SEMANTIC_ERROR_H

#include "../lexical/tokenizer.h"

namespace taurus
{
	using namespace std;

	class Error
	{
	private:
		Token source;
		unsigned int etype;
		unsigned int eid;

	public:
		Error() : etype(0), eid(0) {}
		Error(Token s, unsigned int et, unsigned int ei) : source(s), etype(et), eid(ei) {}
		Token getSource();
		unsigned int getExpectedType();
		unsigned int getExpectationID();
	};
}

#endif
