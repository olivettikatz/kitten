#include "args.h"

namespace args
{
	char Argument::getShorthand()
	{
		return shorthand;
	}

	string Argument::getFull()
	{
		return full;
	}

	string Argument::getHelp()
	{
		return help;
	}

	bool Argument::compare(string arg)
	{
		string tmp = arg;
		if (tmp.find("=") != string::npos)
			tmp = tmp.substr(0, tmp.find("="));

		if (tmp.compare("-"+shorthand) == 0)
			return true;
		else if (tmp.compare("--"+full) == 0)
			return true;
		else
			return false;
	}

	void ArgumentParser::dumpHelp()
	{
		cout << name << "\n";
		cout << " written by " << author << "\n";
		cout << " last built on " << __DATE__ << " at " << __TIME__ << "\n";
		cout << " using of Kitten 0.01 alpha\n";
		cout << " on version " << version << "\n\n";

		cout << "usage: " << usage << "\n\n";

		cout << desc << "\n\n";

		cout << "  --help  display this help page\n";
		cout << "  --version  display this help page (which contains version info)\n";

		for (vector<Argument>::iterator i = args.begin(); i != args.end(); i++)
		{
			cout << "  --" << i->getFull();
			if (i->getShorthand() != 0)
				cout << ", -" << i->getShorthand();
			cout << "  " << i->getHelp() << "\n";
		}
	}

	void ArgumentParser::setName(string n)
	{
		name = n;
	}

	void ArgumentParser::setAuthor(string a)
	{
		author = a;
	}

	void ArgumentParser::setVersion(string v)
	{
		version = v;
	}

	void ArgumentParser::setUsage(string u)
	{
		usage = u;
	}

	void ArgumentParser::setDescription(string d)
	{
		desc = d;
	}

	void ArgumentParser::operator << (Argument a)
	{
		args.push_back(a);
	}

	void ArgumentParser::parse(int argc, char *argv[])
	{
		for (int i = 1; i < argc; i++)
		{
			if (argv[i][0] != '-')
			{
				free.push_back(argv[i]);
				continue;
			}
			else if (string(argv[i]).compare("--help") == 0 || string(argv[i]).compare("--version") == 0)
			{
				dumpHelp();
				_exit(0);
			}
			
			for (vector<Argument>::iterator j = args.begin(); j != args.end(); j++)
			{
				if (j->compare(argv[i]))
				{
					flags[j->getFull()] = true;
					if (string(argv[i]).find("=") != string::npos)
					{
						data[j->getFull()] = string(argv[i]).substr(string(argv[i]).find("=")+1);
					}
					break;
				}
			}
		}
	}

	bool ArgumentParser::getFlag(string f)
	{
		return flags[f];
	}

	string ArgumentParser::getValue(string k)
	{
		return data[k];
	}

	vector<string> ArgumentParser::getFreeValues()
	{
		return free;
	}
}
