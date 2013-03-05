/*! \file builder.h
 * C++ library to access the (^v^) kernel and add some optimization features. */

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

	/*! One instruction's worth of bytecode (NOT machine code). You can emit these in Builders in place of the standard emit function. You can also create pages of instructions with the BuilderInstructionPage class, which is especially useful for optimizations and function inlining. */
	class BuilderInstruction
	{
	private:
		opcode op;
		unsigned char reg0;
		unsigned char reg1;
		int arg;

	public:
		/*! Initialize blank instruction (NOP). */
		BuilderInstruction() : op(_nop), reg0(0), reg1(0), arg(0) {}
		
		/*! Initialize instruction with no arguments. */
		BuilderInstruction(opcode o) : op(o), reg0(0), reg1(0), arg(0) {}
		
		/*! Initialize instruction with an integer argument (registers are 0). */
		BuilderInstruction(opcode o, int a) : op(o), reg0(0), reg1(0), arg(a) {}
		
		/*! Initialize instruction with a register argument. */
		BuilderInstruction(opcode o, unsigned char r0) : op(o), reg0(r0), reg1(0), arg(0) {}
		
		/*! Initialize instruction with a register argument and an integer arguments. */
		BuilderInstruction(opcode o, unsigned char r0, int a) : op(o), reg0(r0), reg1(0), arg(a) {}
		
		/*! Initialize instruction with two register arguments. */
		BuilderInstruction(opcode o, unsigned char r0, unsigned char r1) : op(o), reg0(r0), reg1(r1), arg(0) {}
		
		/*! Initialize instruction with two register arguments and an integer arguemnt (full initializer). */
		BuilderInstruction(opcode o, unsigned char r0, unsigned char r1, int a) : op(o), reg0(r0), reg1(r1), arg(a) {}

		/*! Get opcode. */
		opcode getOp();

		/*! Get the first register. */
		unsigned char getReg0();

		/*! Get the second register. */
		unsigned char getReg1();

		/*! Get the integer argument. */
		int getArg();
	};

	/*! Pages of instructions of bytecode (NOT machine code). You can emit pages of instructions in place of the standard emit function. You can use these for optimizations and function inlining. */
	class BuilderInstructionPage
	{
	public:
		/*! To iterate over the contents. If you have an iterator i, *i will give you a BuilderInstruction. */
		typedef vector<BuilderInstruction>::iterator iterator;

	private:
		map<string, iterator> functable;
		vector<BuilderInstruction> content;

	public:
		/*! Initialize a blank page. */
		BuilderInstructionPage() {}

		/*! Initialize a page containing one instruction: i. */
		BuilderInstructionPage(BuilderInstruction i) {content.push_back(i);}

		/*! Get the iterator marking the beginning instruction of a function n.
		* @return Will return end() if n is not a declared function. */
		iterator function(string n);

		/*! Get a BuilderInstruction vector containing ONLY instructions pertaining to the function n. Used for function inlining.
		* @return Will return a blank vector if n is not a declared function. */
		vector<BuilderInstruction> getFunctionSegment(string n);

		/*! Append an instruction. */
		BuilderInstructionPage &operator << (BuilderInstruction i);

		/*! Append a page of instructions. */
		BuilderInstructionPage &operator << (BuilderInstructionPage p);

		/*! Get the instruction at an index. Used for relocation reemition in Builder. */
		BuilderInstruction &operator [] (unsigned int idx);

		/*! Gets an iterator to the beginning of the page vector. */
		iterator begin();

		/*! Gets an iterator to the end of the page vector. */
		iterator end();

		/*! Gets the size of the page vector. */
		size_t size();

		/*! Gets the last register used in the bytecode. */
		unsigned char getLastReg();
	};

	/*! Builds machine code programs or bytecode programs. This is where the magic happens. */
	class Builder
	{
	public:
		typedef enum
		{
			reg0AboveLimit,
			reg1AboveLimit,
			nopWarning,
			nullDisplacement,
			operatingDuplicateRegisters,
			nullCall
		} errorcode;

		typedef struct
		{
			errorcode c;
			opcode o;
			unsigned char r0;
			unsigned char r1;
			int a;
			int cp;
		} error;

	private:
		typedef struct
		{
			opcode o;
			unsigned int from;
			string to;
		} relocation;
		map<string, relocation> relocations;

		unsigned int ap;

		bool enableOutputToPage;
		BuilderInstructionPage _page;
		o_jit _jit;

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

		vector<error> errorStack;

		bool checkForErrors(opcode &o, unsigned char &r0, unsigned char &r1, int &a);

	public:
		Builder();
		~Builder();

		/*! This enables outputting to a BuilderInstructionPage instead of the (^v^) kernel. */
		void outputToPage();

		unsigned int countErrors();
		error popError();

		/*! Emit an instruction.
		* \param o The instruction opcode.
		* \param r0 The first register.
		* \param r1 The second register.
		* \param a An integer argument. */
		void emit(opcode o, unsigned char r0, unsigned char r1, int a);

		/*! Emit an instruction from bytecode. */
		void emit(BuilderInstruction i);

		/*! Emit a page of instructions from bytecode. */
		void emit(BuilderInstructionPage p);

		/*! Gets a reference to the (^v^) kernel's JIT compiler struct (will be NULL if outputToPage() is used). */
		o_jit *jit();

		/*! Gets the BuilderInstructionPage (will be blank unless outputToPage() is used). */
		BuilderInstructionPage page();

		/*! Gets the current code pointer value (used for jumps and branches). */
		int label();

		/*! Emits the first few instructions of a function decleration. */
		int emitFunction();

		/*! Emits a function argument (do not mismatch this with the emitFunction() function or segfaults will occur).
		* \param s The size of the argument in bytes.
		* \return The register in which the argument is stored (allocates a new one). */
		unsigned char emitFunctionArgument(unsigned int s);

		/*! Emits a few instructions to handle a function return (returns the eax register). */
		void emitFunctionReturn();

		/*! Emits a branch instruction that can be relocated later. */
		string emitRelocatableBranch(string name, opcode br, string target);

		/*! Emits a jump instruction that can be relocated later. */
		string emitRelocatableJump(string name, string target);

		/*! Emits a relocatable label for use as a target. */
		string emitRelocatableLabel(string name);

		/*! Relocations all relocatable instructions. */
		void reemitRelocations();

		/*! Allocates a new register for use.
		* \param d Whether the register will be dirty or not. */
		unsigned char regAlloc(bool d);

		/*! Returns whether a register is allocated or not. */
		bool regIsAllocated(unsigned char r);

		/*! Sets the value of the register (to be lazily evaluated later). */
		int regValue(unsigned char r, int v);

		/*! Gets the value of the register (if not evaluated yet). */
		int regValue(unsigned char r);

		/*! Sets whether the register is dirty or not. */
		bool regDirty(unsigned char r, bool d);
		
		/*! Gets whether the register is dirty or not. */
		bool regDirty(unsigned char r);

		/*! Frees the register. */
		void regFree(unsigned char r);

		/*! Evaluates the register. */
		void emitReg(unsigned char r);

		/*! Gets whether the float at index i in the stack is dirty or not. */
		bool fpDirty(unsigned int i);

		/*! Sets whether the float at index i in the stack is dirty or not. */
		bool fpDirty(unsigned int i, bool d);

		/*! Gets the value of the float at index i in the stack (if not evaluated yet). */
		double fpValue(unsigned int i);

		/*! Sets the value of the float at index i in the stack (to be lazily evaluated later). */
		double fpValue(unsigned int i, double v);

		/*! Gets the reference in the floating pointer stack at index i. */
		double *fpRef(unsigned int i);

		/*! Evaluates the float at the top of the stack. */
		void emitFp();

		/*! Emits a constant integer (32-bits).
		* \return The register it is stored in. */
		unsigned char emitConstI32(int v);

		/*! Emits a reference to an integer (32-bits).
		* \return The register it is stored in. */
		unsigned char emitRefI32(int *v);

		/*! Emits an instruction to pull the value of a register to an integer reference (32-bits). */
		void emitPullI32(unsigned char r, int *v);

		/*! Emits a reference to a float (64-bits). */
		void emitRefF64(double *v);

		/*! Emits an instruction to pull the value of the float at the top of the stack to its reference. */
		void emitPullF64();

		/*! Emits an operation. */
		unsigned char emitOperation(opcode o, unsigned char r0, unsigned char r1);

		/*! Emits a floating-point operation between the first and second elements from the top of the stack. */
		void emitFloatingOperation(opcode o);

		/*! Emits an instruction to push an argument from a register. */
		void emitPushArg(unsigned char r);

		/*! As long as outputToPage() is enabled and only one function is declared within the Builder, you can convert it to an inline function call.
		* \param args The list of integer references to be used for arguments to be loaded into registers by the function code.
		* \return The BuilderInstructionPage containing the inlined function code. */
		BuilderInstructionPage convertToInline(vector<int *> args);
	};
}

#endif
