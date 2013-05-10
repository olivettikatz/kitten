#ifndef __TAURUS_SEMANTIC_EXPECTATION_H
#define __TAURUS_SEMANTIC_EXPECTATION_H

#include "../lexical/tokenizer.h"
#include "error.h"

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

	class Expectation
	{
	private:
		unsigned int id;
		unsigned int type;
		bool keep;
		vector<Expectation> _sequence;
		vector<Expectation> _or;

	public:
		Expectation(unsigned int i, unsigned int t) : id(i), type(t) {}
		Expectation(unsigned int i, unsigned int t, bool k) : id(i), type(t), keep(k) {}
		Expectation &enableKeep();
		Expectation &disableKeep();
		Expectation &addToSequence(Expectation e);
		Expectation &addOr(Expectation e);
		AST expect(vector<Token> toks, unsigned int &off, vector<Error> &ebuf);
	};
}

#endif
