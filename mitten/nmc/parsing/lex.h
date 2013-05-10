#ifndef __NMC_PARSING_LEX_H
#define __NMC_PARSING_LEX_H

#include <malloc.h>

#include "../data/token.h"
#include "error.h"

char *strndup(const char *s, size_t size);
int atoi(char *s);

unsigned int cast_special_symbol(token t);
token *lex(char *buf);

#endif
