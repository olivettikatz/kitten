#include "error.h"

error _errstack[ERRMAX];
unsigned int _errcount = 0;

void push_error(error e)
{
	_errstack[_errcount++] = e;
}

void dump_error(error e)
{
	printf("%s:%i:%i\n", e.file, e.line, e.col);
}

void kill_if_errors()
{
	for (unsigned int i = 0; i < _errcount; i++)
		dump_error(_errstack[i]);
	if (_errcount > 0)
		_exit(2);
}

