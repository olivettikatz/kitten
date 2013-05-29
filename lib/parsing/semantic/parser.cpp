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

	void Expectation::debugInput(vector<Token> toks, unsigned int off, unsigned int stackc)
	{
		if (_debug)
		{
			cout << pad(stackc) << "Parsing (" << _class << " '" << _name << "'):";
			for (unsigned int i = off; i < off+10 && i < toks.size(); i++)
				cout << " '\033[0;32m" << toks[i].get() << "\033[0;0m'";
			cout << "\n";
		}
	}

	void Expectation::debugOutput(AST rtn, unsigned int stackc)
	{
		if (_debug)
			cout << pad(stackc) << "Returning (" << _class << " '" << _name << "'): \033[0;33m" << rtn.display() << "\033[0;0m\n";
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

	AST Expectation::parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, vector<Expectation *> stack)
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

	AST One::parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, vector<Expectation *> stack)
	{
		debugInput(toks, off, stack.size());
		stack.push_back(this);

		if (toks[off].getType().compare(expecting) == 0)
		{
			off++;
			if (off >= toks.size())
			{
				if (debugging())
					cout << pad(stack.size()) << "Expectation of One '" << expecting << "' overflowed token vector, erroring...\n";

				ebuf.push_back(Error(toks[off-1], expecting, expecting));
				debugOutput(AST(), stack.size());
				return AST();
			}

			if (_keep)
			{
				if (debugging())
					cout << pad(stack.size()) << "Expectation of One '" << expecting << "' met, keeping...\n";
				debugOutput(AST(expecting, toks[off]), stack.size());
				return AST(expecting, toks[off]);
			}
			else
			{
				if (debugging())
					cout << pad(stack.size()) << "Expectation of One '" << expecting << "' met.\n";
				debugOutput(AST(), stack.size());
				return AST();
			}
		}
		else
		{
			if (debugging())
				cout << pad(stack.size()) << "Expectation of One '" << expecting << "' NOT met, erroring...\n";
			ebuf.push_back(Error(toks[off], expecting, expecting));
			debugOutput(AST(), stack.size());
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

	AST Sequence::parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, vector<Expectation *> stack)
	{
		debugInput(toks, off, stack.size());

		AST rtn = AST(getName(), toks[off]);
		for (vector<Expectation *>::iterator i = sequence.begin(); i != sequence.end(); i++)
		{
			stack.push_back(this);
			vector<Error> ebuftmp;
			AST tmp = (*i)->parse(toks, off, ebuftmp, stack);
			if (ebuftmp.empty() == false)
			{
				ebuf.insert(ebuf.end(), ebuftmp.begin(), ebuftmp.end());
				if (debugging())
					cout << pad(stack.size()) << "Expectation of Sequence '" << getName() << "' NOT met (because of '" << (*i)->getName() << "'), erroring...\n";
				if (off < toks.size())
					ebuf.push_back(Error(toks[off], getName(), getName()));
				debugOutput(AST(), stack.size());
				return AST();
			}
			else
			{
				if (debugging())
					cout << pad(stack.size()) << "Expectation of Sequence '" << getName() << "' met.\n";
			}
		}

		debugOutput(rtn, stack.size());
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

	AST Parallel::parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, vector<Expectation *> stack)
	{
		debugInput(toks, off, stack.size());

		vector<Error> ebuftmp;

		unsigned int lastParallel = 0;
		for (unsigned int i = stack.size()-1; i > 0; i--)
		{
			if (stack[i] == this)
			{
				lastParallel = i;
				break;
			}
		}
		
		if (lastParallel < stack.size())
			lastParallel++;

		for (unsigned int i = 0; i < parallels.size(); i++)
		{
			if (parallels[i] == stack[lastParallel])
				continue;

			unsigned int offtmp = off;

			stack.push_back(this);
			AST tmp = parallels[i]->parse(toks, offtmp, ebuftmp, stack);
			if (tmp.good())
			{
				if (debugging())
					cout << pad(stack.size()) << "Expectation of Parallel met.\n";
				off = offtmp;
				debugOutput(tmp, stack.size());
				return tmp;
			}
		}

		if (debugging())
			cout << pad(stack.size()) << "Expectation of Parallel NOT met, erroring...\n";
		ebuf.insert(ebuf.end(), ebuftmp.begin(), ebuftmp.end());
		debugOutput(AST(), stack.size());
		return AST();
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

	AST Many::parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, vector<Expectation *> stack)
	{
		debugInput(toks, off, stack.size());

		AST rtn = AST(getName(), toks[off]);
		while(1)
		{
			unsigned int offtmp = off;
			vector<Error> ebuftmp;
			stack.push_back(this);
			AST tmp = one->parse(toks, offtmp, ebuftmp, stack);
			if (ebuftmp.empty())
			{
				if (debugging())
					cout << pad(stack.size()) << "Expectation of Many '" << getName() << "' met, continuing...\n";
				rtn.add(tmp);
				off = offtmp;
			}
			else
			{
				if (debugging())
					cout << pad(stack.size()) << "Expectation of Many '" << getName() << "' NOT met, halting...\n";
				break;
			}
		}

		debugOutput(rtn, stack.size());
		return rtn;
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

	AST Maybe::parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf, vector<Expectation *> stack)
	{
		debugInput(toks, off, stack.size());

		unsigned offtmp = off;
		vector<Error> ebuftmp;
		stack.push_back(this);
		AST tmp = expecting->parse(toks, offtmp, ebuftmp, stack);
		if (tmp.good())
		{
			if (debugging())
				cout << pad(stack.size()) << "Expectating of Maybe met.\n";
			off = offtmp;
			ebuf.insert(ebuf.end(), ebuftmp.begin(), ebuftmp.end());
			debugOutput(tmp, stack.size());
			return tmp;
		}
		else
		{
			if (debugging())
				cout << pad(stack.size()) << "Expectation of Maybe NOT met.\n";
			debugOutput(AST(), stack.size());
			return AST();
		}
	}
}
