#include "parser.h"

namespace nmc
{
	void SymbolTable::append(string s, int v)
	{
		content[s] = {
			v
		};
	}

	int SymbolTable::getValue(string s)
	{
		return content[s].value;
	}

	InternalAssembly::reg InternalAssembly::parseReg(Token t)
	{
		if (t == "eax")
			return reg::eax;
		else if (t == "ecx")
			return reg::ecx;
		else if (t == "edx")
			return reg::edx;
		else if (t == "ebx")
			return reg::ebx;
		else if (t == "esp")
			return reg::esp;
		else if (t == "ebp")
			return reg::ebp;
		else if (t == "esi")
			return reg::esi;
		else if (t == "edi")
			return reg::edi;
		else if (t == "ax")
			return reg::ax;
		else if (t == "cx")
			return reg::cx;
		else if (t == "dx")
			return reg::dx;
		else if (t == "bx")
			return reg::bx;
		else if (t == "al")
			return reg::al;
		else if (t == "cl")
			return reg::cl;
		else if (t == "bl")
			return reg::bl;
		else
		{
			NMC_ERROR(t, "this isn't a register type (" << t.getString() << ")");
			NMC_NOTE("available: eax, ecx, ebx, esp, ebp, esi, edi,");
			NMC_NOTE("ax, cx, dx, bx, al, cl, bl");
			return reg::eax;
		}
	}

	int InternalAssembly::parseInt(Token t, SymbolTable &st)
	{
		if (t.getType() == Token::category::value)
		{
			return atoi(t.getString().c_str());
		}
		else if (t.getType() == Token::category::symbol)
		{
			return st.getValue(t.getString());
		}
		else
		{
		}
	}

	void InternalAssembly::append(InternalAssembly::opcode o, InternalAssembly::reg r0, InternalAssembly::reg r1, int val)
	{
		instruction tmp = {
			o, r0, r1, val
		};
		content.push_back(tmp);
	}

	vector<InternalAssembly::instruction> InternalAssembly::getContent()
	{
		return content;
	}

	void InternalAssembly::parse(vector<Token> toks, int &off, SymbolTable &st)
	{
		for (; off < toks.size(); off++)
		{
			if (toks[off] == ";")
				continue;

			if (toks[off] == "}")
				break;

			instruction tmp;
			tmp.op = opcode::nop;
			tmp.r0 = reg::eax;
			tmp.r1 = reg::eax;
			tmp.val = 0;
			
			if (toks[off] == "nop")
				tmp.op = opcode::nop;
			else if (toks[off] == "const32")
			{
				tmp.op = opcode::const32;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "mov8")
			{
				tmp.op = opcode::mov8;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "mov16")
			{
				tmp.op = opcode::mov16;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "mov32")
			{
				tmp.op = opcode::mov32;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "mov32_disp")
			{
				tmp.op = opcode::mov32_disp;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
				tmp.val = parseInt(toks[off+3], st);
			}
			else if (toks[off] == "ld8")
			{
				tmp.op = opcode::ld8;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "ld16")
			{
				tmp.op = opcode::ld16;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "ld32")
			{
				tmp.op = opcode::ld32;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "st8")
			{
				tmp.op = opcode::st8;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "st16")
			{
				tmp.op = opcode::st16;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "st32")
			{
				tmp.op = opcode::st32;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "add8")
			{
				tmp.op = opcode::add8;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "add16")
			{
				tmp.op = opcode::add16;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "add32")
			{
				tmp.op = opcode::add32;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "sub8")
			{
				tmp.op = opcode::sub8;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "sub16")
			{
				tmp.op = opcode::sub16;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "sub32")
			{
				tmp.op = opcode::sub32;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "mul32")
			{
				tmp.op = opcode::mul32;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "and8")
			{
				tmp.op = opcode::and8;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "and16")
			{
				tmp.op = opcode::and16;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "and32")
			{
				tmp.op = opcode::and32;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "or8")
			{
				tmp.op = opcode::or8;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "or16")
			{
				tmp.op = opcode::or16;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "or32")
			{
				tmp.op = opcode::or32;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "xor8")
			{
				tmp.op = opcode::xor8;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "xor16")
			{
				tmp.op = opcode::xor16;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "xor32")
			{
				tmp.op = opcode::xor32;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "pusharg32")
			{
				tmp.op = opcode::pusharg32;
				tmp.r0 = parseReg(toks[off+1]);
			}
			else if (toks[off] == "poparg32")
			{
				tmp.op = opcode::poparg32;
				tmp.r0 = parseReg(toks[off+1]);
			}
			else if (toks[off] == "leave")
				tmp.op = opcode::leave;
			else if (toks[off] == "ret")
				tmp.op = opcode::ret;
			else if (toks[off] == "int32")
			{
				tmp.op = opcode::int32;
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "cmp8")
			{
				tmp.op = opcode::cmp8;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "cmp16")
			{
				tmp.op = opcode::cmp16;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "cmp32")
			{
				tmp.op = opcode::cmp32;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "breq8")
			{
				tmp.op = opcode::breq8;
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "breq32")
			{
				tmp.op = opcode::breq32;
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "brne8")
			{
				tmp.op = opcode::brne8;
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "brne32")
			{
				tmp.op = opcode::brne32;
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "brlt8")
			{
				tmp.op = opcode::brlt8;
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "brlt32")
			{
				tmp.op = opcode::brlt32;
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "brle8")
			{
				tmp.op = opcode::brle8;
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "brle32")
			{
				tmp.op = opcode::brle32;
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "brgt8")
			{
				tmp.op = opcode::brgt8;
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "brgt32")
			{
				tmp.op = opcode::brgt32;
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "brge8")
			{
				tmp.op = opcode::brge8;
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "brge32")
			{
				tmp.op = opcode::brge32;
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "jmp8")
			{
				tmp.op = opcode::jmp8;
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "jmp32")
			{
				tmp.op = opcode::jmp32;
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "call32")
			{
				tmp.op = opcode::call32;
				tmp.val = parseInt(toks[off+2], st);
			}
			else if (toks[off] == "fld32")
			{
				tmp.op = opcode::fld32;
				tmp.val = parseInt(toks[off+1], st);
			}
			else if (toks[off] == "fst32")
			{
				tmp.op = opcode::fst32;
				tmp.val = parseInt(toks[off+1], st);
			}
			else if (toks[off] == "fadd")
			{
				tmp.op = opcode::fadd;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "fsub")
			{
				tmp.op = opcode::fsub;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "fmul")
			{
				tmp.op = opcode::fmul;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "fdiv")
			{
				tmp.op = opcode::fdiv;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "fcmp")
			{
				tmp.op = opcode::fcmp;
				tmp.r0 = parseReg(toks[off+1]);
				tmp.r1 = parseReg(toks[off+2]);
			}
			else if (toks[off] == "fchs")
			{
				tmp.op = opcode::fchs;
				tmp.r0 = parseReg(toks[off+1]);
			}
			else if (toks[off] == "fst_rot")
				tmp.op = opcode::fst_rot;
			else if (toks[off] == "fnop")
				tmp.op = opcode::fnop;
			else if (toks[off] == "ftan")
				tmp.op = opcode::ftan;
			else if (toks[off] == "fatan")
				tmp.op = opcode::fatan;
			else if (toks[off] == "fsin")
				tmp.op = opcode::fsin;
			else if (toks[off] == "fcos")
				tmp.op = opcode::fcos;
			else if (toks[off] == "fsincos")
				tmp.op = opcode::fsincos;
			else if (toks[off] == "fsqrt")
				tmp.op = opcode::fsqrt;
			else if (toks[off] == "label")
				st.append(toks[off+1].getString(), content.size());
			else
			{
				NMC_ERROR(toks[off], "this isn't a mnemonic (" << toks[off].getString() << ")");
				NMC_NOTE("there are too many to list here; look them up");
			}

			content.push_back(tmp);
		}
	}

	Any::Any(ComplexValue c)
	{
		type = category::_complexValue;
		_c = shared_ptr<ComplexValue>(new ComplexValue(c));
		if (!_c->ok())
			type = category::_null;
	}

	Any::Any(Operation o)
	{
		type = category::_operation;
		_o = shared_ptr<Operation>(new Operation(o));
		if (!_o->ok())
			type = category::_null;
	}
	
	Any::Any(Expression e)
	{
		type = category::_expression;
		_e = shared_ptr<Expression>(new Expression(e));
		if (!_e->ok())
			type = category::_null;
	}

	bool Any::isNull()
	{
		return (type == category::_null);
	}

	bool Any::isToken()
	{
		return (type == category::_token);
	}

	bool Any::isComplexValue()
	{
		return (type == category::_complexValue);
	}

	bool Any::isOperation()
	{
		return (type == category::_operation);
	}

	bool Any::isExpression()
	{
		return (type == category::_expression);
	}

	Token &Any::getToken()
	{
		return _t;
	}

	ComplexValue &Any::getComplexValue()
	{
		return *_c;
	}

	Operation &Any::getOperation()
	{
		return *_o;
	}

	Expression &Any::getExpression()
	{
		return *_e;
	}

	Any &Any::parse(vector<Token> toks, int &off)
	{
		if (toks[off] == "[")
		{
			*this = Any(ComplexValue().parse(toks, off));
		}
		else if ((toks[off].getType() == Token::category::value && toks[off+1].getType() == Token::category::operation) || toks[off] == "(")
		{
			*this = Any(Operation().parse(toks, off));
		}
		else if (toks[off].getType() == Token::category::value)
		{
			*this = Any(toks[off]);
		}
		else
		{
			*this = Any(Expression().parse(toks, off));
		}

		return *this;
	}

	string Any::display()
	{
		if (type == category::_token)
			return _t.display();
		else if (type == category::_operation)
			return _o->display();
		else if (type == category::_complexValue)
			return _c->display();
		else if (type == category::_expression)
			return _e->display();
		else
			return "_";
	}

	vector<Any> ComplexValue::getContent()
	{
		return content;
	}

	bool ComplexValue::ok()
	{
		return !content.empty();
	}

	ComplexValue &ComplexValue::parse(vector<Token> toks, int &off)
	{
		int n = 0;

		for (; off < toks.size(); off++)
		{
			if (toks[off] == "[")
			{
				n++;
			}
			else if (toks[off] == "]")
			{
				n--;
				if (n == 0)
				{
					break;
				}
			}
			else if (toks[off] == ",")
			{
			}
			else
			{
				content.push_back(Any().parse(toks, off));
			}
		}

		if (n != 0)
		{
			NMC_ERROR(toks[off], "mismatched square brackets");
		}

		return *this;
	}

	string ComplexValue::display()
	{
		if (content.empty())
			return "[]";

		stringstream ss;
		ss << "[ " << content[0].display() << "\n";
		for (int i = 1; i < content.size(); i++)
			ss << ", " << content[i].display() << "\n";
		string tmp = ss.str();
		tmp = tmp.substr(0, tmp.size()-1);
		return tmp+" ]\n";
	}

	Operation::opsym Operation::parseOpsym(Token tok)
	{
		if (tok == "=")
			return opsym::_assign;
		else if (tok == "+=")
			return opsym::_addAssign;
		else if (tok == "-=")
			return opsym::_subAssign;
		else if (tok == "*=")
			return opsym::_mulAssign;
		else if (tok == "/=")
			return opsym::_divAssign;
		else if (tok == "%=")
			return opsym::_modAssign;
		else if (tok == "&=")
			return opsym::_andAssign;
		else if (tok == "|=")
			return opsym::_orAssign;
		else if (tok == "^=")
			return opsym::_xorAssign;
		else if (tok == "~=")
			return opsym::_negAssign;
		else if (tok == "<<=")
			return opsym::_shiftLeftAssign;
		else if (tok == ">>=")
			return opsym::_shiftRightAssign;
		else if (tok == "&&")
			return opsym::_logicalAnd;
		else if (tok == "||")
			return opsym::_logicalOr;
		else if (tok == "<")
			return opsym::_lessThan;
		else if (tok == ">")
			return opsym::_greaterThan;
		else if (tok == "<=")
			return opsym::_lessThanOrEqualTo;
		else if (tok == ">=")
			return opsym::_greaterThanOrEqualTo;
		else if (tok == "==")
			return opsym::_equalTo;
		else if (tok == "!=")
			return opsym::_notEqualTo;
		else if (tok == "+")
			return opsym::_add;
		else if (tok == "-")
			return opsym::_sub;
		else if (tok == "*")
			return opsym::_mul;
		else if (tok == "/")
			return opsym::_div;
		else if (tok == "&")
			return opsym::_and;
		else if (tok == "|")
			return opsym::_or;
		else if (tok == "^")
			return opsym::_xor;
		else if (tok == "<<")
			return opsym::_shiftLeft;
		else if (tok == ">>")
			return opsym::_shiftRight;
		else if (tok == "++")
			return opsym::_increment;
		else if (tok == "--")
			return opsym::_decrement;
		else
		{
			NMC_ERROR(tok, "this isn't an operator (" << tok.getString() << ")");
			return opsym::_notAnOperation;
		}
	}

	int Operation::getPrecedence(Operation::opsym o)
	{
		return precedence[o];
	}

	Operation Operation::parseBase(vector<Token> toks, int *m)
	{
		vector<Token> stack; // the token stack (max 2 values/operations)
		Operation root; // the root operation (this is what is returned)
		Operation *head = &root; // the head operation
		root.sym = opsym::_notAnOperation; // initialize the root operation's symbol

		// m is the length of the operation in the token listing
		if (m != NULL)
			*m = 0;

		// n is the operation scope index (for every scope increase, n++)
		int n = 0;
		for (int i = 0; i < toks.size() && (toks[i] != ";" || toks[i] != ","); i++)
		{
			if (toks[i] == "(")
			{
				// we parse and append the new operation in the sub-scope
				if (n == 0)
				{
					if (stack.empty() && root.sym == opsym::_notAnOperation)
					{
						// there is no root, so we make a new one by parsing the sub-scope
						vector<Token> tmp2(toks.begin()+(i+1), toks.end());
						int tmpm = 0;
						root = parseBase(tmp2, &tmpm);
						i += tmpm;
						head = &root;
						stack.clear();
					}
					else if (stack.size() == 1 && root.sym != opsym::_notAnOperation)
					{
						// there is a root and we so far have one operation on the stack
						Operation tmp;
						// parse the operation on the stack
						tmp.sym = parseOpsym(stack[0]);
						tmp.left = *head;
						vector<Token> tmp2(toks.begin()+(i+1), toks.end());
						// parse the sub-scope
						int tmpm = 0;
						tmp.right = parseBase(tmp2, &tmpm);
						i += tmpm;
						// append it properly
						*head = tmp;
						stack.clear();
					}
					else if (stack.size() == 2 && root.sym == opsym::_notAnOperation)
					{
						// there is no root, but we do have a value and an operation on the stack
						// so we parse the sub-scope and create a new root with what we have
						root.sym = parseOpsym(stack[stack.size()-1]);
						root.left = stack[stack.size()-2];
						vector<Token> tmp(toks.begin()+(i+1), toks.end());
						int tmpm = 0;
						root.right = parseBase(tmp, &tmpm);
						i += tmpm;
						head = &root;
						stack.clear();
					}
					else
					{
						NMC_ERROR(toks[i], "this operation isn't complete");
						NMC_NOTE("often caused by two operations back to back (1+ +1),");
						NMC_NOTE("or two values back to back (1 1 +)");
						NMC_NOTE("it's bad, just leave it at that");
					}
				}

				// increase the scope index
				n++;
			}
			else if (toks[i] == ")")
			{
				// decrease the scope index
				n--;
				
				if (n < 0)
				{
					// we have overstepped our bounds and must exit
					if (root.sym == opsym::_notAnOperation)
					{
						// if we haven't made an operation yet and we've already overstepped our bounds,
						// something is wrong
						NMC_ERROR(toks[i], "the operation has ended before it's began");
						NMC_NOTE("a boundary token appeared before any operation stuff");
						NMC_NOTE("for example: 1 + }");
					}

					return root; // exiting...
				}
			}
			else if (n == 0 && (toks[i].getType() == Token::category::value || toks[i].getType() == Token::category::symbol))
			{
				// if we've reached a value that is in our scope, we append it
				if (stack.size() > 0 && stack[stack.size()-1].getType() == Token::category::operation)
				{
					// if there is already an operation on the stack, we can complete it
					if (stack.size() == 2 && root.sym == opsym::_notAnOperation)
					{
						// if there is no root, we make a new root
						root.sym = parseOpsym(stack[stack.size()-1]);
						root.left = stack[stack.size()-2];
						root.right = toks[i];
						head = &root;
						stack.clear();
					}
					else if (getPrecedence(parseOpsym(stack[stack.size()-1])) <= getPrecedence(head->sym))
					{
						// if the precedence of the new operation is lower than the root
						if (stack.size() == 1 && root.sym != opsym::_notAnOperation)
						{
							// wrap the head
							Operation tmp;
							tmp.sym = parseOpsym(stack[stack.size()-1]);
							tmp.left = *head;
							tmp.right = toks[i];
							*head = tmp;
							stack.clear();
						}
						else
						{
							NMC_ERROR(toks[i], "this operation isn't complete");
							NMC_NOTE("often caused by two operations back to back (1+ +1),");
							NMC_NOTE("or two values back to back (1 1 +)");
							NMC_NOTE("it's bad, just leave it at that");
						}
					}
					else
					{
						// if the precedence of the new operation is lower than the root
						if (stack.size() == 1 && root.sym != opsym::_notAnOperation)
						{
							Operation tmp;
							tmp.sym = parseOpsym(stack[stack.size()-1]);
							tmp.left = head->right;
							tmp.right = toks[i];
							head->right = tmp;
							head = &head->right.getOperation();
							stack.clear();
						}
						else
						{
							NMC_ERROR(toks[i], "this operation isn't complete");
							NMC_NOTE("often caused by two operations back to back (1+ +1),");
							NMC_NOTE("or two values back to back (1 1 +)");
							NMC_NOTE("it's bad, just leave it at that");
						}
					}
				}
				else
				{
					// the symbol is loose and can only be pushed to the stack for further use
					stack.push_back(toks[i]);
				}
			}
			else if (n == 0 && toks[i].getType() == Token::category::operation)
			{
				// operations are always pushed to the stack
				stack.push_back(toks[i]);
			}

			if (m != NULL) // set the maximum length
				*m = i;
		}

		if (root.sym == opsym::_notAnOperation)
		{
			NMC_ERROR(toks[0], "this operation isn't complete");
			NMC_NOTE("often caused by two operations back to back (1+ +1),");
			NMC_NOTE("or two values back to back (1 1 +)");
			NMC_NOTE("it's bad, just leave it at that");
		}

		return root;
	}

	Operation::opsym Operation::getOpsym()
	{
		return sym;
	}

	Any Operation::getLeft()
	{
		return left;
	}

	Any Operation::getRight()
	{
		return right;
	}

	bool Operation::ok()
	{
		return (sym != _notAnOperation);
	}

	Operation &Operation::parse(vector<Token> toks, int &off)
	{
		int i = 0;
		vector<Token> tmp(toks.begin()+off, toks.end());
		Operation rtn = parseBase(tmp, &i);
		off = i;
		*this = rtn;
		return *this;
	}

	string Operation::display()
	{
		stringstream ss;
		ss << "(" << sym << " " << left.display() << " " << right.display() << ")\n";
		return ss.str();
	}

	vector<Expression::typeQualifier> Expression::getTypes()
	{
		return types;
	}

	Token Expression::getSymbol()
	{
		return symbol;
	}

	ComplexValue Expression::getComplexValueArgument()
	{
		return cva;
	}

	vector<Any> Expression::getArguments()
	{
		return args;
	}

	vector<Any> Expression::getBody()
	{
		return body;
	}

	vector<Any> Expression::getWhereBody()
	{
		return where;
	}

	Expression::typeQualifier Expression::parseType(Token tok)
	{
		if (tok == "bool")
			return typeQualifier::_bool;
		else if (tok == "byte")
			return typeQualifier::_byte;
		else if (tok == "int")
			return typeQualifier::_int;
		else if (tok == "float")
			return typeQualifier::_float;
		else if (tok == "ref")
			return typeQualifier::_ref;
		else if (tok == "long")
			return typeQualifier::_long;
		else if (tok == "unsigned")
			return typeQualifier::_unsigned;
		else if (tok == "const")
			return typeQualifier::_const;
		else if (tok == "void")
			return typeQualifier::_void;
		else if (tok == "class")
			return typeQualifier::_class;
		else if (tok == "abstract")
			return typeQualifier::_abstract;
		else if (tok == "private")
			return typeQualifier::_private;
		else if (tok == "protected")
			return typeQualifier::_protected;
		else if (tok == "public")
			return typeQualifier::_public;
		else if (tok == "variadic")
			return typeQualifier::_variadic;
		else if (tok == "vector")
			return typeQualifier::_vector;
		else
		{
			NMC_ERROR(tok, "this isn't a type qualifier (" << tok.getString() << ")");
			NMC_NOTE("available: bool, byte, int, float,");
			NMC_NOTE("ref, long, unsigned, const, void,");
			NMC_NOTE("class, abstract, private, protected,");
			NMC_NOTE("public, variadic");
		}
	}

	bool Expression::ok()
	{
		return !symbol.getString().empty();
	}

	Expression &Expression::parse(vector<Token> toks, int &off)
	{
		while(off < toks.size() && toks[off].getType() == Token::category::type)
		{
			typeQualifier tmp = parseType(toks[off]);
			types.push_back(tmp);
			off++;
		}

		if (off < toks.size() && (toks[off] == ")" || toks[off] == "]" || toks[off] == "}" || toks[off] == "{" || toks[off] == "[" || toks[off] == "("))
		{
			return *this;
		}
		
		if (toks[off].getType() != Token::category::symbol)
		{
			NMC_ERROR(toks[off], "expected a symbol in this expression");
			NMC_NOTE("instead got " << toks[off].display());
			return *this;
		}

		symbol = toks[off];

		off++;

		if (off < toks.size() && (toks[off] == ")" || toks[off] == "]" || toks[off] == "}"))
		{
			return *this;
		}

		if (off < toks.size() && toks[off] == "[")
		{
			cva = ComplexValue().parse(toks, off);
			off++;
		}

		if (off < toks.size() && (toks[off] == ")" || toks[off] == "]" || toks[off] == "}"))
		{
			cout << this->display() << "\n";
			return *this;
		}

		if (off < toks.size() && toks[off] == "(")
		{
			off++;
			for (; off < toks.size(); off++)
			{
				Any tmp = Any().parse(toks, off);
				args.push_back(tmp);
				if (off < toks.size() && toks[off] == ",")
				{
				}
				else if (off < toks.size() && toks[off] == ")")
				{
					break;
				}
				else
				{
				}
			}

			off++;
		}

		if (off < toks.size() && (toks[off] == ")" || toks[off] == "]" || toks[off] == "}"))
		{
			return *this;
		}

		if (off < toks.size() && toks[off] == "{")
		{
			off++;

			for (; off < toks.size(); off++)
			{
				body.push_back(Any().parse(toks, off));
				if (off < toks.size() && toks[off] == ";")
				{
				}
				else if (off < toks.size() && toks[off] == "}")
				{
					break;
				}
				else
				{
				}
			}

			off++;
		}

		if (off < toks.size() && (toks[off] == ")" || toks[off] == "]" || toks[off] == "}"))
		{
			return *this;
		}

		if (off < toks.size() && toks[off] == "where")
		{
			off++;

			for (; off < toks.size(); off++)
			{
				where.push_back(Any().parse(toks, off));
				if (off < toks.size() && toks[off] == ";")
				{
				}
				else if (off < toks.size() && toks[off] == "}")
				{
					break;
				}
				else
				{
				}
			}

			off++;
		}

		return *this;
	}

	string Expression::display()
	{
		stringstream ss;
		ss << "types: ";
		for (vector<typeQualifier>::iterator i = types.begin(); i != types.end(); i++)
			ss << *i << " ";
		ss << "\nsymbol: " << symbol.display() << "\n";
		ss << "cva: " << cva.display() << "\n";
		for (int i = 0; i < args.size(); i++)
			ss << "args[" << i << "]: " << args[i].display() << "\n";
		for (int i = 0; i < body.size(); i++)
			ss << "body[" << i << "]: " << body[i].display() << "\n";
		for (int i = 0; i < where.size(); i++)
			ss << "where[" << i << "]: " << where[i].display() << "\n";
		return ss.str();
	}

	vector<Any> parse(vector<Token> toks)
	{
		vector<Any> rtn;
		int i = 0;
		while(i < toks.size()-1)
		{
			rtn.push_back(Any().parse(toks, i));
			if (toks[i] == ";")
			{
				i++;
			}
		}

		return rtn;
	}
}

