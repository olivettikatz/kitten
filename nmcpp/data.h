#ifndef __NMC_DATA_H
#define __NMC_DATA_H

#include <iostream>
#include <vector>
#include <fstream>
#include <streambuf>
#include <string>
#include <sstream>

namespace nmc
{
	using namespace std;

	typedef enum
	{
		ASCII,
		UTF8,
		UTF16,
		UTF32
	} encoding;

	string readEncoded(string path, encoding enc);

	class Token
	{
	public:
		typedef enum
		{
			value,
			operation,
			boundary,
			separator,
			type,
			eol,
			symbol
		} category;

	private:
		string content;
		unsigned int line, col;
		category cat;

	public:
		Token() : line(0), col(0), cat(symbol) {}
		Token(string s) : content(s), line(0), col(0), cat(symbol) {}
		Token(string s, unsigned int l, unsigned int c) : content(s), line(l), col(c), cat(symbol) {}
		Token &setType(category t);
		category getType();
		string getString();
		string setString(string s);
		unsigned int getLine();
		unsigned int getCol();
		bool operator == (const char *s);
		bool operator == (string s);
		bool operator != (const char *s);
		bool operator != (string s);
		bool contains(string only);
		string display();
	};

	template<typename T> string displayMulti(string n, vector<T> v)
	{
		return display(n, v, 0);
	}

	template<typename T> string displayMulti(string n, vector<T> v, int o)
	{
		stringstream ss;
		for (int i = o; i < v.size(); i++)
			ss << n << "[" << i << "]: " << v[i].display() << "\n";
		return ss.str();
	}
}

#endif
