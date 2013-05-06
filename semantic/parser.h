#ifndef __TAURUS_PARSER_H
#define __TAURUS_PARSER_H

#include "expectation.h"

namespace taurus
{
	using namespace std;

	class Expectation
	{
	private:
		string id;
		string expectation;
		bool keep;
		bool many;
		vector<string> sequence;
		vector<string> alternates;

	public:
		Expectation() : expectation(0), keep(false), many(false) {}
		Expectation(string e) : expectation(e), keep(false), many(false) {}
		Expectation &identify(string i);
		Expectation &operator || (Expectation e);
		Expectation &operator , (Expectation e);
		Expectation &operator ++ ();
		Expectation &keep();

		string getID();
		string getExpectation();
		bool getKeep();
		bool getMany();
		vector<string> getSequence();
		vector<string> getAlternates();
	};

	class Parser
	{
	private:
		map<string, Expectation> content;

	public:
		Parser() {}
		Expectation operator [] (string e);
		Expectation operator () ();
		Expectation operator () (string n, string e);
		Expectation operator () (string n);
		Parser &add(Expectation e);
		AST parse(string n, vector<Token> toks, unsigned int &off, vector<Error> &ebuf);
	};
}

#endif
