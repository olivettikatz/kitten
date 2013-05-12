#ifndef __KTP_SEMANTIC_ERROR_H
#define __KTP_SEMANTIC_ERROR_H

#include "../lexical/tokenizer.h"

namespace ktp
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
