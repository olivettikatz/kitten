#ifndef __TAURUS_PARSER_H
#define __TAURUS_PARSER_H

#include <map>
#include "error.h"
#include "ast.h"

namespace taurus
{
	using namespace std;

	class Expectation
	{
	private:
		string id;
		string expectation;
		bool _keep;
		bool many;
		vector<string> sequence;
		vector<string> alternates;

	public:
		Expectation() : expectation(0), _keep(false), many(false) {}
		Expectation(string e) : expectation(e), _keep(false), many(false) {}
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
