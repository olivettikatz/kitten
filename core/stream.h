#ifndef __TSE_STREAM_H
#define __TSE_STREAM_H

#include <malloc.h>
#include <string.h>
#include "../os/os.h"
#include "region.h"

typedef struct
{
	FILE *desc;
	char *buf;
	size_t bufsize;
} tse_stream;

tse_stream tse_stream_new();
tse_stream tse_stream_new_from(FILE *f);
int tse_stream_del(tse_stream *s);
FILE *tse_stream_file_get(tse_stream *s);
FILE *tse_stream_file_set(tse_stream *s, FILE *f);
size_t tse_stream_bufsize_get(tse_stream *s);
char *tse_stream_buffer_pull(tse_stream *s);

int tse_stream_to_region(tse_stream *s, tse_region *r);

#endif
