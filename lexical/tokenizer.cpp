#include "tokenizer.h"

namespace taurus
{
	Token &Token::setContent(string s)
	{
		content = s;
		return *this;
	}

	Token &Token::setLine(unsigned int l)
	{
		line = l;
		return *this;
	}

	Token &Token::setColumn(unsigned int c)
	{
		column = c;
		return *this;
	}

	Token &Token::setType(unsigned int t)
	{
		type = t;
		return *this;
	}

	Token &Token::setFile(string f)
	{
		file = f;
		return *this;
	}

	bool Token::empty()
	{
		return content.empty();
	}

	string Token::get()
	{
		return content;
	}

	string Token::getContent()
	{
		return content;
	}

	unsigned int Token::getLine()
	{
		return line;
	}

	unsigned int Token::getColumn()
	{
		return column;
	}

	unsigned int Token::getType()
	{
		return type;
	}

	string Token::getFile()
	{
		return file;
	}

	bool Token::operator == (Token other)
	{
		return (content.compare(other.content) == 0);
	}

	bool Token::operator == (string other)
	{
		return (content.compare(other) == 0);
	}

	bool Token::operator == (unsigned int other)
	{
		return (type == other);
	}

	bool Token::operator != (Token other)
	{
		return (content.compare(other.content) != 0);
	}

	bool Token::operator != (string other)
	{
		return (content.compare(other) != 0);
	}

	bool Token::operator != (unsigned int other)
	{
		return (type != other);
	}

	void Tokenizer::appendPatternToVectorSorted(vector<Pattern> &v, Pattern p)
	{
		unsigned int idx = 0;
		for (; idx < v.size(); idx++)
		{
			if (v[idx].getLengthRequest() > p.getLengthRequest())
			{
				break;
			}
		}
		v.insert(v.begin()+idx, p);
	}

	unsigned int Tokenizer::fitsPatternVector(string s, vector<Pattern> v)
	{
		for (vector<Pattern>::iterator i = v.begin(); i != v.end(); i++)
		{
			unsigned int n = i->match(s);
			if (n > 0)
			{
				return n;
			}
		}

		return 0;
	}

	unsigned int Tokenizer::isNoDelimStart(string s)
	{
		return fitsPatternVector(s, noDelimStart);
	}

	unsigned int Tokenizer::isNoDelimEnd(string s)
	{
		return fitsPatternVector(s, noDelimEnd);
	}

	unsigned int Tokenizer::isSkipStart(string s)
	{
		return fitsPatternVector(s, skipStart);
	}

	unsigned int Tokenizer::isSkipEnd(string s)
	{
		return fitsPatternVector(s, skipEnd);
	}

	unsigned int Tokenizer::isWhitespace(string s)
	{
		return fitsPatternVector(s, whitespace);
	}

	unsigned int Tokenizer::isDeliminator(string s)
	{
		return fitsPatternVector(s, deliminator);
	}

	unsigned int Tokenizer::categorize(Token t)
	{
		for (vector<pair<unsigned int, Pattern> >::iterator i = categorizers.begin(); i != categorizers.end(); i++)
		{
			if (i->second.match(t.get()))
			{
				return i->first;
			}
		}

		return 0;
	}

	Tokenizer &Tokenizer::addNoDelimStart(Pattern p)
	{
		appendPatternToVectorSorted(noDelimStart, p);
		return *this;
	}

	Tokenizer &Tokenizer::addNoDelimEnd(Pattern p)
	{
		appendPatternToVectorSorted(noDelimEnd, p);
		return *this;
	}

	Tokenizer &Tokenizer::addSkipStart(Pattern p)
	{
		appendPatternToVectorSorted(skipStart, p);
		return *this;
	}

	Tokenizer &Tokenizer::addSkipEnd(Pattern p)
	{
		appendPatternToVectorSorted(skipEnd, p);
		return *this;
	}

	Tokenizer &Tokenizer::addWhitespace(Pattern p)
	{
		appendPatternToVectorSorted(whitespace, p);
		return *this;
	}

	Tokenizer &Tokenizer::addDeliminator(Pattern p)
	{
		appendPatternToVectorSorted(deliminator, p);
		return *this;
	}

	Tokenizer &Tokenizer::addCategorizer(unsigned int t, Pattern p)
	{
		pair<unsigned int, Pattern> tmp;
		tmp.first = t;
		tmp.second = p;
		categorizers.push_back(tmp);
		return *this;
	}

	vector<Token> Tokenizer::tokenize(string s)
	{
		vector<Token> rtn;
		unsigned int last = 0;
		unsigned int line = 0;
		unsigned int column = 0;
		bool noDelim = false;

		for (unsigned int i = 0; i < s.size(); i++)
		{
			unsigned int toksize = 0;
			unsigned int toktype = 0;

			if (noDelim == false && (toksize = isNoDelimStart(s.substr(i))))
			{
				if (i-last > 0)
				{
					Token tmp = Token(s.substr(last, i-last), line, column);
					tmp.setType(categorize(tmp));
					rtn.push_back(tmp);
				}

				last = i+1;
				noDelim = true;
			}
			else if (noDelim == true && (toksize = isNoDelimEnd(s.substr(i))))
			{
				noDelim = false;
				if (i-last > 0)
				{
					Token tmp = Token(s.substr(last-1, i-last+toksize), line, column);
					tmp.setType(categorize(tmp));
					rtn.push_back(tmp);
					last = i+toksize-1;
				}
			}
			else if (noDelim == true)
			{
			}
			else if (toksize = isSkipStart(s.substr(i)))
			{
				while(isSkipEnd(s.substr(i++)) == false)
				{
					if (s[i] == '\n')
					{
						column = 0;
						line++;
					}
					else
					{
						column++;
					}
				}
				last = i+1;
			}
			else if (toksize = isDeliminator(s.substr(i)))
			{
				if (i-last > 0)
				{
					Token tmp = Token(s.substr(last, i-last), line, column);
					tmp.setType(categorize(tmp));
					rtn.push_back(tmp);
				}
				Token tmp = Token(s.substr(i, toksize), line, column);
				tmp.setType(categorize(tmp));
				rtn.push_back(tmp);
				i += toksize-1;
				last = i+1;
			}
			else if (toksize = isWhitespace(s.substr(i)))
			{
				if (i-last > 0)
				{
					Token tmp = Token(s.substr(last, i-last), line, column);
					tmp.setType(categorize(tmp));
					rtn.push_back(tmp);
				}
				last = i+toksize;
			}

			if (s[i] == '\n')
			{
				column = 0;
				line++;
			}
			else
			{
				column++;
			}
		}

		return rtn;
	}
}
