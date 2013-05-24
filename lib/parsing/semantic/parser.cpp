#include "parser.h"

namespace parsing
{
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

	AST One::parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf)
	{
		if (toks[off].getType().compare(expecting) == 0)
		{
			off++;
			if (_keep)
				return AST(expecting, toks[off]);
			else
				return AST();
		}
		else
		{
			ebuf.push_back(Error(toks[off], expecting, expecting));
			return AST();
		}
	}

	Sequence &Sequence::append(Expectation &e)
	{
		sequence.push_back(&e);
		return *this;
	}

	unsigned int &Sequence::minLength()
	{
		unsigned int rtn = 0;
		for (vector<shared_ptr<Expectation> >::iterator i = sequence.begin(); i != sequence.end(); i++)
			rtn += (*i)->minLength();
		return rtn;
	}

	AST Sequence::parse(vector<Token> toks, unsigned int &off, vector<Error> &ebuf)
	{
		for (vector<shared_ptr<Expectation>
	}

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

		vector<Error> ebuftmp;

		if (toks[off].getType().compare(content[n].getExpectation()) != 0)
		{
			cout << "\texpectation " << n << " failed ('" << toks[off].get() << "' type '" << toks[off].getType() << "' != '" << content[n].getExpectation() << "')\n";
			ebuftmp.push_back(Error(toks[off], content[n].getExpectation(), n));
		}
		else
		{
			if (content[n].getKeep())
			{
				rtn = AST(n, toks[off]);
			}
		}

		if (content[n].getExpectation().compare("") != 0)
		{
			off++;
		}

		for (unsigned int i = 0; i < content[n].getSequence().size(); i++)
		{
			rtn.add(parse(content[n].getSequence()[i], toks, off, ebuftmp));
		}

		if (ebuftmp.empty() == false)
		{
			for (unsigned int i = 0; i < content[n].getAlternates().size(); i++)
			{
				cout << "\tchecking alternate '" << content[n].getAlternates()[i] << "'...\n";
				unsigned int oldoff = off;
				ebuftmp.clear();
				rtn = parse(content[n].getAlternates()[i], toks, off, ebuftmp);
				if (rtn.good())
				{
					cout << "\tfound alternate: '" << content[n].getAlternates()[i] << "'!\n";
					break;
				}
			}

			if (rtn.good() == false)
			{
				cout << "\texpectation '" << n << "' unmatchable...\n";
				ebuftmp.push_back(Error(toks[off], content[n].getExpectation(), n));
				return AST();
			}
		}

		ebuf.insert(ebuf.end(), ebuftmp.begin(), ebuftmp.end());

		if (content[n].getMany())
		{
			AST tmp = AST();

			while(rtn.good())
			{
				cout << "\texpecting another '" << n << "'...\n";
				tmp.add(rtn);
				ebuftmp.clear();
				rtn = parse(n, toks, off, ebuftmp);
				if (rtn.empty() && ebuftmp.empty() == false)
				{
					ebuf.insert(ebuf.end(), ebuftmp.begin(), ebuftmp.end());
					break;
				}
			}

			return tmp;
		}

		return rtn;
	}
}

