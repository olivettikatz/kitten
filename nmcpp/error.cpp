#include "error.h"

namespace nmc
{
	string currentFile;
	string currentPage;
	stringstream errorBuffer;
	unsigned int errorCount;
	vector<string> lines;
	bool enableDeveloperInfo = false;

	void initErrors(string f, string p)
	{
		currentFile = f;
		currentPage = p;
		errorBuffer.str("");
		errorCount = 0;

		lines.clear();
		string buf;
		for (string::iterator i = p.begin(); i != p.end(); i++)
		{
			if (*i == '\n')
			{
				lines.push_back(buf);
			}
			else
			{
				buf += *i;
			}
		}
		if (!buf.empty())
			lines.push_back(buf);
	}

	void dumpErrors()
	{
		cout << errorBuffer.str() << "\n";
		if (errorCount == 1)
			cout << "* \033[0;31m1\033[0;0m error\n";
		else
			cout << "* \033[0;31m" << errorCount << "\033[0;0m errors\n";
		errorBuffer.str("");
		errorCount = 0;
	}

	void dieIfErrors()
	{
		if (errorCount > 0)
		{
			dumpErrors();
			_exit(1);
		}
	}
}
