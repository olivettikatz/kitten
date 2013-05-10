#ifndef __NMC_DATA_TOKEN_H
#define __NMC_DATA_TOKEN_H

#include <stdio.h>
#include <string.h>

#define VALUE 0
#define OPERATION 1
#define BOUNDARY 2
#define SEPARATOR 3
#define TYPE 4
#define END_OF_LINE 5
#define SYMBOL 6

typedef struct
{
	char *str;
	size_t strsize;
	unsigned int line;
	unsigned int col;
	unsigned int cat;
} token;

int tokcmp(token t, char *s);
int tokcontains(token t, char *c);

#endif
