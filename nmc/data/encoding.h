#ifndef __NMC_DATA_ENCODING_H
#define __NMC_DATA_ENCODING_H

#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "error.h"

#define ASCII 0
#define UTF8 1
#define UTF16 2
#define UTF32 3

char *read_ascii(char *path);
char *read_utf8(char *path);
char *read_utf16(char *path);
char *read_utf32(char *path);
char *read_encoded(char *path, int enc);

#endif
