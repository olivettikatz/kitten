/*! \file generator.h
 * Generating machine code instructions into vectors. */

#ifndef __NIGHTOWL_JIT_GENERATOR_H
#define __NIGHTOWL_JIT_GENERATOR_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "vector.h"

/*! A JIT compiler's data. */
typedef struct
{
	o_vector content; /*!< The code so far. */
} o_jit;

/*! Initialize a new JIT compiler. */
o_jit o_jit_init();

/*! Release the memory in a JIT compiler and wipe the contents. */
void o_jit_release(o_jit *j);

/*! Gets the size of the machine code in memory. */
unsigned int o_jit_get_size(o_jit *j);

/*! Gets the number of used bytes within the machine code. */
unsigned int o_jit_get_used(o_jit *j);

/*! Emits an 8-bit opcode. */
int o_jit_emit8(o_jit *j, unsigned char b);

/*! Emits a 32-bit value. */
int o_jit_emit32(o_jit *j, int i);

/*! Emits an 8-bit opcode with a register ID added to it. */
int o_jit_emit8r(o_jit *j, unsigned char b, unsigned char r);

/*! Emits an 8-bit opcode with ORed with two registers (used heavily in x86). */
int o_jit_emit8mr(o_jit *j, unsigned char b, unsigned char a0, unsigned char a1);

/*! Emits a label (gets the current code pointer position). */
unsigned int o_jit_label(o_jit *j);

/*! Sets the insert point of the JIT compiler. */
void o_jit_insert(o_jit *j, unsigned int l);

/*! Gets a function pointer to the machine code emitted so far. */
void *o_jit_fp(o_jit *j);

#define O_INST(n) void o_emit_##n(o_jit *j, unsigned char r0, unsigned char r1, int a)
#define O_EMIT(j, n, r0, r1, a) o_emit_##n(j, (unsigned char)r0, (unsigned char)r1, (int)a)

#endif
