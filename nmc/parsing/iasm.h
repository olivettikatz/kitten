#ifndef __NMC_PARSING_IASM_H
#define __NMC_PARSING_IASM_H

#include "lex.h"
#include "symbols.h"

typedef enum
{
	nop,
	const32,
	mov8,
	mov16,
	mov32,
	mov32_disp,
	ld8,
	ld16,
	ld32,
	st8,
	st16,
	st32,
	add8,
	add16,
	add32,
	sub8,
	sub16,
	sub32,
	mul32,
	and8,
	and16,
	and32,
	or8,
	or16,
	or32,
	xor8,
	xor16,
	xor32,
	pusharg32,
	poparg32,
	leave,
	ret,
	int32,
	cmp8,
	cmp16,
	cmp32,
	breq8,
	breq32,
	brne8,
	brne32,
	brlt8,
	brlt32,
	brle8,
	brle32,
	brgt8,
	brgt32,
	brge8,
	brge32,
	jmp8,
	jmp32,
	call32,
	fld32,
	fst32,
	fadd,
	fsub,
	fmul,
	fdiv,
	fcmp,
	fchs,
	fst_rot,
	fnop,
	ftan,
	fatan,
	fsin,
	fcos,
	fsincos,
	fsqrt
} iasm_inst;

typedef enum
{
	eax = 0,
	ecx = 1,
	edx = 2,
	ebx = 3,
	esp = 4,
	ebp = 5,
	esi = 6,
	edi = 7,
	ax = 0,
	cx = 1,
	dx = 2,
	bx = 3,
	al = 0,
	cl = 1,
	dl = 2,
	bl = 3
} iasm_reg;

typedef struct
{
	iasm_inst inst;
	iasm_reg r0;
	iasm_reg r1;
	int val;
} iasm;

iasm *parse_iasm(token *toks, symtable *st);

#endif
