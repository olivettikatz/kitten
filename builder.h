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
		iterator begin();
		iterator end();
		size_t size();
		unsigned char getLastReg();
	};

	class Builder
	{
	private:
		typedef void (*emitter)(o_jit*,unsigned char,unsigned char,int);

		map<opcode, emitter> emitters;
		map<string, int> labels;

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
		void emit(opcode o, unsigned char r0, unsigned char r1, int a);
		void emit(BuilderInstruction i);
		void emit(BuilderInstructionPage p);
		o_jit *jit();

		int label();

		int emitFunctionPrototype(string n, vector<pair<unsigned char, unsigned int> > args, int argreg);
		int emitFunctionReturn();

		int emitIfCondition(unsigned char r, int v, opcode o);
		int emitIfCondition(unsigned char r);
		int emitIfElse();
		int emitIfEndif(int lif, opcode o);
		int emitIfEndif(int lif);
		int emitIfEndif(int lif, int lelse, opcode o);
		int emitIfEndif(int lif, int lelse);
		int emitIfEndif();

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
