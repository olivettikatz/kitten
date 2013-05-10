#include "builder.h"

namespace taurus
{
	Pattern Builder::operator () ()
	{
		return Pattern();
	}

	Expectation Builder::operator [] ()
	{
		return parser();
	}

	Expectation Builder::operator [] (string e)
	{
		return parser(categoryMap[e]);
	}

	void Builder::noDelim(Pattern ps, Pattern pe)
	{
		tokenizer.addNoDelim(ps, pe);
	}

	void Builder::skip(Pattern ps, Pattern pe)
	{
		tokenziner.addSkip(ps, pe);
	}

	void Builder::whitespace(Pattern p)
	{
		tokenizer.addWhitespace(p);
	}

	void Builder::token(string c, Pattern p)
	{
		categoryMap[c] = categoryMap.size()+1;
		tokenizer.addCategorizerDeliminator(categoryMap[c], p);
	}

	void Builder::combine(Pattern pa, Pattern pb)
	{
		tokenizer.addCombinator(pa, pb);
	}

	Expectation Builder::expect(string n, Expectation e)
	{
		expectationMap[n] = expectationMap.size()+1;
		parser.add(e);
		return e;
	}

	Expectation Builder::expectation(string n)
	{
		return parser[expectationMap[n]];
	}

	Tokenizer Builder::getTokenizer()
	{
		return tokenizer;
	}

	Parser Parser::getParser()
	{
		return parser;
	}
}

