#ifndef __TAURUS_LEXICAL_TOKENIZER_H
#define __TAURUS_LEXICAL_TOKENIZER_H

#include <iostream>
#include <string>
#include <vector>
#include "pattern.h"

namespace taurus
{
	using namespace std;

	class Token
	{
	private:
		string content;
		unsigned int line;
		unsigned int column;
		unsigned int type;
		string file;

	public:
		Token() : line(0), column(0), type(0) {}
		Token(string s) : content(s), line(0), column(0), type(0) {}
		Token(string s, unsigned int l, unsigned int c) : content(s), line(l), column(c), type(0) {}
		Token(string s, unsigned int l, unsigned int c, unsigned int t) : content(s), line(l), column(c), type(t) {}
		Token(string s, unsigned int l, unsigned int c, unsigned int t, string f) : content(s), line(l), column(c), type(t), file(f) {}
		Token &setContent(string s);
		Token &setLine(unsigned int l);
		Token &setColumn(unsigned int c);
		Token &setType(unsigned int t);
		Token &setFile(string f);

		bool empty();

		string get();
		string getContent();
		unsigned int getLine();
		unsigned int getColumn();
		unsigned int getType();
		string getFile();

		bool operator == (Token other);
		bool operator == (string other);
		bool operator == (unsigned int other);
		bool operator != (Token other);
		bool operator != (string other);
		bool operator != (unsigned int other);
	};

	class Tokenizer
	{
	private:
		vector<Pattern> noDelimStart;
		vector<Pattern> noDelimEnd;
		vector<Pattern> skipStart;
		vector<Pattern> skipEnd;
		vector<Pattern> whitespace;
		vector<Pattern> deliminator;
		vector<pair<unsigned int, Pattern> > categorizers;

		void appendPatternToVectorSorted(vector<Pattern> &v, Pattern p);
		unsigned int fitsPatternVector(string s, vector<Pattern> v);
		unsigned int isNoDelimStart(string s);
		unsigned int isNoDelimEnd(string s);
		unsigned int isSkipStart(string s);
		unsigned int isSkipEnd(string s);
		unsigned int isWhitespace(string s);
		unsigned int isDeliminator(string s);
		unsigned int categorize(Token t);

	public:
		Tokenizer() {}
		Tokenizer &addNoDelimStart(Pattern p);
		Tokenizer &addNoDelimEnd(Pattern p);
		Tokenizer &addSkipStart(Pattern p);
		Tokenizer &addSkipEnd(Pattern p);
		Tokenizer &addWhitespace(Pattern p);
		Tokenizer &addDeliminator(Pattern p);
		Tokenizer &addCategorizer(unsigned int t, Pattern p);

		vector<Token> tokenize(string s);
	};
}

#endif
