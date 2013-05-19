#ifndef __KITTEN_PARSING_SEMANTIC_AST_H
#define __KITTEN_PARSING_SEMANTIC_AST_H

#include "../lexical/tokenizer.h"
#include <sstream>

namespace parsing
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
		string display();
	};
}

#endif
