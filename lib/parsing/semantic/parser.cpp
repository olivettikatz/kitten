#include "parser.h"

namespace parsing
{
	void Expectation::name(string n)
	{
		_name = n;
	}

	void Expectation::nameClass(string c)
	{
		_class = c;
	}

	string Expectation::getName()
	{
		return _name;
	}

	string Expectation::getClass()
	{
		return _class;
	}

	void Expectation::debug()
	{
		_debug = true;
	}

	void Expectation::debugTokens(vector<Token> toks, unsigned int off)
	{
		if (_debug)
		{
			cout << "Parsing:";
			for (unsigned int i = off; i < off+10 && i < toks.size(); i++)
				cout << " '" << toks[i].get() << "'";
			cout << "\n";
		}
	}

	void Expectation::debugAST(AST rtn, unsigned int stackc)
	{
		if (_debug)
			cout << pad(stackc) << "Returning: " << rtn.display() << "\n";
	}

	bool Expectation::debugging()
	{
		return _debug;
	}

	string Expectation::pad(unsigned int stackc)
	{
		string rtn;
		for (unsigned int i = 0; i < stackc; i++)
			rtn += " ";
		return rtn;
	}

	unsigned int Expectation::minLength()
	{
		return 0;
	}

	AST Expectation::parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, unsigned int stackc)
	{
		return AST();
	}

	One &One::keep()
	{
		_keep = true;
		return *this;
	}

	unsigned int One::minLength()
	{
		if (expecting.empty())
			return 0;
		else
			return 1;
	}

	AST One::parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, unsigned int stackc)
	{
		cout << pad(stackc) << "One '" << expecting << "' ";
		debugTokens(toks, off);

		if (toks[off].getType().compare(expecting) == 0)
		{
			off++;
			if (_keep)
			{
				if (debugging())
					cout << pad(stackc) << "Expectation of One '" << expecting << "' met, keeping...\n";
				debugAST(AST(expecting, toks[off]), stackc);
				return AST(expecting, toks[off]);
			}
			else
			{
				if (debugging())
					cout << pad(stackc) << "Expectation of One '" << expecting << "' met.\n";
				debugAST(AST(), stackc);
				return AST();
			}
		}
		else
		{
			if (debugging())
				cout << pad(stackc) << "Expectation of One '" << expecting << "' NOT met, erroring...\n";
			ebuf.push_back(Error(toks[off], expecting, expecting));
			debugAST(AST(), stackc);
			return AST();
		}
	}

	One &OneDB::one(string n)
	{
		content[n] = One(n);
		return content[n];
	}

	One &OneDB::keep(string n)
	{
		content[n] = One(n).keep();
		return content[n];
	}

	One &OneDB::operator [] (string n)
	{
		return content[n];
	}

	void Sequence::debug()
	{
		_debug = true;
		for (vector<Expectation *>::iterator i = sequence.begin(); i != sequence.end(); i++)
			if ((*i)->debugging() == false)
				(*i)->debug();
	}

	void Sequence::assumeSize(unsigned int s)
	{
		size = (signed int)s;
	}

	Sequence &Sequence::append(Expectation &e)
	{
		sequence.push_back(&e);
		return *this;
	}

	unsigned int Sequence::minLength()
	{
		if (size >= 0)
			return (unsigned int)size;

		unsigned int rtn = 0;
		for (vector<Expectation *>::iterator i = sequence.begin(); i != sequence.end(); i++)
			rtn += (*i)->minLength();
		return rtn;
	}

	AST Sequence::parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, unsigned int stackc)
	{
		cout << pad(stackc) << "Sequence '" << name << "' ";
		debugTokens(toks, off);

		AST rtn = AST(name, toks[off]);
		for (vector<Expectation *>::iterator i = sequence.begin(); i != sequence.end(); i++)
		{
			AST tmp = (*i)->parse(toks, off, ebuf, stackc+1);
			if (tmp.good() == false)
			{
				if (debugging())
					cout << pad(stackc) << "Expectation of Sequence '" << name << "' NOT met, erroring...\n";
				ebuf.push_back(Error(toks[off], name, name));
				debugAST(AST(), stackc);
				return AST();
			}
			else
			{
				if (debugging())
					cout << pad(stackc) << "Expectation of Sequence '" << name << "' met.\n";
			}
		}

		debugAST(rtn, stackc);
		return rtn;
	}

	void Parallel::debug()
	{
		_debug = true;
		for (vector<Expectation *>::iterator i = parallels.begin(); i != parallels.end(); i++)
			if ((*i)->debugging() == false)
				(*i)->debug();
	}

	void Parallel::assumeSize(unsigned int s)
	{
		size = (signed int)s;
	}

	Parallel &Parallel::append(Expectation &e)
	{
		vector<Expectation *>::iterator i = parallels.begin();
		for (; i != parallels.end(); i++)
		{
			if ((*i)->minLength() <= e.minLength())
			{
				if (i != parallels.begin())
					i--;
				break;
			}
		}

		parallels.insert(i, &e);
		return *this;
	}

	unsigned int Parallel::minLength()
	{
		if (size >= 0)
			return (unsigned int)size;

		unsigned int lowest = (unsigned int)-1;
		for (vector<Expectation *>::iterator i = parallels.begin(); i != parallels.end(); i++)
			if ((*i)->minLength() < lowest)
				lowest = (*i)->minLength();
		if (parallels.empty())
			lowest = 0;
		return lowest;
	}

	AST Parallel::parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, unsigned int stackc)
	{
		cout << pad(stackc) << "Parallel ";
		debugTokens(toks, off);

		vector<Error> ebuftmp;
		for (vector<Expectation *>::iterator i = parallels.begin(); i != parallels.end(); i++)
		{
			unsigned int offtmp = off;

			AST tmp = (*i)->parse(toks, offtmp, ebuftmp, stackc+1);
			if (tmp.good())
			{
				if (debugging())
					cout << pad(stackc) << "Expectation of Parallel met.\n";
				off = offtmp;
				debugAST(tmp, stackc);
				return tmp;
			}
		}

		if (debugging())
			cout << pad(stackc) << "Expectation of Parallel NOT met, erroring...\n";
		ebuf.insert(ebuf.end(), ebuftmp.begin(), ebuftmp.end());
		debugAST(AST(), stackc);
		return AST();
	}

	Many::Many(string n, Expectation &e)
	{
		name = n;
		one = &e;
	}

	void Many::debug()
	{
		_debug = true;
		if (one->debugging() == false)
			one->debug();
	}

	unsigned int Many::minLength()
	{
		if (one != NULL)
			return one->minLength();
		else
			return 0;
	}

	AST Many::parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, unsigned int stackc)
	{
		cout << pad(stackc) << "Many '" << name << "' ";
		debugTokens(toks, off);

		AST rtn = AST(name, toks[off]);
		while(1)
		{
			unsigned int offtmp = off;
			vector<Error> ebuftmp;
			AST tmp = one->parse(toks, offtmp, ebuftmp, stackc+1);
			if (ebuftmp.empty())
			{
				if (debugging())
					cout << pad(stackc) << "Expectation of Many '" << name << "' met, continuing...\n";
				rtn.add(tmp);
				off = offtmp;
			}
			else
			{
				if (debugging())
					cout << pad(stackc) << "Expectation of Many '" << name << "' NOT met, halting...\n";
				break;
			}
		}

		debugAST(rtn, stackc);
		return rtn;
	}

	Maybe::Maybe(Expectation &e)
	{
		expecting = &e;
	}

	void Maybe::debug()
	{
		_debug = true;
		if (expecting->debugging() == false)
			expecting->debug();
	}

	unsigned int Maybe::minLength()
	{
		return 0;
	}

	AST Maybe::parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, unsigned int stackc)
	{
		cout << pad(stackc) << "Maybe ";
		debugTokens(toks, off);

		unsigned offtmp = off;
		vector<Error> ebuftmp;
		AST tmp = expecting->parse(toks, offtmp, ebuftmp, stackc+1);
		if (tmp.good())
		{
			if (debugging())
				cout << pad(stackc) << "Expectating of Maybe met.\n";
			off = offtmp;
			ebuf.insert(ebuf.end(), ebuftmp.begin(), ebuftmp.end());
			debugAST(tmp, stackc);
			return tmp;
		}
		else
		{
			if (debugging())
				cout << pad(stackc) << "Expectation of Maybe NOT met.\n";
			debugAST(AST(), stackc);
			return AST();
		}
	}
}
