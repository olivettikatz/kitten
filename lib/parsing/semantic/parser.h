#ifndef __KITTEN_PARSING_PARSER_H
#define __KITTEN_PARSING_PARSER_H

#include <map>
#include "error.h"
#include "ast.h"

namespace parsing
{
	using namespace std;

	class Expectation
	{
	public:
		unsigned int minLength();
		AST parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf);
	};

	class One : public Expectation
	{
	private:
		string expecting;
		bool _keep;

	public:
		One() : _keep(false) {}
		One(string e) : _keep(false), expecting(e) {}
		One(string e, bool k) : _keep(k), expecting(e) {}
		One &keep();
		unsigned int minLength();
		AST parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf);
	};

	class Sequence : public Expectation
	{
	private:
		vector<shared_ptr<Expectation> > sequence;

	public:
		Sequence() {}
		Sequence &append(Expectation &e);
		unsigned int minLength();
		AST parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf);
	};

	class Parallel : public Expectation
	{
	private:
		vector<shared_ptr<Expectation> > parallels;

	public:
		Parallel() {}
		Parallel &append(Expectation &e);
		unsigned int minLength();
		AST parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf);
	};

	class Many : public Expectation
	{
	private:
		shared_ptr<Expectation> one;

	public:
		Many() : one(NULL) {}
		Many(Expectation &e);
		unsigned int minLength();
		AST parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf);
	};

	class Expectation
	{
	private:
		string id;
		string expectation;
		bool _keep;
		bool _many;
		vector<string> sequence;
		vector<pair<unsigned int, string> > alternates;

	public:
		Expectation() : _keep(false), _many(false) {}
		Expectation(string e) : expectation(e), _keep(false), _many(false) {}
		Expectation &identify(string i);
		Expectation &operator || (Expectation e);
		Expectation &operator << (Expectation e);
		Expectation &keep();
		Expectation &many();

		string getID();
		string getExpectation();
		bool getKeep();
		bool getMany();
		vector<string> getSequence();
		vector<string> getAlternates();

		string display();
	};

	class Parser
	{
	private:
		map<string, Expectation> content;

	public:
		Parser() {}
		Expectation operator () ();
		Expectation operator () (string n, string e);
		Expectation operator () (string n);
		Expectation &preadd(string n);
		Expectation &add(string n, Expectation e);
		Parser many(string n);
		AST parse(string n, vector<Token> toks, unsigned int &off, vector<Error> &ebuf);
	};
}

#endif
