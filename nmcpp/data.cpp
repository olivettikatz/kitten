#include "data.h"

namespace nmc
{
	string readEncoded(string path, encoding enc)
	{
		ifstream f(path.c_str());
		string page((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());

		if (enc == UTF16)
		{
			string tmp;
			for (int i = 0; i < page.size()*0.5; i++)
			{
				short c = 0;
				c |= page[i*2]<<8;
				c |= page[i*2+1];
				tmp += (char)c;
			}
			page = tmp;
		}
		else if (enc == UTF32)
		{
			string tmp;
			for (int i = 0; i < page.size()*0.25; i++)
			{
				int c = 0;
				c |= page[i*4]<<24;
				c |= page[i*4+1]<<16;
				c |= page[i*4+2]<<8;
				c |= page[i*4+3];
				tmp += (char)c;
			}
		}

		return page;
	}

	Token &Token::setType(Token::category t)
	{
		cat = t;
		return *this;
	}

	Token::category Token::getType()
	{
		return cat;
	}

	string Token::getString()
	{
		return content;
	}

	string Token::setString(string s)
	{
		return (content = s);
	}

	unsigned int Token::getLine()
	{
		return line;
	}

	unsigned int Token::getCol()
	{
		return col;
	}

	bool Token::operator == (const char *s)
	{
		return (content.compare(string(s)) == 0);
	}

	bool Token::operator == (string s)
	{
		return (content.compare(s) == 0);
	}

	bool Token::operator != (const char *s)
	{
		return (content.compare(string(s)) != 0);
	}

	bool Token::operator != (string s)
	{
		return (content.compare(s) != 0);
	}

	bool Token::contains(string only)
	{
		for (string::iterator i = content.begin(); i != content.end(); i++)
		{
			if (only.find(*i) == string::npos)
			{
				return false;
			}
		}

		return true;
	}

	string Token::display()
	{
		stringstream ss;
		ss << "\"" << content << "\"?";
		if (cat == category::value)
			ss << "value";
		else if (cat == category::operation)
			ss << "operation";
		else if (cat == category::boundary)
			ss << "boundary";
		else if (cat == category::separator)
			ss << "separator";
		else if (cat == category::type)
			ss << "type";
		else if (cat == category::eol)
			ss << "eol";
		else if (cat == category::symbol)
			ss << "symbol";
		ss << "@" << line << ":" << col;
		return ss.str();
	}
}
