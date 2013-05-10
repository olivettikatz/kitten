#include "expectation.h"

namespace taurus
{
	bool AST::good()
	{
		return (eid > 0);
	}

	bool AST::empty()
	{
		return content.empty();
	}

	AST &AST::add(AST a)
	{
		children.push_back(a);
		return *this;
	}

	unsigned int AST::getExpectationID()
	{
		return eid;
	}

	Token AST::getContent()
	{
		return content;
	}

	unsigned int AST::getChildrenSize()
	{
		return children.size();
	}

	AST AST::getChild(unsigned int idx)
	{
		return children[idx];
	}

	Expectation &Expectation::enableKeep()
	{
		keep = true;
		return *this;
	}

	Expectation &Expectation::disableKeep()
	{
		keep = false;
		return *this;
	}

	Expectation &Expectation::addToSequence(Expectation e)
	{
		_sequence.push_back(e);
		return *this;
	}

	Expectation &Expectation::addOr(Expectation e)
	{
		_or.push_back(e);
		return *this;
	}

	AST Expectation::expect(vector<Token> toks, unsigned int &off, vector<Error> &ebuf)
	{
		AST rtn = AST();
		
		if (toks[off].getType() != type && type != 0)
		{
			for (vector<Expectation>::iterator i = _or.begin(); i != _or.end(); i++)
			{
				unsigned int oldoff = off;
				rtn = i->expect(toks, off, ebuf);
				if (rtn.good())
				{
					break;
				}
				else
				{
					off = oldoff;
				}
			}

			if (rtn.good() == false)
			{
				ebuf.push_back(Error(toks[off], type, id));
				return AST();
			}
		}
		else if (keep)
		{
			rtn = AST(id, toks[off]);
		}

		off++;

		for (vector<Expectation>::iterator i = _sequence.begin(); i != _sequence.end(); i++)
		{
			rtn.add(i->expect(toks, off, ebuf));
		}

		return rtn;
	}
}

