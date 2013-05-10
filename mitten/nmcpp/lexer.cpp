#include "lexer.h"

namespace nmc
{
	Token::category Lexer::castSpecialSymbols(Token t)
	{
		if (t == "true")
			return Token::category::value;
		else if (t == "True")
			return Token::category::value;
		else if (t == "TRUE")
			return Token::category::value;
		else if (t == "false")
			return Token::category::value;
		else if (t == "False")
			return Token::category::value;
		else if (t == "FALSE")
			return Token::category::value;
		else if (t.contains("-0123456789.abcdefABCDEFx") && string("0123456789.-").find(t.getString()[0]) != string::npos)
			return Token::category::value;
		else if (t == "any")
			return Token::category::type;
		else if (t == "bool")
			return Token::category::type;
		else if (t == "byte")
			return Token::category::type;
		else if (t == "int")
			return Token::category::type;
		else if (t == "float")
			return Token::category::type;
		else if (t == "ref")
			return Token::category::type;
		else if (t == "long")
			return Token::category::type;
		else if (t == "unsigned")
			return Token::category::type;
		else if (t == "const")
			return Token::category::type;
		else if (t == "void")
			return Token::category::type;
		else if (t == "class")
			return Token::category::type;
		else if (t == "abstract")
			return Token::category::type;
		else if (t == "private")
			return Token::category::type;
		else if (t == "protected")
			return Token::category::type;
		else if (t == "public")
			return Token::category::type;
		else if (t == "variadic")
			return Token::category::type;
		else if (t == "b8")
			return Token::category::type;
		else if (t == "i8")
			return Token::category::type;
		else if (t == "ui8")
			return Token::category::type;
		else if (t == "i16")
			return Token::category::type;
		else if (t == "ui16")
			return Token::category::type;
		else if (t == "i32")
			return Token::category::type;
		else if (t == "ui32")
			return Token::category::type;
		else if (t == "f32")
			return Token::category::type;
		else if (t == "f64")
			return Token::category::type;
		else if (t == "vector")
			return Token::category::type;
		else
			return t.getType();
	}

	void Lexer::safePush(vector<Token> &rtn, Token tok)
	{
		if (tok.getType() == Token::category::symbol)
			tok.setType(castSpecialSymbols(tok));
		
		if (!rtn.empty() && tok == ".")
		{
			if (rtn.back() == "-")
			{
				rtn.back().setString("-.");
				rtn.back().setType(Token::category::value);
				return ;
			}
			else if (rtn.back().getType() == Token::category::value)
			{
				rtn.back().setString(rtn.back().getString()+".");
				return ;
			}
		}
		else if (!rtn.empty() && tok.getType() == Token::category::value)
		{
			if (rtn.back().getType() == Token::category::value)
			{
				rtn.back().setString(rtn.back().getString()+tok.getString());
				return ;
			}
			else if (rtn.back() == "-")
			{
				if (rtn[rtn.size()-2].getType() == Token::category::symbol)
				{
				}
				else if (rtn[rtn.size()-2].getType() == Token::category::value)
				{
				}
				else if (rtn[rtn.size()-2] == ")")
				{
				}
				else if (rtn[rtn.size()-2] == "}")
				{
				}
				else
				{
					rtn.back().setString(rtn.back().getString()+tok.getString());
					rtn.back().setType(Token::category::value);
					return ;
				}
			}
			else if (rtn.back() == ".")
			{
				if (rtn[rtn.size()-2].getType() == Token::category::symbol)
				{
				}
				else if (rtn[rtn.size()-2] == ")")
				{
				}
				else if (rtn[rtn.size()-2] == "}")
				{
				}
				else
				{
					rtn.back().setString(rtn.back().getString()+tok.getString());
					rtn.back().setType(Token::category::value);
					return ;
				}
			}
		}

		rtn.push_back(tok);
	}

	vector<Token> Lexer::lex(string buf)
	{
		vector<Token> rtn;
		unsigned int last = 0;
		unsigned int line = 0;
		unsigned int col = 0;
		bool quote = false;

		for (unsigned int i = 0; i < buf.size(); i++)
		{
			unsigned int toksize = 0;
			Token::category toktype = Token::category::symbol;

			if (buf[i] == '"')
			{
				if (quote)
				{
					quote = false;
					if (i-last > 0)
					{
						int so = 2;
						if (buf[i+1] == 'x')
							so++;
						safePush(rtn, Token(buf.substr(last-1, i-last+so), line, col).setType(Token::category::value));
						last = i+so-1;
					}
				}
				else
				{
					if (i-last > 0)
					{
						safePush(rtn, Token(buf.substr(last, i-last), line, col).setType(Token::category::symbol));
					}
					last = i+1;
					quote = true;
				}
			}
			else if (quote)
			{
			}
			else if (buf[i] == '/' && buf[i+1] == '/')
			{
				while(buf[i++] != '\n');
				last = i+1;
			}
			else if (buf[i] == '/' && buf[i+1] == '*')
			{
				for (;buf[i] != 0; i++)
					if (buf[i] == '*' && buf[i+1] == '/')
						break;
				i++;
				last = i+1;
			}
			else if (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
			{
				if (i-last > 0)
				{
					safePush(rtn, Token(buf.substr(last, i-last), line, col).setType(Token::category::symbol));
				}
				last = i+1;
			}
			else if (buf[i] == '(' || buf[i] == ')' || buf[i] == '[' || buf[i] == ']' || buf[i] == '{' || buf[i] == '}')
			{
				toksize = 1;
				toktype = Token::category::boundary;
			}
			else if (buf[i] == ',')
			{
				toksize = 1;
				toktype = Token::category::separator;
			}
			else if (buf[i] == ';')
			{
				toksize = 1;
				toktype = Token::category::eol;
			}
			else if (buf[i] == '\'' && (buf[i+1] == '\\' && buf[i+2] == 'x'))
			{
				toksize = 6;
				toktype = Token::category::value;
			}
			else if (buf[i] == '\'' && buf[i+1] == '\\')
			{
				toksize = 4;
				toktype = Token::category::value;
			}
			else if (buf[i] == '\'' && buf[i+2] == '\'')
			{
				toksize = 3;
				toktype = Token::category::value;
			}
			else
			{
				toktype = Token::category::operation;
				if ((buf[i] == '<' && buf[i+1] == '<') && buf[i+2] == '=')
					toksize = 3;
				else if ((buf[i] == '>' && buf[i+1] == '>') && buf[i+2] == '=')
					toksize = 3;
				else if (buf[i] == '&' && buf[i+1] == '&')
					toksize = 2;
				else if (buf[i] == '|' && buf[i+1] == '|')
					toksize = 2;
				else if (buf[i] == '<' && buf[i+1] == '<')
					toksize = 2;
				else if (buf[i] == '>' && buf[i+1] == '>')
					toksize = 2;
				else if (buf[i] == '<' && buf[i+1] == '=')
					toksize = 2;
				else if (buf[i] == '>' && buf[i+1] == '=')
					toksize = 2;
				else if (buf[i] == '=' && buf[i+1] == '=')
					toksize = 2;
				else if (buf[i] == '!' && buf[i+1] == '=')
					toksize = 2;
				else if (buf[i] == '+' && buf[i+1] == '=')
					toksize = 2;
				else if (buf[i] == '-' && buf[i+1] == '=')
					toksize = 2;
				else if (buf[i] == '*' && buf[i+1] == '=')
					toksize = 2;
				else if (buf[i] == '/' && buf[i+1] == '=')
					toksize = 2;
				else if (buf[i] == '%' && buf[i+1] == '=')
					toksize = 2;
				else if (buf[i] == '&' && buf[i+1] == '=')
					toksize = 2;
				else if (buf[i] == '|' && buf[i+1] == '=')
					toksize = 2;
				else if (buf[i] == '^' && buf[i+1] == '=')
					toksize = 2;
				else if (buf[i] == '~' && buf[i+1] == '=')
					toksize = 2;
				else if (buf[i] == '.')
					toksize = 1;
				else if (buf[i] == '+')
					toksize = 1;
				else if (buf[i] == '-')
					toksize = 1;
				else if (buf[i] == '*')
					toksize = 1;
				else if (buf[i] == '/')
					toksize = 1;
				else if (buf[i] == '%')
					toksize = 1;
				else if (buf[i] == '&')
					toksize = 1;
				else if (buf[i] == '|')
					toksize = 1;
				else if (buf[i] == '<')
					toksize = 1;
				else if (buf[i] == '>')
					toksize = 1;
				else if (buf[i] == '=')
					toksize = 1;
			}

			if (toksize > 0)
			{
				if (i-last > 0)
				{
					safePush(rtn, Token(buf.substr(last, i-last), line, col).setType(Token::category::symbol));
					last = i+1;
				}

				safePush(rtn, Token(buf.substr(i, toksize), line, col).setType(toktype));
				i += toksize-1;
				last = i+1;
			}

			if (buf[i] == '\n')
			{
				col = 0;
				line++;
			}
			else
			{
				col++;
			}
		}

		return rtn;
	}
}
