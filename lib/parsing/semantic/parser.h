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
	protected:
		string _class;
		string _name;
		bool _debug;

	public:
		void name(string n);
		void nameClass(string c);
		string getName();
		string getClass();
		virtual void debug();
		void debugTokens(vector<Token> toks, unsigned int off);
		void debugAST(AST rtn, unsigned int stackc);
		bool debugging();
		string pad(unsigned int stackc);
		virtual unsigned int minLength();
		virtual AST parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, unsigned int stackc);
	};

	class One : public Expectation
	{
	private:
		string expecting;
		bool _keep;

	public:
		One() : _keep(false) {nameClass("One");}
		One(string e) : _keep(false), expecting(e) {nameClass("One");name(e);}
		One(string e, bool k) : _keep(k), expecting(e) {nameClass("One");name(e);}
		One &keep();
		unsigned int minLength();
		AST parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, unsigned int stackc);
	};

	class OneDB
	{
	private:
		map<string, One> content;

	public:
		OneDB() {}
		One &one(string n);
		One &keep(string n);
		One &operator [] (string n);
	};

	class Sequence : public Expectation
	{
	private:
		int size;
		vector<Expectation *> sequence;

	public:
		Sequence() : size(-1) {nameClass("Sequence");}
		Sequence(string n) : name(n), size(-1) {nameClass("Sequence");name(n);}
		void debug();
		void assumeSize(unsigned int s);
		Sequence &append(Expectation &e);
		unsigned int minLength();
		AST parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, unsigned int stackc);
	};

	class Parallel : public Expectation
	{
	private:
		int size;
		vector<Expectation *> parallels;

	public:
		Parallel() : size(-1) {}
		void debug();
		void assumeSize(unsigned int s);
		Parallel &append(Expectation &e);
		unsigned int minLength();
		AST parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, unsigned int stackc);
	};

	class Many : public Expectation
	{
	private:
		string name;
		Expectation *one;

	public:
		Many() : one(NULL) {}
		Many(string n) : name(n), one(NULL) {}
		Many(string n, Expectation &e);
		void debug();
		unsigned int minLength();
		AST parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, unsigned int stackc);
	};

	class Maybe : public Expectation
	{
	private:
		Expectation *expecting;

	public:
		Maybe() {}
		Maybe(Expectation &e);
		void debug();
		unsigned int minLength();
		AST parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, unsigned int stackc);
	};
}

#endif
