#include "token.h"

int tokcmp(token t, char *s)
{
	return strncmp(t.str, s, t.strsize);
}

int tokcontains(token t, char *c)
{
	int n = 0;
	for (int i = 0; i < t.strsize; i++)
		if (strchr(c, t.str[i]) == NULL)
			n++;
	return n;
}

