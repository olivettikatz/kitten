#include "symbols.h"

int symtable_append(symtable *st, token n, void *ha)
{
	symbol tmp;
	tmp.name = n;
	tmp.heapaddr = ha;
	st->buf[st->n++] = tmp;
	return st->n-1;
}

int symtable_search(symtable *st, char *n)
{
	for (int i = 0; i < st->n; i++)
		if (tokcmp(st->buf[i].name, n) == 0)
			return i;
	return -1;
}
