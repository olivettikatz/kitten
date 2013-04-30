#include "pattern.h"

namespace taurus
{
	Pattern &Pattern::setAlgorithm(Pattern::algorithmType a)
	{
		algorithm = a;
		return *this;
	}

	Pattern &Pattern::setArgument(string a)
	{
		argument = a;
		return *this;
	}

	Pattern &Pattern::setAlternate(Pattern::callback a)
	{
		alternateCallback = a;
		return *this;
	}

	Pattern &Pattern::attach(Pattern::attachmentType t, Pattern p)
	{
		pair<attachmentType, Pattern> tmp;
		tmp.first = t;
		tmp.second = p;
		attachments.push_back(tmp);
		return *this;
	}

	unsigned int Pattern::getLengthRequest()
	{
		if (algorithm == isEqualTo || algorithm == startsWith || algorithm == endsWith)
			return argument.size();
		else
			return 0;
	}

	bool Pattern::match(string s)
	{
		bool rtn = false;

		if (algorithm == isEqualTo)
			rtn = (argument.compare(s) == 0);
		else if (algorithm == isNotEqualTo)
			rtn = (argument.compare(s) != 0);
		else if (algorithm == onlyContains)
		{
			rtn = true;
			for (string::iterator i = s.begin(); i != s.end(); i++)
			{
				if (argument.find(*i) == string::npos)
				{
					rtn = false;
					break;
				}
			}
		}
		else if (algorithm == doesNotContain)
		{
			rtn = true;
			for (string::iterator i = s.begin(); i != s.end(); i++)
			{
				if (argument.find(*i) != string::npos)
				{
					rtn = false;
					break;
				}
			}
		}
		else if (algorithm == startsWith)
			rtn = (s.find(argument) == 0);
		else if (algorithm == endsWith)
			rtn = (s.rfind(argument) == s.size()-argument.size());
		else if (algorithm == alternate)
			rtn = alternateCallback(s);

		for (vector<pair<attachmentType, Pattern> >::iterator i = attachments.begin(); i != attachments.end(); i++)
		{
			if (i->first == attachAnd)
				rtn = (rtn && i->second.match(s));
			else if (i->first == attachOr)
				rtn = (rtn || i->second.match(s));
			else if (i->first == attachAndNot)
				rtn = (rtn && i->second.match(s) == false);
			else if (i->first == attachOrNot)
				rtn = (rtn || i->second.match(s) == false);
		}

		return rtn;
	}
	
	string digits = "0123456789";
	string lettersLower = "abcdefghijklmnopqrstuvwxyz";
	string lettersUpper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
}

