/*! \file vector.h
 * A heterogenous vector. */

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

/*! A heterogenous vector. */
typedef struct
{
	void *body; /*!< The memory space. */
	size_t head; /*!< The number of bytes used within the memory space. */
	size_t size; /*!< The size of the memory space (in bytes). */
	o_vector_entry *entries; /*!< The entry array. */
	size_t entryhead; /*!< The number of used entries in entries. */
	size_t entrysize; /*!< The number of allocated entries in entries. */
} o_vector;

/*! Initialize a vector with entries. */
o_vector o_vector_init();

/*! Initialize a vector without any entries (all entries are of size 1). */
o_vector o_vector_init_no_entries();

/*! Release the memory stored within a vector and wipe it's contents. */
void o_vector_release(o_vector *v);

/*! Gets the number of bytes used in a vector. */
size_t o_vector_headsize(o_vector *v);

/*! Gets the number of bytes allocated for use in a vector. */
size_t o_vector_memsize(o_vector *v);

/*! Gets the number of elements in a vector. */
size_t o_vector_length(o_vector *v);

/*! Allocates a new element in a vector.
 * \param s The size of the element in bytes.
 * \return The index of the element in the vector. */
int o_vector_alloc(o_vector *v, size_t s);

/*! Frees an element from a vector and the memory it takes up. */
void o_vector_free(o_vector *v, size_t i);

/*! Gets the element in a vector at an index. */
void *o_vector_at(o_vector *v, size_t i);

/*! Gets the size of an element in a vector at an index (in bytes). */
size_t o_vector_at_size(o_vector *v, size_t i);

/*! Sets the head of the vector in bytes. */
void o_vector_set_head(o_vector *v, size_t h);

/*! Gets the memory space of the vector. */
void *o_vector_body(o_vector *v);

#endif
