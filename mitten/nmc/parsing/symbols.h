#ifndef __NMC_PARSING_SYMBOLS_H
#define __NMC_PARSING_SYMBOLS_H

#include "lex.h"

typedef struct
{
	token name;
	void *heapaddr;
} symbol;

#define MAX_SYMBOLS 1024

typedef struct
{
	symbol buf[MAX_SYMBOLS];
	int n;
} symtable;

int symtable_append(symtable *st, token n, void *ha);
int symtable_search(symtable *st, char *n);

#endif
