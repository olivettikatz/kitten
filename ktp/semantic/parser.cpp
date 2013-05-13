#include "parser.h"

namespace ktp
{
	Expectation &Expectation::identify(string i)
	{
		id = i;
		return *this;
	}

	Expectation &Expectation::operator || (Expectation e)
	{
		alternates.push_back(e.id);
		return *this;
	}

	Expectation &Expectation::operator << (Expectation e)
	{
		sequence.push_back(e.id);
		return *this;
	}

	Expectation &Expectation::keep()
	{
		_keep = true;
		return *this;
	}

	Expectation &Expectation::many()
	{
		_many = true;
		return *this;
	}

	string Expectation::getID()
	{
		return id;
	}

	string Expectation::getExpectation()
	{
		return expectation;
	}

	bool Expectation::getKeep()
	{
		return _keep;
	}

	bool Expectation::getMany()
	{
		return _many;
	}

	vector<string> Expectation::getSequence()
	{
		return sequence;
	}

	vector<string> Expectation::getAlternates()
	{
		return alternates;
	}

	Expectation Parser::operator [] (string e)
	{
		return Expectation(e);
	}

	Expectation Parser::operator () ()
	{
		return Expectation();
	}

	Expectation Parser::operator () (string n, string e)
	{
		Expectation rtn = Expectation(e);
		rtn.identify(n);
		content[n] = rtn;
		return rtn;
	}

	Expectation Parser::operator () (string n)
	{
		if (content.find(n) != content.end())
			return content[n];
		else
			return Expectation(n);
	}

	Parser Parser::add(Expectation e)
	{
		content[e.getID()] = e;
		return *this;
	}

	Parser Parser::add(string n, Expectation e)
	{
		e.identify(n);
		content[n] = e;
		return *this;
	}

	Parser Parser::many(string n)
	{
		if (content.find(n) != content.end())
			content[n].many();
		return *this;
	}

	AST Parser::parse(string n, vector<Token> toks, unsigned int &off, vector<Error> &ebuf)
	{
		AST rtn = AST();

		if (toks[off].getType().compare(content[n].getExpectation()) != 0 && content[n].getExpectation().compare(""))
		{
			cout << "expectation " << n << " failed.\n";
			cout << "\tchecking alternates...\n";
			for (vector<string>::iterator i = content[n].getAlternates().begin(); i != content[n].getAlternates().end(); i++)
			{
				unsigned int oldoff = off;
				vector<Error> ebuftmp;
				rtn = parse(*i, toks, off, ebuftmp);
				if (rtn.good())
				{
					cout << "\tfound alternate: " << *i << "!\n";
					ebuf.insert(ebuf.end(), ebuftmp.begin(), ebuftmp.end());
					break;
				}
				else
				{
					off = oldoff;
				}
			}

			if (rtn.good() == false)
			{
				cout << "\texpectation un-matchable, erroring.\n";
				ebuf.push_back(Error(toks[off], content[n].getExpectation(), n));
				return AST();
			}
		}
		else if (content[n].getKeep())
		{
			cout << "expectation " << n << " matched and kept!\n";
			rtn = AST(n, toks[off]);
		}

		off++;

		for (vector<string>::iterator i = content[n].getSequence().begin(); i != content[n].getSequence().end(); i++)
		{
			cout << "\tadding " << *i << " to sequence...\n";
			rtn.add(parse(*i, toks, off, ebuf));
		}

		if (content[n].getMany())
		{
			cout << "\texpecting many repetitions...\n";
			AST tmp = AST();
			tmp.add(rtn);

			while(rtn.good() == false)
			{
				vector<Error> ebuftmp;
				rtn = parse(n, toks, off, ebuftmp);
				if (rtn.good())
					break;
				else
				{
					ebuf.insert(ebuf.end(), ebuftmp.begin(), ebuftmp.end());
					tmp.add(rtn);
				}
			}

			return tmp;
		}

		return rtn;
	}
}

