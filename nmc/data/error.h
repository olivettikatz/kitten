#ifndef __NMC_DATA_ERROR_H
#define __NMC_DATA_ERROR_H

#include <stdio.h>
#include <unistd.h>

void interr(char *msg);
void intext_int(char *name, int value);
void intext_hex(char *name, int value);
void intext_str(char *name, char *value);
void intkill();

#endif
