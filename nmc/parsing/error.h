#ifndef __NMC_PARSING_ERROR_H
#define __NMC_PARSING_ERROR_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define ERRMAX 1024

typedef struct
{
	char *file;
	unsigned int line;
	unsigned int col;
} error;

extern error _errstack[ERRMAX];
extern unsigned int _errcount;

void push_error(error e);
void dump_error(error e);
void kill_if_errors();

#endif
