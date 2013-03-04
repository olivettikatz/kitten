#ifndef __NIGHTOWL_JIT_ARCH_H
#define __NIGHTOWL_JIT_ARCH_H

#include "../generator.h"

#define O_UNSUPPORTED 0
#define O_X86 1
#define O_ARCH O_UNSUPPORTED

#ifdef __i386__
#undef O_ARCH
#define O_ARCH O_X86
#endif

#ifdef __i486__
#undef O_ARCH
#define O_ARCH O_X86
#endif

#ifdef __i586__
#undef O_ARCH
#define O_ARCH O_X86
#endif

#ifdef __i686__
#undef O_ARCH
#define O_ARCH O_X86
#endif

#if O_ARCH == O_UNSUPPORTED
#error NightOwl: unsupported architecture (supported: x86)
#endif

#if O_ARCH == O_X86
#define O_REGISTERS 8
#define O_REGISTER_LB 0
#define O_REGISTER_HB 4
#define O_REGISTER_RTN 0
#define O_REGISTER_EC 3

#define O_EAX 0
#define O_ECX 1
#define O_EDX 2
#define O_EBX 3
#define O_ESP 4
#define O_EBP 5
#define O_ESI 6
#define O_EDI 7

#define O_FPST_SIZE 8

#define O_AP_INIT 8
#endif

O_INST(nop);
O_INST(const);
O_INST(mov);
O_INST(mov_disp);
O_INST(ld);
O_INST(st);
O_INST(add);
O_INST(sub);
O_INST(mul);
O_INST(and);
O_INST(or);
O_INST(xor);
O_INST(pusharg);
O_INST(poparg);
O_INST(leave);
O_INST(ret);
O_INST(int);
O_INST(cmp);
O_INST(breq);
O_INST(brne);
O_INST(brlt);
O_INST(brle);
O_INST(brgt);
O_INST(brge);
O_INST(jmp);
O_INST(call);
O_INST(fld);
O_INST(fst);
O_INST(fadd);
O_INST(fsub);
O_INST(fmul);
O_INST(fdiv);
O_INST(fcmp);
O_INST(fchs);
O_INST(fst_rot);
O_INST(fnop);
O_INST(ftan);
O_INST(fatan);
O_INST(fsin);
O_INST(fcos);
O_INST(fsincos);
O_INST(fsqrt);

#endif
