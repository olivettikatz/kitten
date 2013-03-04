#include "generator.h"

o_jit o_jit_init()
{
	o_jit rtn;
	rtn.content = o_vector_init_no_entries();
}

void o_jit_release(o_jit *j)
{
	o_vector_release(&j->content);
}

unsigned int o_jit_get_size(o_jit *j)
{
	return o_vector_memsize(&j->content);
}

unsigned int o_jit_get_used(o_jit *j)
{
	return o_vector_headsize(&j->content);

}

int o_jit_emit8(o_jit *j, unsigned char b)
{
	*(unsigned char *)o_vector_at(&j->content, o_vector_alloc(&j->content, 1)) = b;
	return 0;
}

int o_jit_emit32(o_jit *j, int i)
{
	memcpy(o_vector_at(&j->content, o_vector_alloc(&j->content, sizeof(int))), &i, sizeof(int));
	return 0;
}

int o_jit_emit8r(o_jit *j, unsigned char b, unsigned char r)
{
	return o_jit_emit8(j, b+r);
}

int o_jit_emit8mr(o_jit *j, unsigned char b, unsigned char a0, unsigned char a1)
{
	return o_jit_emit8(j, b | (a0<<3) | a1);
}

unsigned int o_jit_label(o_jit *j)
{
	return o_vector_headsize(&j->content);
}

void o_jit_insert(o_jit *j, unsigned int l)
{
	o_vector_set_head(&j->content, l);
}

void *o_jit_fp(o_jit *j)
{
	return o_vector_body(&j->content);
}

