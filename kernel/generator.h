#ifndef __NIGHTOWL_JIT_GENERATOR_H
#define __NIGHTOWL_JIT_GENERATOR_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "vector.h"

typedef struct
{
	o_vector content;
} o_jit;

o_jit o_jit_init();
void o_jit_release(o_jit *j);
unsigned int o_jit_get_size(o_jit *j);
unsigned int o_jit_get_used(o_jit *j);
int o_jit_emit8(o_jit *j, unsigned char b);
int o_jit_emit32(o_jit *j, int i);
int o_jit_emit8r(o_jit *j, unsigned char b, unsigned char r);
int o_jit_emit8mr(o_jit *j, unsigned char b, unsigned char a0, unsigned char a1);
unsigned int o_jit_label(o_jit *j);
void o_jit_insert(o_jit *j, unsigned int l);
void *o_jit_fp(o_jit *j);

#define O_INST(n) void o_emit_##n(o_jit *j, unsigned char r0, unsigned char r1, int a)
#define O_EMIT(j, n, r0, r1, a) o_emit_##n(j, (unsigned char)r0, (unsigned char)r1, (int)a)

#endif
