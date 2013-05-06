#include "parser.h"

namespace taurus
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

	Expectation &Expectation::operator , (Expectation e)
	{
		sequence.push_back(e.id);
		return *this;
	}

	Expectation &Expectation::operator ++ ()
	{
		many = true;
		return *this;
	}

	Expectation &Expectation::keep()
	{
		keep = true;
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
		return keep;
	}

	bool Expectation::getMany()
	{
		return many;
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
			return Expectation(e);
	}

	Parser &Parser::add(Expectation e)
	{
		content[e.getID()] = e;
		return *this;
	}

	AST Parser::parse(string n, vector<Token> toks, unsigned int &off, vector<Error> &ebuf)
	{
		AST rtn = AST();

		if (toks[off].getType() != content[n].getExpectation() && content[n].getExpectation() != 0)
		{
			for (vector<string>::iterator i = content[n].getAlternates().begin(); i != content[n].getAlternates().end(); i++)
			{
				unsigned int oldoff = off;
				vector<Error> ebuftmp;
				rtn = parse(*i, toks, off, ebuftmp);
				if (rtn.good())
				{
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
				ebuf.push_back(Error(toks[off], content[n].getExpectation(), n));
			}
		}
		else if (content[n].getKeep())
		{
			rtn = AST(n, toks[off]);
		}

		off++;

		for (vector<string>::iterator i = content[n].getSequence().begin(); i != content[n].getSequence().end(); i++)
		{
			rtn.add(parse(*i, toks, off, ebuf));
		}

		if (content[n].getMany())
		{
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

