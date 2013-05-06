#ifndef __TAURUS_BUILDER_H
#define __TAURUS_BUILDER_H

#include "taurus.h"

namespace taurus
{
	using namespace std;

	class Builder
	{
	private:
		map<string, unsigned int> categoryMap;
		map<string, unsigned int> expectationMap;
		Tokenizer tokenizer;
		Parser parser;

	public:
		Builder() {}
		Pattern operator () ();
		Expectation operator [] ();
		Expectation operator [] (string e);
		void noDelim(Pattern ps, Pattern pe);
		void skip(Pattern ps, Pattern pe);
		void whitespace(Pattern p);
		void token(string c, Pattern p);
		void combine(Pattern pa, Pattern pb);
		Expectation expect(string n, Expectation e);
		Expectation expectation(string n);
		Tokenizer getTokenizer();
		Parser getParser();
	};
}

#endif
