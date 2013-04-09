#include "parse.h"

complex_value parse_complex_value(token *toks, int *i)
{
	complex_value rtn;
	rtn.a = (any *)malloc(1024*sizeof(any));
	int an = 0;
	int n = 0;

	for (; toks[*i].str != NULL; *i++)
	{
		if (tokcmp(toks[*i], "[") == 0)
		{
			n++;
		}
		else if (tokcmp(toks[*i], "]") == 0)
		{
			n--;
			if (n == 0)
			{
				return rtn;
			}
		}
		else if (tokcmp(toks[*i], ",") == 0)
		{
		}
		else
		{
			rtn.a[an++] = parse_any(toks, i);
		}
	}

	if (n != 0)
	{
		// error
	}
}

const int precedence[32] = {
	0,
	1,
	1,
	2,
	3,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	5,
	5,
	6,
	6,
	6,
	6,
	6,
	6,
	7,
	7,
	8,
	9,
	10,
	10,
	10,
	10,
	11,
	11,
	12
};

opsym get_opsym(token tok)
{
	if (tokcmp(tok, "=") == 0)
		return assign;
	else if (tokcmp(tok, "+=") == 0)
		return add_assign;
	else if (tokcmp(tok, "-=") == 0)
		return sub_assign;
	else if (tokcmp(tok, "*=") == 0)
		return mul_assign;
	else if (tokcmp(tok, "/=") == 0)
		return div_assign;
	else if (tokcmp(tok, "%=") == 0)
		return mod_assign;
	else if (tokcmp(tok, "&=") == 0)
		return and_assign;
	else if (tokcmp(tok, "|=") == 0)
		return or_assign;
	else if (tokcmp(tok, "^=") == 0)
		return xor_assign;
	else if (tokcmp(tok, "~=") == 0)
		return neg_assign;
	else if (tokcmp(tok, "<<=") == 0)
		return shift_left_assign;
	else if (tokcmp(tok, ">>=") == 0)
		return shift_right_assign;
	else if (tokcmp(tok, "&&") == 0)
		return logical_and;
	else if (tokcmp(tok, "||") == 0)
		return logical_or;
	else if (tokcmp(tok, "<") == 0)
		return less_than;
	else if (tokcmp(tok, ">") == 0)
		return greater_than;
	else if (tokcmp(tok, "<=") == 0)
		return less_than_or_equal_to;
	else if (tokcmp(tok, ">=") == 0)
		return greater_than_or_equal_to;
	else if (tokcmp(tok, "==") == 0)
		return equal_to;
	else if (tokcmp(tok, "!=") == 0)
		return not_equal_to;
	else if (tokcmp(tok, "+") == 0)
		return add;
	else if (tokcmp(tok, "-") == 0)
		return sub;
	else if (tokcmp(tok, "*") == 0)
		return mul;
	else if (tokcmp(tok, "/") == 0)
		return div;
	else if (tokcmp(tok, "%") == 0)
		return mod;
	else if (tokcmp(tok, "&") == 0)
		return and;
	else if (tokcmp(tok, "|") == 0)
		return or;
	else if (tokcmp(tok, "^") == 0)
		return xor;
	else if (tokcmp(tok, "<<") == 0)
		return shift_left;
	else if (tokcmp(tok, ">>") == 0)
		return shift_right;
	else if (tokcmp(tok, "++") == 0)
		return increment;
	else if (tokcmp(tok, "--") == 0)
		return decrement;
	else
	{
		// error
	}
}

operation parse_operation_base(token *toks, int *m)
{
	if (tokcmp(toks[0], "(") == 0)
		return parse_operation_base(toks+1, m);

	token stack[128];
	int stackc = 0;

	operation root;
	operation *head;

	root.sym = not_an_operation;

	if (m != NULL)
		*m = 0;

	int n = 0;
	for (int i = 0; tokcmp(toks[i], ";") != 0 || tokcmp(toks[i], ",") != 0; i++)
	{
		if (tokcmp(toks[i], "(") == 0)
		{
			if (n == 0)
			{
				if (stackc == 1 && root.sym != not_an_operation)
				{
					operation tmp;
					tmp.sym = get_opsym(stack[stackc-1]);
					tmp.left.type = _operation;
					tmp.left.o = (struct operation *)malloc(sizeof(operation));
					*((operation *)tmp.left.o) = root;
					tmp.right.type = _operation;
					tmp.right.o = (struct operation *)malloc(sizeof(operation));
					*((operation *)tmp.right.o) = parse_operation_base(toks+i+1, NULL);
					root = tmp;
					head = &root;
				}
				else if (stackc == 2 && root.sym == not_an_operation)
				{
					root.sym = get_opsym(stack[stackc-1]);
					root.left.type = _token;
					root.left.t = (token *)malloc(sizeof(token));
					*(root.left.t) = stack[stackc-2];
					root.right.type = _operation;
					root.right.o = (struct operation *)malloc(sizeof(operation));
					*((operation *)root.right.o) = parse_operation_base(toks+i+1, NULL);
					stackc = 0;
				}
				else
				{
					// error
				}
			}
			n++;
		}
		else if (tokcmp(toks[i], ")") == 0)
		{
			n--;
		}
		else if (n == 0 && (toks[i].cat == VALUE || toks[i].cat == SYMBOL))
		{
			if (stackc > 0 && stack[stackc-1].cat == OPERATION)
			{
				if (precedence[get_opsym(stack[stackc-1])] < precedence[head->sym])
				{
					if (stackc == 1 && root.sym != not_an_operation)
					{
						operation tmp;
						tmp.sym = get_opsym(stack[stackc-1]);
						tmp.left.type = _operation;
						tmp.left.o = (struct operation *)malloc(sizeof(operation));
						*((operation *)tmp.left.o) = root;
						tmp.right.type = _token;
						tmp.right.t = toks+i;
						stackc = 0;
					}
					else if (stackc == 2 && root.sym == not_an_operation)
					{
						root.sym = get_opsym(stack[stackc-1]);
						root.left.type = _token;
						root.left.t = (token *)malloc(sizeof(token));
						*(root.left.t) = stack[stackc-2];
						root.right.type = _token;
						root.right.t = toks+i;
						stackc = 0;
					}
					else
					{
						// error
					}
				}
				else
				{
					if (stackc == 1 && root.sym != not_an_operation)
					{
						operation tmp;
						tmp.sym = get_opsym(stack[stackc-1]);
						tmp.left.type = _operation;
						tmp.left.o = (struct operation *)malloc(sizeof(operation));
						*((operation *)tmp.left.o) = root;
						tmp.right.type = _token;
						tmp.right.t = (token *)malloc(sizeof(token));
						*(tmp.right.t) = stack[stackc-2];
						root = tmp;
						head = &root;
					}
					else if (stackc == 2 && root.sym == not_an_operation)
					{
						root.sym = get_opsym(stack[stackc-1]);
						root.left.type = _token;
						root.left.t = (token *)malloc(sizeof(token));
						*(root.left.t) = stack[stackc-2];
						root.right.type = _token;
						root.right.t = toks+i;
						stackc = 0;
					}
					else
					{
						// error
					}
				}
			}
			else
			{
				stack[stackc++] = toks[i];
			}
		}
		else if (n == 0 && toks[i].cat == OPERATION)
		{
			stack[stackc++] = toks[i];
		}
		else
		{
			// error
		}
		
		if (m != NULL)
			*m = i;

		if (n < 0)
		{
			if (root.sym == not_an_operation)
			{
				// error
			}

			return root;
		}

	}

	if (root.sym == not_an_operation)
	{
		 // error
	}

	return root;
}

operation parse_operation(token *toks, int *i)
{
	int tmp = 0;
	operation rtn = parse_operation_base(toks+(*i), &tmp);
	*i = tmp;
	return rtn;
}

any parse_any(token *toks, int *i)
{
	if (tokcmp(toks[*i], "[") == 0)
	{
		any a;
		a.type = _complex_value;
		a.c = (struct complex_value *)malloc(sizeof(complex_value));
		*((complex_value *)a.c) = parse_complex_value(toks, i);
		return a;
	}
	else if (toks[*i+1].cat == OPERATION || tokcmp(toks[*i], "(") == 0)
	{
		any a;
		a.type = _operation;
		a.o = (struct operation *)malloc(sizeof(operation));
		*((operation *)a.o) = parse_operation(toks, i);
		return a;
	}
	else
	{
		any a;
		a.type = _expression;
		a.e = (struct expression *)malloc(sizeof(expression));
		*((expression *)a.e) = parse_expression(toks, i);
		return a;
	}
}

expression parse_expression(token *toks, int *i)
{
	expression rtn;
	rtn.types = (token *)malloc(sizeof(token)*6);
	int ntypes = 0;

	while(toks[*i].cat == TYPE)
	{
		rtn.types[ntypes++] = toks[*i];
		(*i)++;
	}

	if (toks[*i].cat != SYMBOL)
	{
		// error
	}

	rtn.symbol = toks[*i];
	(*i)++;

	if (tokcmp(toks[*i], "[") == 0)
	{
		rtn.cva = parse_complex_value(toks, i);
	}

	if (tokcmp(toks[*i], "(") == 0)
	{
		int nargs = 0;
		rtn.args = (any *)malloc(sizeof(any)*128);
		(*i)++;

		while(1)
		{
			rtn.args[nargs++] = parse_any(toks, i);
			if (tokcmp(toks[*i], ",") == 0)
			{
			}
			else if (tokcmp(toks[*i], ")") == 0)
			{
				break;
			}
			else
			{
				// error
			}
		}
	}

	if (tokcmp(toks[*i], "{") == 0)
	{
		rtn.body = (any *)malloc(sizeof(any)*1024);
		int nbody = 0;
		(*i)++;

		while(1)
		{
			rtn.body[nbody++] = parse_any(toks, i);
			if (tokcmp(toks[*i], ";") == 0)
			{
			}
			else if (tokcmp(toks[*i], "}") == 0)
			{
				break;
			}
			else
			{
				// error
			}
		}
	}

	if (tokcmp(toks[*i], "where") == 0)
	{
		rtn.where = (any *)malloc(sizeof(any)*128);
		int nwhere = 0;
		(*i) += 2;

		while(1)
		{
			rtn.where[nwhere++] = parse_any(toks, i);
			if (tokcmp(toks[*i], ";") == 0)
			{
			}
			else if (tokcmp(toks[*i], "}") == 0)
			{
				break;
			}
			else
			{
				// error
			}
		}
	}

	return rtn;
}

any *parse(token *toks)
{
	any *rtn = (any *)malloc(sizeof(any)*1024);
	int i = 0;
	while(1)
	{

	}
}

