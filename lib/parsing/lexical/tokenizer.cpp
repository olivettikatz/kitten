#include "tokenizer.h"

namespace parsing
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

	Token &Token::setType(string t)
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

	string Token::set(string s)
	{
		return (content = s);
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

	string Token::getType()
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

	bool Token::operator != (Token other)
	{
		return (content.compare(other.content) != 0);
	}

	bool Token::operator != (string other)
	{
		return (content.compare(other) != 0);
	}

	void Tokenizer::appendPatternToVectorSorted(vector<Pattern> &v, Pattern p)
	{
		unsigned int idx = 0;
		for (; idx < v.size(); idx++)
		{
			if (v[idx].getLengthRequest() < p.getLengthRequest())
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

	unsigned int Tokenizer::isWhitespace(string s)
	{
		return fitsPatternVector(s, _whitespace);
	}

	unsigned int Tokenizer::isDeliminator(string s)
	{
		return fitsPatternVector(s, _deliminator);
	}

	string Tokenizer::categorize(Token t)
	{
		for (vector<pair<string, Pattern> >::iterator i = categorizers.begin(); i != categorizers.end(); i++)
		{
			if (i->second.match(t.get()))
			{
				return i->first;
			}
		}

		return "";
	}

	Pattern Tokenizer::operator () ()
	{
		return Pattern();
	}

	Tokenizer &Tokenizer::noDelim(Pattern ps, Pattern pe)
	{
		pair<Pattern, Pattern> tmp;
		tmp.first = ps;
		tmp.second = pe;
		_noDelim.push_back(tmp);
		return *this;
	}

	Tokenizer &Tokenizer::skip(Pattern ps, Pattern pe)
	{
		pair<Pattern, Pattern> tmp;
		tmp.first = ps;
		tmp.second = pe;
		_skip.push_back(tmp);
		return *this;
	}

	Tokenizer &Tokenizer::whitespace(Pattern p)
	{
		appendPatternToVectorSorted(_whitespace, p);
		return *this;
	}

	Tokenizer &Tokenizer::deliminator(Pattern p)
	{
		appendPatternToVectorSorted(_deliminator, p);
		return *this;
	}

	Tokenizer &Tokenizer::categorizer(string t, Pattern p)
	{
		pair<string, Pattern> tmp;
		tmp.first = t;
		tmp.second = p;
		categorizers.push_back(tmp);
		return *this;
	}

	Tokenizer &Tokenizer::token(string t, Pattern p)
	{
		bool isdelim = false;
		for (vector<Pattern>::iterator i = _deliminator.begin(); i != _deliminator.end(); i++)
		{
			if (i->compare(p))
			{
				isdelim = true;
				break;
			}
		}

		if (isdelim == false)
			deliminator(p);

		return categorizer(t, p);
	}

	Tokenizer &Tokenizer::combine(Pattern pa, Pattern pb)
	{
		pair<Pattern, Pattern> tmp;
		tmp.first = pa;
		tmp.second = pb;
		combinators.push_back(tmp);
		return *this;
	}

	vector<Token> Tokenizer::tokenize(string s)
	{
		vector<Token> rtn;
		unsigned int last = 0;
		unsigned int line = 0;
		unsigned int column = 0;

		for (unsigned int i = 0; i < s.size(); i++)
		{
			unsigned int toksize = 0;
			unsigned int toktype = 0;

			for (vector<pair<Pattern, Pattern> >::iterator j = _noDelim.begin(); j != _noDelim.end(); j++)
			{
				if (toksize = j->first.match(s.substr(i)))
				{
					if (i-last > 0)
					{
						Token tmp = Token(s.substr(last, i-last), line, column);
						tmp.setType(categorize(tmp));
						rtn.push_back(tmp);
					}

					last = i;
					i += toksize;

					for (; i < s.size(); i++)
					{
						if (toksize = j->second.match(s.substr(i)))
						{
							Token tmp = Token(s.substr(last, i-last+toksize), line, column);
							tmp.setType(categorize(tmp));
							rtn.push_back(tmp);
							last = i+toksize;
							break;
						}
					}

					break;
				}
			}
			
			for (vector<pair<Pattern, Pattern> >::iterator j = _skip.begin(); j != _skip.end(); j++)
			{
				if (toksize = j->first.match(s.substr(i)))
				{
					if (i-last > 0)
					{
						Token tmp = Token(s.substr(last, i-last), line, column);
						tmp.setType(categorize(tmp));
						rtn.push_back(tmp);
					}

					last = i+1;
					i += toksize;

					for (; i < s.size(); i++)
					{
						if (toksize = j->second.match(s.substr(i)))
						{
							last = i+toksize;
							i += toksize;
							break;
						}
					}

					break;
				}
			}

			if (toksize = isDeliminator(s.substr(i)))
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

		for (int i = 1; i < rtn.size(); i++)
		{
			for (vector<pair<Pattern, Pattern> >::iterator j = combinators.begin(); j != combinators.end(); j++)
			{
				if (j->first.match(rtn[i-1].get()) && j->second.match(rtn[i].get()))
				{
					rtn[i-1].set(rtn[i-1].get()+rtn[i].get());
					rtn.erase(rtn.begin()+i);
					i--;
				}
			}
		}

		return rtn;
	}

	vector<Token> Tokenizer::tokenizeFile(string path)
	{
		ifstream f(path.c_str());
		string s((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
		return tokenize(s);
	}
}
