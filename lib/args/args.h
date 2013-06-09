#ifndef __ARGS_H
#define __ARGS_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <unistd.h>

namespace args
{
	using namespace std;

	class Argument
	{
	private:
		char shorthand;
		string full;
		string help;

	public:
		Argument() {}
		Argument(string f) : full(f), shorthand(0) {}
		Argument(char s, string f) : shorthand(s), full(f) {}
		Argument(char s, string f, string h) : shorthand(s), full(f), help(h) {}
		char getShorthand();
		string getFull();
		string getHelp();
		bool compare(string arg);
	};

	class ArgumentParser
	{
	private:
		string name;
		string author;
		string version;
		string usage;
		string desc;
		vector<Argument> args;

		map<string, bool> flags;
		map<string, string> data;
		vector<string> free;

		void dumpHelp();
	
	public:
		ArgumentParser() {}
		void setName(string n);
		void setAuthor(string a);
		void setVersion(string v);
		void setUsage(string u);
		void setDescription(string d);
		void operator << (Argument a);
		void parse(int argc, char *argv[]);
		bool getFlag(string f);
		string getValue(string k);
		vector<string> getFreeValues();
	};
}

#endif
