#ifndef __NIGHTOWL_VECTOR_H
#define __NIGHTOWL_VECTOR_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define O_VECTOR_MEMSIZE 1024
#define O_VECTOR_MEMSIZE_SCALE(s) ((s)+512)
#define O_VECTOR_ENTRYSIZE 64
#define O_VECTOR_ENTRYSIZE_SCALE(s) ((s)+32)

typedef struct
{
	size_t offset;
	size_t size;
} o_vector_entry;

typedef struct
{
	void *body;
	size_t head;
	size_t size;
	o_vector_entry *entries;
	size_t entryhead;
	size_t entrysize;
} o_vector;

o_vector o_vector_init();
o_vector o_vector_init_no_entries();
void o_vector_release(o_vector *v);
size_t o_vector_headsize(o_vector *v);
size_t o_vector_memsize(o_vector *v);
size_t o_vector_length(o_vector *v);
int o_vector_alloc(o_vector *v, size_t s);
void o_vector_free(o_vector *v, size_t i);
void *o_vector_at(o_vector *v, size_t i);
size_t o_vector_at_size(o_vector *v, size_t i);
void o_vector_set_head(o_vector *v, size_t h);
void *o_vector_body(o_vector *v);

#endif
