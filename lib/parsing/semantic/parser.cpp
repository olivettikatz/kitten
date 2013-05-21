#include "parser.h"

namespace parsing
{
	Expectation &Expectation::identify(string i)
	{
		id = i;
		return *this;
	}

	Expectation &Expectation::operator || (Expectation e)
	{
		vector<pair<unsigned int, string> >::iterator i;
		for (i = alternates.begin(); i != alternates.end(); i++)
		{
			if (i->first <= e.sequence.size())
			{
				if (i != alternates.begin())
					i--;
				break;
			}
		}

		pair<unsigned int, string> tmp;
		tmp.first = e.sequence.size();
		tmp.second = e.id;
		alternates.insert(i, tmp);

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
		vector<string> tmp;
		for (vector<pair<unsigned int, string> >::iterator i = alternates.begin(); i != alternates.end(); i++)
			tmp.push_back(i->second);
		return tmp;
	}

	string Expectation::display()
	{
		stringstream ss;
		ss << "'" << id << "' => '" << expectation << "'";
		if (_keep)
			ss << " keep";
		if (_many)
			ss << " many";
		if (!sequence.empty())
		{
			ss << "\n\t";
			for (vector<string>::iterator i = sequence.begin(); i != sequence.end(); i++)
				ss << " << '" << *i << "'";
		}
		if (!alternates.empty())
		{
			ss << "\n\t";
			for (vector<pair<unsigned int, string> >::iterator i = alternates.begin(); i != alternates.end(); i++)
				ss << " || '" << i->second << "'";
		}

		return ss.str();
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
		{
			content[n] = Expectation(n).identify(n);
			cout << content[n].display() << "\n";
			return content[n];
		}
	}

	Expectation &Parser::preadd(string n)
	{
		content[n] = Expectation().identify(n);
		return content[n];
	}

	Expectation &Parser::add(string n, Expectation e)
	{
		content[n] = e.identify(n);
		cout << content[n].display() << "\n";
		return content[n];
	}

	Parser Parser::many(string n)
	{
		if (content.find(n) != content.end())
			content[n].many();
		return *this;
	}

	AST Parser::parse(string n, vector<Token> toks, unsigned int &off, vector<Error> &ebuf)
	{
		cout << "parsing (with '" << n << "'):";
		for (unsigned int i = off; i < toks.size(); i++)
			cout << " '" << toks[i].get() << "'";
		cout << "\n";
		AST rtn = AST();

		if (toks[off].getType().compare(content[n].getExpectation()) != 0 && content[n].getExpectation().compare("") != 0)
		{
			cout << "expectation " << n << " failed ('" << toks[off].get() << "' type '" << toks[off].getType() << "' != '" << content[n].getExpectation() << "')\n";
			cout << "\tchecking alternates...\n";
			for (vector<string>::iterator i = content[n].getAlternates().begin(); i != content[n].getAlternates().end(); i++)
			{
				cout << "\t\tchecking '" << *i << "'...\n";
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
			cout << "\texpectation " << n << " matched and kept!\n";
			rtn = AST(n, toks[off]);
		}
		else
		{
			cout << "\texpectation " << n << " matched ('" << toks[off].get() << "' type '" << toks[off].getType() << "' == '" << content[n].getExpectation() << "')\n";
		}

		off++;

		for (unsigned int i = 0; i < content[n].getSequence().size(); i++)
		{
			cout << "\tadding '" << content[n].getSequence()[i] << "' to '" << n << "'s sequence...\n";
			rtn.add(parse(content[n].getSequence()[i], toks, off, ebuf));
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

