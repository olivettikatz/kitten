#ifndef __KTP_PARSER_H
#define __KTP_PARSER_H

#include <map>
#include "error.h"
#include "ast.h"

namespace ktp
{
	using namespace std;

	class Expectation
	{
	private:
		string id;
		string expectation;
		bool _keep;
		bool _many;
		vector<string> sequence;
		vector<string> alternates;

	public:
		Expectation() : expectation(0), _keep(false), _many(false) {}
		Expectation(string e) : expectation(e), _keep(false), _many(false) {}
		Expectation identify(string i);
		Expectation operator || (Expectation e);
		Expectation operator << (Expectation e);
		Expectation keep();
		Expectation many();

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
		Parser add(Expectation e);
		Parser add(string n, Expectation e);
		Parser many(string n);
		AST parse(string n, vector<Token> toks, unsigned int &off, vector<Error> &ebuf);
	};
}

#endif
