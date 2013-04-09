#include "lex.h"

unsigned int cast_special_symbol(token t)
{
	if (tokcmp(t, "true") == 0)
		return VALUE;
	else if (tokcmp(t, "True") == 0)
		return VALUE;
	else if (tokcmp(t, "TRUE") == 0)
		return VALUE;
	else if (tokcmp(t, "false") == 0)
		return VALUE;
	else if (tokcmp(t, "False") == 0)
		return VALUE;
	else if (tokcmp(t, "FALSE") == 0)
		return VALUE;
	else if (tokcontains(t, "-0123456789.abcdefABCDEFx") == 0 && strchr("0123456789.-", t.str[0]) != NULL)
		return VALUE;
	else if (tokcmp(t, "any") == 0)
		return TYPE;
	else if (tokcmp(t, "bool") == 0)
		return TYPE;
	else if (tokcmp(t, "byte") == 0)
		return TYPE;
	else if (tokcmp(t, "int") == 0)
		return TYPE;
	else if (tokcmp(t, "float") == 0)
		return TYPE;
	else if (tokcmp(t, "ref") == 0)
		return TYPE;
	else if (tokcmp(t, "long") == 0)
		return TYPE;
	else if (tokcmp(t, "unsigned") == 0)
		return TYPE;
	else if (tokcmp(t, "const") == 0)
		return TYPE;
	else if (tokcmp(t, "void") == 0)
		return TYPE;
	else if (tokcmp(t, "class") == 0)
		return TYPE;
	else if (tokcmp(t, "abstract") == 0)
		return TYPE;
	else if (tokcmp(t, "private") == 0)
		return TYPE;
	else if (tokcmp(t, "protected") == 0)
		return TYPE;
	else if (tokcmp(t, "public") == 0)
		return TYPE;
	else if (tokcmp(t, "variadic") == 0)
		return TYPE;
	else if (tokcmp(t, "b8") == 0)
		return TYPE;
	else if (tokcmp(t, "i8") == 0)
		return TYPE;
	else if (tokcmp(t, "ui8") == 0)
		return TYPE;
	else if (tokcmp(t, "i16") == 0)
		return TYPE;
	else if (tokcmp(t, "ui16") == 0)
		return TYPE;
	else if (tokcmp(t, "i32") == 0)
		return TYPE;
	else if (tokcmp(t, "ui32") == 0)
		return TYPE;
	else if (tokcmp(t, "f32") == 0)
		return TYPE;
	else if (tokcmp(t, "f64") == 0)
		return TYPE;
	else
		return t.cat;
}

token *lex(char *buf)
{
	token *rtn = (token *)malloc(sizeof(token)*strlen(buf));
	int last = 0;
	int nrtn = 0;

	int line = 0;
	int col = 0;
	int quote = 0;

	for (int i = 0; buf[i] != 0; i++)
	{
		int ts = 0;
		int tt = 0;
		
		if (buf[i] == '"')
		{
			if (quote)
			{
				quote = 0;
				if (i-last > 0)
				{
					int so = 2;
					if (buf[i+1] == 'x')
						so++;
					token tok = {
						buf+last-1,
						i-last+so,
						line,
						col,
						VALUE
					};
					rtn[nrtn++] = tok;
					last = i+1+so-2;
				}
			}
			else
			{
				if (i-last > 0)
				{
					token tok = {
						buf+last,
						i-last,
						line,
						col,
						SYMBOL
					};
					tok.cat = cast_special_symbol(tok);
					rtn[nrtn++] = tok;
				}
				last = i+1;

				quote = 1;
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
			for (;buf[i] != 0;i++)
				if (buf[i] == '*' && buf[i+1] == '/')
					break;
			i++;
			last = i+1;
		}
		else if (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n')
		{
			if (i-last > 0)
			{
				token tok = {
					buf+last,
					i-last,
					line,
					col,
					SYMBOL
				};
				tok.cat = cast_special_symbol(tok);
				rtn[nrtn++] = tok;
			}
			last = i+1;
		}
		else if (buf[i] == '(' || buf[i] == ')' || buf[i] == '[' || buf[i] == ']' || buf[i] == '{' || buf[i] == '}')
		{
			ts = 1;
			tt = BOUNDARY;
		}
		else if (buf[i] == ',')
		{
			ts = 1;
			tt = SEPARATOR;
		}
		else if (buf[i] == ';')
		{
			ts = 1;
			tt = END_OF_LINE;
		}
		else if (buf[i] == '\'' && buf[i+2] == '\'')
		{
			if (i-last > 0)
			{
				token tok = {
					buf+last,
					i-last,
					line,
					col,
					SYMBOL
				};
				tok.cat = cast_special_symbol(tok);
				rtn[nrtn++] = tok;
				last = i+1;
			}

			ts = 3;
			tt = VALUE;
		}
		else
		{	
			tt = OPERATION;
			if ((buf[i] == '<' && buf[i+1] == '<') && buf[i+2] == '=')
			{
				ts = 3;
			}
			else if ((buf[i] == '>' && buf[i+1] == '>') && buf[i+2] == '=')
			{
				ts = 3;
			}
			else if (buf[i] == '&' && buf[i+1] == '&')
			{
				ts = 2;
			}
			else if (buf[i] == '|' && buf[i+1] == '|')
			{
				ts = 2;
			}
			else if (buf[i] == '<' && buf[i+1] == '<')
			{
				ts = 2;
			}
			else if (buf[i] == '>' && buf[i+1] == '>')
			{
				ts = 2;
			}
			else if (buf[i] == '<' && buf[i+1] == '=')
			{
				ts = 2;
			}
			else if (buf[i] == '>' && buf[i+1] == '=')
			{
				ts = 2;
			}
			else if (buf[i] == '=' && buf[i+1] == '=')
			{
				ts = 2;
			}
			else if (buf[i] == '!' && buf[i+1] == '=')
			{
				ts = 2;
			}
			else if (buf[i] == '+' && buf[i+1] == '=')
			{
				ts = 2;
			}
			else if (buf[i] == '-' && buf[i+1] == '=')
			{
				ts = 2;
			}
			else if (buf[i] == '*' && buf[i+1] == '=')
			{
				ts = 2;
			}
			else if (buf[i] == '/' && buf[i+1] == '=')
			{
				ts = 2;
			}
			else if (buf[i] == '%' && buf[i+1] == '=')
			{
				ts = 2;
			}
			else if (buf[i] == '&' && buf[i+1] == '=')
			{
				ts = 2;
			}
			else if (buf[i] == '|' && buf[i+1] == '=')
			{
				ts = 2;
			}
			else if (buf[i] == '^' && buf[i+1] == '=')
			{
				ts = 2;
			}
			else if (buf[i] == '~' && buf[i+1] == '=')
			{
				ts = 2;
			}
			else if (buf[i] == '.')
			{
				ts = 1;
			}
			else if (buf[i] == '+')
			{
				ts = 1;
			}
			else if (buf[i] == '-')
			{
				ts = 1;
			}
			else if (buf[i] == '*')
			{
				ts = 1;
			}
			else if (buf[i] == '/')
			{
				ts = 1;
			}
			else if (buf[i] == '%')
			{
				ts = 1;
			}
			else if (buf[i] == '&')
			{
				ts = 1;
			}
			else if (buf[i] == '|')
			{
				ts = 1;
			}
			else if (buf[i] == '^')
			{
				ts = 1;
			}
			else if (buf[i] == '~')
			{
				ts = 1;
			}
			else if (buf[i] == '!')
			{
				ts = 1;
			}
			else if (buf[i] == '<')
			{
				ts = 1;
			}
			else if (buf[i] == '>')
			{
				ts = 1;
			}
			else if (buf[i] == '=')
			{
				ts = 1;
			}
		}

		if (ts > 0)
		{
			if (i-last > 0)
			{
				token tok = {
					buf+last,
					i-last,
					line,
					col,
					SYMBOL
				};
				tok.cat = cast_special_symbol(tok);
				rtn[nrtn++] = tok;
				last = i+1;
			}
			
			token tok = {
				buf+i,
				ts,
				line,
				col,
				tt
			};
			tok.cat = cast_special_symbol(tok);
			rtn[nrtn++] = tok;

			i += ts-1;
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

	for (int i = 0; i < nrtn; i++)
	{
		if (rtn[i].strsize == 1 && rtn[i].str[0] == '-')
		{
			if (rtn[i-1].cat != SYMBOL && rtn[i-1].cat != VALUE)
			{
				rtn[i+1].str--;
				rtn[i+1].strsize++;
				if (rtn[i+1].str[0] != '-')
				{
					// error
				}
				memcpy(rtn+i, rtn+(i+1), (nrtn-i)*sizeof(token));
				nrtn--;
				i--;
			}
		}
		else if (rtn[i].strsize == 1 && rtn[i].str[0] == '.')
		{
			if (rtn[i-1].cat != SYMBOL)
			{
				rtn[i+1].str--;
				rtn[i+1].strsize++;
				if (rtn[i+1].str[0] != '.')
				{
					// error
				}
				memcpy(rtn+i, rtn+(i+1), (nrtn-i)*sizeof(token));
				nrtn--;
				i--;
			}
		}
		else if (rtn[i].strsize == 2 && (rtn[i].str[0] == '-' && rtn[i].str[1] == '.'))
		{
			rtn[i+1].str -= 2;
			rtn[i+1].strsize += 2;
			if (rtn[i+1].str[0] != '-' || rtn[i+1].str[1] != '.')
			{
				// error
			}

			memcpy(rtn+i, rtn+(i+1), (nrtn-i)*sizeof(token));
			nrtn--;
			i--;
		}
	}

	rtn[nrtn].str = NULL;

	return rtn;
}

