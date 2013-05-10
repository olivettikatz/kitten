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
		string etype;
		string eid;

	public:
		Error() {}
		Error(Token s, string et, string ei) : source(s), etype(et), eid(ei) {}
		Token getSource();
		string getExpectedType();
		string getExpectationID();
	};
}

#endif
