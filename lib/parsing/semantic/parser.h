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
		void debugInput(vector<Token> toks, unsigned int off, unsigned int stackc);
		void debugOutput(AST rtn, unsigned int stackc);
		bool debugging();
		string pad(unsigned int stackc);
		virtual unsigned int minLength();
		virtual AST parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, vector<Expectation *> stack);
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
		AST parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, vector<Expectation *> stack);
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
		Sequence(string n) : size(-1) {nameClass("Sequence");name(n);}
		void debug();
		void assumeSize(unsigned int s);
		Sequence &append(Expectation &e);
		unsigned int minLength();
		AST parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, vector<Expectation *> stack);
	};

	class Parallel : public Expectation
	{
	private:
		int size;
		vector<Expectation *> parallels;

	public:
		Parallel() : size(-1) {nameClass("Parallel");}
		Parallel(string n) : size(-1) {name(n);nameClass("Parallel");}
		void debug();
		void assumeSize(unsigned int s);
		Parallel &append(Expectation &e);
		unsigned int minLength();
		AST parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, vector<Expectation *> stack);
	};

	class Many : public Expectation
	{
	private:
		Expectation *one;

	public:
		Many() : one(NULL) {nameClass("Many");}
		Many(string n) : one(NULL) {name(n);nameClass("Many");}
		Many(string n, Expectation &e) : one(&e) {name(n);nameClass("Many");}
		void debug();
		unsigned int minLength();
		AST parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, vector<Expectation *> stack);
	};

	class Maybe : public Expectation
	{
	private:
		Expectation *expecting;

	public:
		Maybe() {nameClass("Maybe");}
		Maybe(string n, Expectation &e) : expecting(&e) {name(n);nameClass("Maybe");}
		void debug();
		unsigned int minLength();
		AST parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, vector<Expectation *> stack);
	};
}

#endif
