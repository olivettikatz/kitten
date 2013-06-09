#include "extract.h"

namespace extract
{
	string Extractor::trimWhitespace(string s)
	{
		while(s.empty() == false && (s[0] == '\n' || s[0] == ' '))
			s = s.substr(1);
		while(s.empty() == false && (s.back() == '\n' || s.back() == ' '))
			s = s.substr(0, s.size()-1);
		return s;
	}

	void Extractor::ignore(string from, string to)
	{
		pair<string, string> tmp;
		tmp.first = from;
		tmp.second = to;
		ignores.push_back(tmp);
	}

	void Extractor::extract(string from, string to)
	{
		extractStart = from;
		extractEnd = to;
	}

	void Extractor::attach(string enable, string disable)
	{
		enableAttachment = enable;
		disableAttachment = disable;
	}

	vector<pair<Extractor::segmentType, string> > Extractor::parse(string page)
	{
		bool attachment = false;
		unsigned int start = 0;
		unsigned int end = 0;
		unsigned int attach = 0;

		vector<pair<segmentType, string> > rtn;

		for (unsigned int i = 0; i < page.size(); i++)
		{
			for (vector<pair<string, string> >::iterator j = ignores.begin(); j < ignores.end(); j++)
			{
				if (page.compare(i, j->first.size(), j->first) == 0)
				{
					while(page.compare(i++, j->second.size(), j->second) != 0);
					i += j->second.size();
					if (i >= page.size())
						break;
				}
			}

			if (page.compare(i, enableAttachment.size(), enableAttachment) == 0)
			{
				attachment = true;
				i += enableAttachment.size();
				attach = i;
				if (i >= page.size())
					break;
			}
			else if (page.compare(i, disableAttachment.size(), disableAttachment) == 0)
			{
				pair<segmentType, string> tmp;
				tmp.first = attached;
				tmp.second = trimWhitespace(page.substr(attach, i-attach));
				rtn.push_back(tmp);

				attachment = false;
				i += disableAttachment.size();
				if (i >= page.size())
					break;
			}
			else if (page.compare(i, extractStart.size(), extractStart) == 0)
			{
				i += extractStart.size();
				start = i;

				if (start >= page.size())
					break;

				if (attachment)
				{
					pair<segmentType, string> tmp;
					tmp.first = attached;
					tmp.second = trimWhitespace(page.substr(end, i-end));
					rtn.push_back(tmp);
				}
			}
			else if (page.compare(i, extractEnd.size(), extractEnd) == 0)
			{
				end = i;

				pair<segmentType, string> tmp;
				tmp.first = extraction;
				tmp.second = trimWhitespace(page.substr(start, i-start));
				rtn.push_back(tmp);
			}
		}

		return rtn;
	}

	string readFile(string path)
	{
		ifstream f(path.c_str());
		string rtn((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
		return rtn;
	}
}
