#ifndef __NMC_PARSER_H
#define __NMC_PARSER_H

#include <iostream>
#include <map>
#include <memory>
#include "lexer.h"
#include "error.h"

namespace nmc
{
	using namespace std;

	class SymbolTable
	{
	private:
		typedef struct
		{
			int value;
		} symbol;

		map<string, symbol> content;

	public:
		SymbolTable() {}
		void append(string s, int v);
		int getValue(string s);
	};

	class InternalAssembly
	{
	public:
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
		} opcode;

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
		} reg;

		typedef struct
		{
			opcode op;
			reg r0;
			reg r1;
			int val;
		} instruction;

	private:
		vector<instruction> content;
		reg parseReg(Token t);
		int parseInt(Token t, SymbolTable &st);

	public:
		InternalAssembly() {}
		void append(opcode o, reg r0, reg r1, int val);
		vector<instruction> getContent();
		void parse(vector<Token> toks, int &off, SymbolTable &st);
	};

	class ComplexValue;
	class Operation;
	class Expression;

	class Any
	{
	public:
		typedef enum
		{
			_token,
			_complexValue,
			_operation,
			_expression,
			_null
		} category;

	private:
		category type;
		Token _t;
		shared_ptr<ComplexValue> _c;
		shared_ptr<Operation> _o;
		shared_ptr<Expression> _e;

	public:
		Any() : type(category::_null) {}
		Any(Token t) : type(category::_token), _t(t) {}
		Any(ComplexValue c);
		Any(Operation o);
		Any(Expression e);
		bool isNull();
		bool isToken();
		bool isComplexValue();
		bool isOperation();
		bool isExpression();
		Token &getToken();
		ComplexValue &getComplexValue();
		Operation &getOperation();
		Expression &getExpression();
		Any &parse(vector<Token> toks, int &off);
		string display();
	};

	class ComplexValue
	{
	private:
		vector<Any> content;

	public:
		ComplexValue() {}
		vector<Any> getContent();
		bool ok();
		ComplexValue &parse(vector<Token> toks, int &off);
		string display();
	};
		
	const int precedence[32] = {
		0,
		1,
		1,
		2,
		3,
		4,
		4,
		4,
		4,
		4,
		4,
		4,
		5,
		5,
		6,
		6,
		6,
		6,
		6,
		6,
		7,
		7,
		8,
		9,
		10,
		10,
		10,
		10,
		11,
		11,
		12
	};

	class Operation
	{
	public:
		typedef enum
		{
			_assign,
			_addAssign,
			_subAssign,
			_mulAssign,
			_divAssign,
			_modAssign,
			_andAssign,
			_orAssign,
			_xorAssign,
			_negAssign,
			_shiftLeftAssign,
			_shiftRightAssign,
			_logicalAnd,
			_logicalOr,
			_lessThan,
			_greaterThan,
			_lessThanOrEqualTo,
			_greaterThanOrEqualTo,
			_equalTo,
			_notEqualTo,
			_add,
			_sub,
			_mul,
			_div,
			_mod,
			_and,
			_or,
			_xor,
			_shiftLeft,
			_shiftRight,
			_getMember,
			_logicalNegate,
			_negate,
			_increment,
			_decrement,
			_notAnOperation
		} opsym;

	private:
		opsym sym;
		Any left;
		Any right;

		opsym parseOpsym(Token tok);
		int getPrecedence(opsym o);
		Operation parseBase(vector<Token> toks, int *m);

	public:
		Operation() : sym(opsym::_notAnOperation) {}
		opsym getOpsym();
		Any getLeft();
		Any getRight();
		bool ok();
		Operation &parse(vector<Token> toks, int &off);
		string display();
	};

	class Expression
	{
	public:
		typedef enum
		{
			_bool,
			_byte,
			_int,
			_float,
			_ref,
			_long,
			_unsigned,
			_const,
			_void,
			_class,
			_abstract,
			_private,
			_protected,
			_public,
			_variadic,
			_vector
		} typeQualifier;

	private:
		vector<typeQualifier> types;
		Token symbol;
		ComplexValue cva;
		vector<Any> args;
		vector<Any> body;
		vector<Any> where;

		typeQualifier parseType(Token tok);

	public:
		Expression() {}
		vector<typeQualifier> getTypes();
		Token getSymbol();
		ComplexValue getComplexValueArgument();
		vector<Any> getArguments();
		vector<Any> getBody();
		vector<Any> getWhereBody();
		bool ok();
		Expression &parse(vector<Token> toks, int &off);
		string display();
	};

	vector<Any> parse(vector<Token> toks);
}

#endif
