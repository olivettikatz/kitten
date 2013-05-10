#ifndef __NMC_PARSING_PARSE_H
#define __NMC_PARSING_PARSE_H

#include "lex.h"

typedef enum
{
	_token,
	_complex_value,
	_operation,
	_expression
} any_type;

typedef struct
{
	any_type type;
	token *t;
	struct complex_value *c;
	struct operation *o;
	struct expression *e;
} any;

typedef struct
{
	any *a;
} complex_value;

typedef enum
{
	assign = 0,
	add_assign,
	sub_assign,
	mul_assign,
	div_assign,
	mod_assign,
	and_assign,
	or_assign,
	xor_assign,
	neg_assign,
	shift_left_assign,
	shift_right_assign,
	logical_and,
	logical_or,
	less_than,
	greater_than,
	less_than_or_equal_to,
	greater_than_or_equal_to,
	equal_to,
	not_equal_to,
	add,
	sub,
	mul,
	div,
	mod,
	and,
	or,
	xor,
	shift_left,
	shift_right,
	get_member,
	logical_negate,
	negate,
	increment,
	decrement,
	not_an_operation
} opsym;

typedef struct
{
	opsym sym;
	any left;
	any right;
} operation;

typedef struct
{
	token *types;
	token symbol;
	complex_value cva;
	any *args;
	any *body;
	any *where;
} expression;

complex_value parse_complex_value(token *toks, int *i);
operation parse_operation(token *toks, int *i);
any parse_any(token *toks, int *i);
expression parse_expression(token *toks, int *i);
any *parse(token *toks);

#endif
