#include "vector.h"

o_vector o_vector_init()
{
	o_vector rtn;
	rtn.body = malloc(O_VECTOR_MEMSIZE);
	rtn.head = 0;
	rtn.size = O_VECTOR_MEMSIZE;
	rtn.entries = (o_vector_entry *)malloc(sizeof(o_vector_entry)*O_VECTOR_ENTRYSIZE);
	rtn.entryhead = 0;
	rtn.entrysize = O_VECTOR_ENTRYSIZE;
	return rtn;
}

o_vector o_vector_init_no_entries()
{
	o_vector rtn;
	rtn.body = malloc(O_VECTOR_MEMSIZE);
	rtn.head = 0;
	rtn.size = O_VECTOR_MEMSIZE;
	rtn.entries = NULL;
	rtn.entryhead = 0;
	rtn.entrysize = 0;
	return rtn;
}

void o_vector_release(o_vector *v)
{
	if (v != NULL)
	{
		free(v->body);
		v->head = 0;
		v->size = 0;
		free(v->entries);
		v->entryhead = 0;
		v->entrysize = 0;
	}
}

void _o_vector_scale_memory(o_vector *v, size_t min)
{
	size_t old = v->size;
	v->size = O_VECTOR_MEMSIZE_SCALE(v->size);
	if (v->size-old <= min)
		v->size = O_VECTOR_MEMSIZE_SCALE(old+min);
	void *tmp = malloc(v->size);
	if (v->body != NULL)
		memcpy(tmp, v->body, v->head);
	free(v->body);
	v->body = tmp;
}

void _o_vector_scale_entries(o_vector *v)
{
	v->entrysize = O_VECTOR_ENTRYSIZE_SCALE(v->entrysize);
	o_vector_entry *tmp = (o_vector_entry *)malloc(sizeof(o_vector_entry)*v->entrysize);
	if (v->entries != NULL)
		memcpy(tmp, v->entries, v->entryhead*sizeof(o_vector_entry));
	free(v->entries);
	v->entries = tmp;
}

size_t o_vector_headsize(o_vector *v)
{
	if (v != NULL)
		return v->head;
	else
		return 0;
}

size_t o_vector_memsize(o_vector *v)
{
	if (v != NULL)
		return v->size;
	else
		return 0;
}

size_t o_vector_length(o_vector *v)
{
	if (v != NULL)
	{
		if (v->entries != NULL)
			return v->entryhead;
		else
			return v->head;
	}
	else
		return 0;
}

int o_vector_alloc(o_vector *v, size_t s)
{
	if (v == NULL)
		return -1;

	if (s == 0)
		return -2;

	o_vector_entry e;
	e.offset = v->head;
	e.size = s;

	v->head += e.size;
	if (v->head >= v->size-1)
		_o_vector_scale_memory(v, s);

	if (v->entries != NULL)
	{
		v->entries[v->entryhead++] = e;
		if (v->entryhead >= v->entrysize-1)
			_o_vector_scale_entries(v);
		return v->entryhead-1;
	}
	else
	{
		return v->head-s;
	}
}

void o_vector_free(o_vector *v, size_t i)
{
	if (v == NULL)
		return ;
	if (v->body == NULL)
		return ;
	if (v->entries != NULL && i > v->entryhead)
		return ;
	if (v->entries == NULL && i > v->head)
		return ;

	if (v->entries != NULL)
	{
		memcpy(v->body+v->entries[i].offset, v->body+v->entries[i].offset+v->entries[i].size, v->head-v->entries[i].offset-v->entries[i].size);
		v->head -= v->entries[i].size;

		v->entries[i].offset = 0;
		v->entries[i].size = 0;
	}
	else
	{
		memcpy(v->body+i, v->body+i+1, v->head-i-1);
		v->head--;
	}

	if (O_VECTOR_MEMSIZE_SCALE(v->head) < v->size)
	{
		v->size = O_VECTOR_MEMSIZE_SCALE(v->head);
		void *tmp = malloc(v->size);
		memcpy(tmp, v->body, v->head);
		free(v->body);
		v->body = tmp;
	}
}

void *o_vector_at(o_vector *v, size_t i)
{
	if (v == NULL)
		return NULL;
	if (v->body == NULL)
		return NULL;
	if (v->entries != NULL && (i < v->entryhead && v->entries[i].size > 0))
		return v->body+v->entries[i].offset;
	else if (v->entries == NULL && (i < v->head))
		return v->body+i;
	else
		return NULL;
}

size_t o_vector_at_size(o_vector *v, size_t i)
{
	if (v == NULL)
		return 0;
	if (v->entries == NULL)
		return 1;
	if (i < v->entryhead && v->entries[i].size > 0)
		return v->entries[i].size;
	else
		return 0;
}

void o_vector_set_head(o_vector *v, size_t h)
{
	if (v != NULL && h < v->size)
		v->head = h;
}

void *o_vector_body(o_vector *v)
{
	if (v == NULL)
		return NULL;
	else
		return v->body;
}

