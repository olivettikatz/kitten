#include "error.h"

void interr(char *msg)
{
	fprintf(stderr, "error: %s\n", msg);
}

void intext_int(char *name, int value)
{
	fprintf(stderr, "\t%s == %i\n", name, value);
}

void intext_hex(char *name, int value)
{
	fprintf(stderr, "\t%s == %x\n", name, value);
}

void intext_str(char *name, char *value)
{
	fprintf(stderr, "\t%s == \"%s\"\n", name, value);
}

void intkill()
{
	_exit(1);
}

