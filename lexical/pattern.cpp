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

	Pattern &Pattern::operator == (string s)
	{
		*this = Pattern(isEqualTo, s);
		return *this;
	}

	Pattern &Pattern::operator != (string s)
	{
		*this = Pattern(isNotEqualTo, s);
		return *this;
	}

	Pattern &Pattern::operator += (string s)
	{
		*this = Pattern(onlyContains, s);
		return *this;
	}

	Pattern &Pattern::operator -= (string s)
	{
		*this = Pattern(doesNotContain, s);
		return *this;
	}

	Pattern &Pattern::operator < (string s)
	{
		*this = Pattern(startsWith, s);
		return *this;
	}

	Pattern &Pattern::operator > (string s)
	{
		*this = Pattern(endsWith, s);
		return *this;
	}

	Pattern &Pattern::operator () (Pattern::callback a)
	{
		*this = Pattern(a);
		return *this;
	}

	Pattern &Pattern::operator && (Pattern other)
	{
		return attach(attachAnd, other);
	}

	Pattern &Pattern::operator || (Pattern other)
	{
		return attach(attachOr, other);
	}

	bool Pattern::compare(Pattern other)
	{
		if (algorithm != other.algorithm)
			return false;

		if (argument.compare(other.argument) != 0)
			return false;

		if (alternateCallback != other.alternateCallback)
			return false;

		if (andPatterns.size() != other.andPatterns.size())
			return false;

		for (int i = 0; i < andPatterns.size(); i++)
			if (andPatterns[i].compare(other.andPatterns[i]) == false)
				return false;
		
		if (orPatterns.size() != other.orPatterns.size())
			return false;

		for (int i = 0; i < orPatterns.size(); i++)
			if (!orPatterns[i].compare(other.orPatterns[i]))
				return false;

		if (attachments.size() != other.attachments.size())
			return false;

		for (int i = 0; i < attachments.size(); i++)
			if ((attachments[i].first != other.attachments[i].first) || !attachments[i].second.compare(other.attachments[i].second))
				return false;

		return true;
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

