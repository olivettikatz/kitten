#ifndef __KTP_SEMANTIC_AST_H
#define __KTP_SEMANTIC_AST_H

#include "../lexical/tokenizer.h"

namespace ktp
{
	using namespace std;

	class AST
	{
	private:
		string eid;
		Token content;
		vector<AST> children;

	public:
		AST() {}
		AST(string e, Token c) : eid(e), content(c) {}
		bool good();
		bool empty();
		AST &add(AST a);
		string getExpectationID();
		Token getContent();
		unsigned int getChildrenSize();
		AST getChild(unsigned int idx);
	};
}

#endif
