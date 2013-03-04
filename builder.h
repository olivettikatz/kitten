#ifndef __NIGHTOWL_BUILDER_H
#define __NIGHTOWL_BUILDER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <math.h>

extern "C"
{
	#include "kernel/vector.h"
	#include "kernel/generator.h"
	#include "kernel/arch/arch.h"
}

namespace nightowl
{
	using namespace std;
		
	typedef enum
	{
		_nop,
		_const,
		_mov,
		_mov_disp,
		_ld,
		_st,
		_add,
		_sub,
		_mul,
		_and,
		_or,
		_xor,
		_pusharg,
		_poparg,
		_leave,
		_ret,
		_int,
		_cmp,
		_breq,
		_brne,
		_brlt,
		_brle,
		_brgt,
		_brge,
		_jmp,
		_call,
		_fld,
		_fst,
		_fadd,
		_fsub,
		_fmul,
		_fdiv,
		_fcmp,
		_fchs,
		_fst_rot,
		_fnop,
		_ftan,
		_fatan,
		_fsin,
		_fcos,
		_fsincos,
		_fsqrt
	} opcode;

	typedef void (*emitter)(o_jit*,unsigned char,unsigned char,int);
	static map<opcode, emitter> emitters = {
		{_nop, o_emit_nop},
		{_const, o_emit_const},
		{_mov, o_emit_mov},
		{_mov_disp, o_emit_mov_disp},
		{_ld, o_emit_ld},
		{_st, o_emit_st},
		{_add, o_emit_add},
		{_sub, o_emit_sub},
		{_mul, o_emit_mul},
		{_and, o_emit_and},
		{_or, o_emit_or},
		{_xor, o_emit_xor},
		{_pusharg, o_emit_pusharg},
		{_poparg, o_emit_poparg},
		{_leave, o_emit_leave},
		{_ret, o_emit_ret},
		{_int, o_emit_int},
		{_cmp, o_emit_cmp},
		{_breq, o_emit_breq},
		{_brne, o_emit_brne},
		{_brlt, o_emit_brlt},
		{_brle, o_emit_brle},
		{_brgt, o_emit_brgt},
		{_brge, o_emit_brge},
		{_breq, o_emit_breq},
		{_jmp, o_emit_jmp},
		{_call, o_emit_call},
		{_fld, o_emit_fld},
		{_fst, o_emit_fst},
		{_fadd, o_emit_fadd},
		{_fsub, o_emit_fsub},
		{_fmul, o_emit_fmul},
		{_fdiv, o_emit_fdiv},
		{_fcmp, o_emit_fcmp},
		{_fchs, o_emit_fchs},
		{_fst_rot, o_emit_fst_rot},
		{_fnop, o_emit_fnop},
		{_ftan, o_emit_ftan},
		{_fatan, o_emit_fatan},
		{_fsin, o_emit_fsin},
		{_fcos, o_emit_fcos},
		{_fsincos, o_emit_fsincos},
		{_fsqrt, o_emit_fsqrt}
	};

	class BuilderInstruction
	{
	private:
		opcode op;
		unsigned char reg0;
		unsigned char reg1;
		int arg;

	public:
		BuilderInstruction() : op(_nop), reg0(0), reg1(0), arg(0) {}
		BuilderInstruction(opcode o) : op(o), reg0(0), reg1(0), arg(0) {}
		BuilderInstruction(opcode o, int a) : op(o), reg0(0), reg1(0), arg(a) {}
		BuilderInstruction(opcode o, unsigned char r0) : op(o), reg0(r0), reg1(0), arg(0) {}
		BuilderInstruction(opcode o, unsigned char r0, int a) : op(o), reg0(r0), reg1(0), arg(a) {}
		BuilderInstruction(opcode o, unsigned char r0, unsigned char r1) : op(o), reg0(r0), reg1(r1), arg(0) {}
		BuilderInstruction(opcode o, unsigned char r0, unsigned char r1, int a) : op(o), reg0(r0), reg1(r1), arg(a) {}
		opcode getOp();
		unsigned char getReg0();
		unsigned char getReg1();
		int getArg();
	};

	class BuilderInstructionPage
	{
	public:
		typedef vector<BuilderInstruction>::iterator iterator;

	private:
		map<string, iterator> functable;
		vector<BuilderInstruction> content;

	public:
		BuilderInstructionPage() {}
		BuilderInstructionPage(BuilderInstruction i) {content.push_back(i);}
		iterator function(string n);
		vector<BuilderInstruction> getFunctionSegment(string n);
		BuilderInstructionPage &operator << (BuilderInstruction i);
		BuilderInstructionPage &operator << (BuilderInstructionPage p);
		BuilderInstruction &operator [] (unsigned int idx);
		iterator begin();
		iterator end();
		size_t size();
		unsigned char getLastReg();
	};

	class Builder
	{
	private:
		typedef struct
		{
			opcode o;
			unsigned int from;
			unsigned int to;
		} relocation;
		vector<relocation> relocations;

		unsigned int ap;

		bool enableOutputToPage;
		BuilderInstructionPage _page;
		o_jit _jit;

		typedef struct
		{
			opcode branch;
			int lif;
			int lelse;
		} ifStackElement;
		vector<ifStackElement> ifStack;

		typedef enum
		{
			free,
			pure,
			dirty,
		} regmode;

		typedef struct
		{
			regmode mode;
			int value;
		} reginfo;

		typedef struct
		{
			bool dirty;
			double *ref;
		} fpinfo;

		reginfo regs[O_REGISTER_HB];
		fpinfo fpst[O_FPST_SIZE];
		unsigned int fpstUsed;

	public:
		Builder();
		~Builder();
		void outputToPage();
		void emit(opcode o, unsigned char r0, unsigned char r1, int a);
		void emit(BuilderInstruction i);
		void emit(BuilderInstructionPage p);
		o_jit *jit();
		BuilderInstructionPage page();

		int label();

		int emitFunction();
		unsigned char emitFunctionArgument(unsigned int s);
		void emitFunctionReturn();

		unsigned int emitRelocatableBranch(opcode br, unsigned int target);
		unsigned int emitRelocatableJump(unsigned int target);
		void reemitRelocations();

		unsigned char regAlloc(bool d);
		bool regIsAllocated(unsigned char r);
		int regValue(unsigned char r, int v);
		int regValue(unsigned char r);
		bool regDirty(unsigned char r, bool d);
		bool regDirty(unsigned char r);
		void regFree(unsigned char r);
		void emitReg(unsigned char r);

		bool fpDirty(unsigned int i);
		bool fpDirty(unsigned int i, bool d);
		double fpValue(unsigned int i);
		double fpValue(unsigned int i, double v);
		double *fpRef(unsigned int i);
		void emitFp();

		unsigned char emitConstI32(int v);
		unsigned char emitRefI32(int *v);
		void emitPullI32(unsigned char r, int *v);

		void emitRefF64(double *v);
		void emitPullF64();

		unsigned char emitOperation(opcode o, unsigned char r0, unsigned char r1);
		void emitFloatingOperation(opcode o);
		void emitPushArg(unsigned char r);
		void emitPopArg(unsigned char r);
	};
}

#endif
