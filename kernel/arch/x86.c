#include "arch.h"

O_INST(nop)
{
	o_jit_emit8(j, 0x90);
}

O_INST(const)
{
	o_jit_emit8r(j, 0xb8, r0);
	o_jit_emit32(j, a);
}

O_INST(mov)
{
	o_jit_emit8(j, 0x89);
	o_jit_emit8mr(j, 0xc0, r1, r0);
}

O_INST(mov_disp)
{
	o_jit_emit8(j, 0x8b);
	o_jit_emit8mr(j, 0x40, r0, r1);
	o_jit_emit8(j, (unsigned char)a);
}

O_INST(ld)
{
	o_jit_emit8(j, 0x8b);
	o_jit_emit8mr(j, 0x00, r1, r0);
}

O_INST(st)
{
	o_jit_emit8(j, 0x89);
	o_jit_emit8mr(j, 0x00, r0, r1);
}

O_INST(add)
{
	o_jit_emit8(j, 0x01);
	o_jit_emit8mr(j, 0xc0, r1, r0);
}

O_INST(sub)
{
	o_jit_emit8(j, 0x29);
	o_jit_emit8mr(j, 0xc0, r1, r0);
}

O_INST(mul)
{
	o_jit_emit8(j, 0x0f);
	o_jit_emit8mr(j, 0xaf, r1, r0);
}

O_INST(and)
{
	o_jit_emit8(j, 0x21);
	o_jit_emit8mr(j, 0xc0, r1, r0);
}

O_INST(or)
{
	o_jit_emit8(j, 0x09);
	o_jit_emit8mr(j, 0xc0, r1, r0);
}

O_INST(xor)
{
	o_jit_emit8(j, 0x32);
	o_jit_emit8mr(j, 0xc0, r1, r0);
}

O_INST(pusharg)
{
	o_jit_emit8r(j, 0x50, r0);
}

O_INST(poparg)
{
	o_jit_emit8r(j, 0x58, r0);
}

O_INST(leave)
{
	o_jit_emit8(j, 0xc9);
}

O_INST(ret)
{
	o_jit_emit8(j, 0xc3);
}

O_INST(int)
{
	o_jit_emit8(j, 0xcd);
	o_jit_emit8(j, (unsigned char)a);
}

O_INST(cmp)
{
	o_jit_emit8(j, 0x83);
	o_jit_emit8mr(j, 0xf8, r0, (unsigned char)a);
}

O_INST(breq)
{
	o_jit_emit8(j, 0x74);
	o_jit_emit8(j, a-o_jit_label(j)-1);
}

O_INST(brne)
{
	o_jit_emit8(j, 0x75);
	o_jit_emit8(j, a-o_jit_label(j)-1);
}

O_INST(brlt)
{
	o_jit_emit8(j, 0x7c);
	o_jit_emit8(j, a-o_jit_label(j)-1);
}

O_INST(brle)
{
	o_jit_emit8(j, 0x7e);
	o_jit_emit8(j, a-o_jit_label(j)-1);
}

O_INST(brgt)
{
	o_jit_emit8(j, 0x7f);
	o_jit_emit8(j, a-o_jit_label(j)-1);
}

O_INST(brge)
{
	o_jit_emit8(j, 0x7d);
	o_jit_emit8(j, a-o_jit_label(j)-1);
}

O_INST(jmp)
{
	o_jit_emit8(j, 0xeb);
	o_jit_emit8(j, a-o_jit_label(j)-1);
}

O_INST(call)
{
	o_jit_emit8(j, 0xff);
	o_jit_emit8(j, 0xd0);
}

O_INST(fld)
{
	o_jit_emit8(j, 0xdd);
	o_jit_emit8(j, 0x05);
	o_jit_emit32(j, a);
}

O_INST(fst)
{
	o_jit_emit8(j, 0xdd);
	o_jit_emit8(j, 0x15);
	o_jit_emit32(j, a);
}

O_INST(fadd)
{
	o_jit_emit8(j, 0xd8);
	o_jit_emit8mr(j, 0xc0, r1, r0);
}

O_INST(fsub)
{
	o_jit_emit8(j, 0xd8);
	o_jit_emit8mr(j, 0xe0, r1, r0);
}

O_INST(fmul)
{
	o_jit_emit8(j, 0xd8);
	o_jit_emit8mr(j, 0xc8, r1, r0);
}

O_INST(fdiv)
{
	o_jit_emit8(j, 0xd8);
	o_jit_emit8mr(j, 0xf0, r1, r0);
}

O_INST(fcmp)
{
	o_jit_emit8(j, 0xd8);
	o_jit_emit8mr(j, 0xd0, r1, r0);
	o_jit_emit8(j, 0x9b);
	o_jit_emit8(j, 0xdf);
	o_jit_emit8(j, 0xe0);
	o_jit_emit8(j, 0x9b);
	o_jit_emit8(j, 0x9e);
}

O_INST(fchs)
{
	o_jit_emit8(j, 0xd9);
	o_jit_emit8(j, 0xe0);
}

O_INST(fst_rot)
{
	o_jit_emit8(j, 0xd9);
	o_jit_emit8(j, 0xf6);
}

O_INST(fnop)
{
	o_jit_emit8(j, 0xd9);
	o_jit_emit8(j, 0xd0);
}

O_INST(ftan)
{
	o_jit_emit8(j, 0xd9);
	o_jit_emit8(j, 0xf2);
}

O_INST(fatan)
{
	o_jit_emit8(j, 0xd9);
	o_jit_emit8(j, 0xf3);
}

O_INST(fsin)
{
	o_jit_emit8(j, 0xd9);
	o_jit_emit8(j, 0xfe);
}

O_INST(fcos)
{
	o_jit_emit8(j, 0xd9);
	o_jit_emit8(j, 0xff);
}

O_INST(fsincos)
{
	o_jit_emit8(j, 0xd9);
	o_jit_emit8(j, 0xfb);
}

O_INST(fsqrt)
{
	o_jit_emit8(j, 0xd9);
	o_jit_emit8(j, 0xfa);
}

