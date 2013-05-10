#ifndef __KTI_STREAM_H
#define __KTI_STREAM_H

#include <malloc.h>
#include <string.h>
#include "../os/os.h"
#include "region.h"

typedef struct
{
	FILE *desc;
	char *buf;
	size_t bufsize;
} kti_stream;

kti_stream kti_stream_new();
kti_stream kti_stream_new_from(FILE *f);
int kti_stream_del(kti_stream *s);
int kti_stream_clear(kti_stream *s);
char *kti_stream_read(kti_stream *s);

int kti_stream_to_region(kti_stream *s, kti_region *r);

#endif
