#ifndef __TAURUS_SEMANTIC_AST_H
#define __TAURUS_SEMANTIC_AST_H

#include "../lexical/tokenizer.h"

namespace taurus
{
	using namespace std;

	class AST
	{
	private:
		unsigned int eid;
		Token content;
		vector<AST> children;

	public:
		AST() : eid(0) {}
		AST(unsigned int e, Token c) : eid(e), content(c) {}
		bool good();
		bool empty();
		AST &add(AST a);
		unsigned int getExpectationID();
		Token getContent();
		unsigned int getChildrenSize();
		AST getChild(unsigned int idx);
	};
}

#endif
