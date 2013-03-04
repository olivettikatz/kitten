#include "builder.h"

namespace nightowl
{
	using namespace std;

	opcode BuilderInstruction::getOp()
	{
		return op;
	}

	unsigned char BuilderInstruction::getReg0()
	{
		return reg0;
	}

	unsigned char BuilderInstruction::getReg1()
	{
		return reg1;
	}

	int BuilderInstruction::getArg()
	{
		return arg;
	}

	BuilderInstructionPage::iterator BuilderInstructionPage::function(string n)
	{
		if (functable.find(n) == functable.end())
			return (functable[n] = content.begin()+content.size());
		else
			return functable[n];
	}

	vector<BuilderInstruction> BuilderInstructionPage::getFunctionSegment(string n)
	{
		if (functable.find(n) == functable.end())
		{
			vector<BuilderInstruction> rtn;
			return rtn;
		}

		iterator s = functable[n];
		iterator e = end();
		for (map<string, iterator>::iterator i = functable.begin(); i != functable.end(); i++)
			if (i->second < e)
				e = i->second;
		vector<BuilderInstruction> rtn(s, e);
		return rtn;
	}

	BuilderInstructionPage &BuilderInstructionPage::operator << (BuilderInstruction i)
	{
		content.push_back(i);
		return *this;
	}

	BuilderInstructionPage &BuilderInstructionPage::operator << (BuilderInstructionPage p)
	{
		content.insert(content.end(), p.begin(), p.end());
	}

	BuilderInstructionPage::iterator BuilderInstructionPage::begin()
	{
		return content.begin();
	}

	BuilderInstructionPage::iterator BuilderInstructionPage::end()
	{
		return content.end();
	}

	size_t BuilderInstructionPage::size()
	{
		return content.size();
	}

	unsigned char BuilderInstructionPage::getLastReg()
	{
		return content.back().getReg0();
	}

	Builder::Builder()
	{
		enableOutputToPage = false;
		_jit = o_jit_init();
		_page = BuilderInstructionPage();
		
		for (int i = 0; i < O_REGISTER_HB; i++)
		{
			regs[i].mode = free;
			regs[i].value = 0;
		}

		for (int i = 0; i < O_FPST_SIZE; i++)
		{
			fpst[i].dirty = false;
			fpst[i].ref = 0x00;
		}
		fpstUsed = 0;
	}

	Builder::~Builder()
	{
		o_jit_release(&_jit);
	}

	void Builder::outputToPage()
	{
		enableOutputToPage = true;
		o_jit_release(&_jit);
	}

	void Builder::emit(opcode o, unsigned char r0, unsigned char r1, int a)
	{
		if (enableOutputToPage)
			_page << BuilderInstruction(o, r0, r1, a);
		else
			emitters[o](&_jit, r0, r1, a);
	}

	void Builder::emit(BuilderInstruction i)
	{
		if (enableOutputToPage)
			_page << i;
		else
			emit(i.getOp(), i.getReg0(), i.getReg1(), i.getArg());
	}

	void Builder::emit(BuilderInstructionPage p)
	{
		if (enableOutputToPage)
			_page << p;
		else
		{
			for (BuilderInstructionPage::iterator i = p.begin(); i != p.end(); i++)
				emit(*i);
		}
	}

	o_jit *Builder::jit()
	{
		return &_jit;
	}

	BuilderInstructionPage Builder::page()
	{
		return _page;
	}

	int Builder::label()
	{
		if (enableOutputToPage)
			return _page.size();
		else
			return o_jit_label(&_jit);
	}

	int Builder::emitFunction()
	{
		int tmp = label();
		ap = O_AP_INIT;
		emit(_pusharg, O_EBP, 0, 0);
		emit(_mov, O_EBP, O_ESP, 0);
		return tmp;
	}

	unsigned char Builder::emitFunctionArgument(unsigned int s)
	{
		unsigned char r = regAlloc(true);
		emit(_mov_disp, r, O_EBP, (int)ap);
		ap += s;
		return r;
	}

	void Builder::emitFunctionReturn()
	{
		if (regDirty(O_EAX) == false)
			emit(_const, O_EAX, 0, regValue(O_EAX));
		emit(_leave, 0, 0, 0);
		emit(_ret, 0, 0, 0);
	}

	unsigned int Builder::emitRelocatableBranch(opcode br, unsigned int target)
	{
		relocation relo;
		relo.o = br;
		relo.from = label();
		relo.to = target;
		relocations.push_back(relo);
		emit(br, 0, 0, 0);
		return relocations.size()-1;
	}

	unsigned int Builder::emitRelocatableJump(unsigned int target)
	{
		relocation relo;
		relo.o = _jmp;
		relo.from = label();
		relo.to = target;
		relocations.push_back(relo);
		emit(_jmp, 0, 0, 0);
		return relocations.size()-1;
	}

	void Builder::reemitRelocations()
	{
		for (vector<relocation>::iterator i = relocations.begin(); i != relocations.end(); i++)
		{
			if (enableOutputToPage)
			{
				_page[i->from] = BuilderInstruction(i->o, 0, 0, relocations[i->to].from);
			}
			else
			{
				int tmp = label();
				o_jit_insert(&_jit, i->from);
				emit(i->o, 0, 0, relocations[i->to].from);
				o_jit_insert(&_jit, tmp);
			}
		}
	}

	int Builder::emitIfCondition(unsigned char r, int v, opcode o)
	{
		emit(_cmp, r, 0, v);
		int l = label();
		emit(o, 0, 0, 0);
		ifStackElement e = {
			o,
			l,
			-1
		};
		ifStack.push_back(e);
		return l;
	}

	int Builder::emitIfCondition(unsigned char r)
	{
		return emitIfCondition(r, 1, _brlt);
	}

	int Builder::emitIfElse()
	{
		int tmp = label();
		for (vector<ifStackElement>::reverse_iterator i = ifStack.rbegin(); i != ifStack.rend(); i++)
		{
			if (i->lelse == -1)
			{
				i->lelse = tmp;
				break;
			}
		}
		emit(_jmp, 0, 0, 0);
		return tmp;
	}

	int Builder::emitIfEndif(int lif, opcode o)
	{
		ifStack.pop_back();
		int tmp = label();
		o_jit_insert(&_jit, lif);
		emit(o, 0, 0, tmp);
		o_jit_insert(&_jit, tmp);
		return tmp;
	}

	int Builder::emitIfEndif(int lif)
	{
		return emitIfEndif(lif, _brlt);
	}

	int Builder::emitIfEndif(int lif, int lelse, opcode o)
	{
		ifStack.pop_back();
		int tmp = label();
		o_jit_insert(&_jit, lif);
		emit(o, 0, 0, lelse);
		o_jit_insert(&_jit, lelse);
		emit(_jmp, 0, 0, tmp);
		o_jit_insert(&_jit, tmp);
		return tmp;
	}

	int Builder::emitIfEndif(int lif, int lelse)
	{
		return emitIfEndif(lif, lelse, _brlt);
	}

	int Builder::emitIfEndif()
	{
		if (ifStack.back().lelse == -1)
			return emitIfEndif(ifStack.back().lif, ifStack.back().branch);
		else
			return emitIfEndif(ifStack.back().lif, ifStack.back().lelse, ifStack.back().branch);
	}

	unsigned char Builder::regAlloc(bool d)
	{
		for (int i = O_REGISTER_HB-1; i > 0; i--)
		{
			if (regs[i].mode == free)
			{
				if (d)
					regs[i].mode = dirty;
				else
					regs[i].mode = pure;
				regs[i].value = 0;
				return (unsigned char)i;
			}
		}

		return O_REGISTER_HB;
	}

	bool Builder::regIsAllocated(unsigned char r)
	{
		return (regs[r].mode == free);
	}

	int Builder::regValue(unsigned char r, int v)
	{
		return (regs[r].value = v);
	}

	int Builder::regValue(unsigned char r)
	{
		return regs[r].value;
	}

	bool Builder::regDirty(unsigned char r, bool d)
	{
		if (d)
			regs[r].mode = dirty;
		else
			regs[r].mode = pure;
		return d;
	}

	bool Builder::regDirty(unsigned char r)
	{
		if (regs[r].mode == dirty)
			return true;
		return false;
	}

	void Builder::regFree(unsigned char r)
	{
		regs[r].mode = free;
	}

	void Builder::emitReg(unsigned char r)
	{
		if (regDirty(r) == false)
			emit(_const, r, 0, regValue(r));
	}

	bool Builder::fpDirty(unsigned int i)
	{
		return fpst[fpstUsed-i].dirty;
	}

	bool Builder::fpDirty(unsigned int i, bool d)
	{
		return (fpst[fpstUsed-i].dirty = d);
	}

	double Builder::fpValue(unsigned int i)
	{
		return *(fpst[fpstUsed-i].ref);
	}

	double Builder::fpValue(unsigned int i, double v)
	{
		return (*(fpst[fpstUsed-i].ref) = v);
	}

	double *Builder::fpRef(unsigned int i)
	{
		return fpst[fpstUsed-i].ref;
	}

	void Builder::emitFp()
	{
		if (fpDirty(0) == false)
			emit(_fld, 0, 0, (int)fpRef(0));
	}

	unsigned char Builder::emitConstI32(int v)
	{
		unsigned char r = regAlloc(false);
		regValue(r, v);
		return r;
	}

	unsigned char Builder::emitRefI32(int *v)
	{
		unsigned char r = regAlloc(true);
		unsigned char tmp = regAlloc(true);
		emit(_const, tmp, 0, (int)v);
		emit(_ld, r, tmp, 0);
		regFree(tmp);
		return r;
	}

	void Builder::emitPullI32(unsigned char r, int *v)
	{
		if (regDirty(r))
		{
			unsigned char tmp = regAlloc(true);
			emit(_const, tmp, 0, (int)v);
			emit(_st, r, tmp, 0);
			regFree(tmp);
		}
		else
		{
			unsigned char tmp = regAlloc(true);
			emit(_const, r, 0, regValue(r));
			emit(_const, tmp, 0, (int)v);
			emit(_st, r, tmp, 0);
			regFree(tmp);
		}
	}

	void Builder::emitRefF64(double *v)
	{
		fpst[fpstUsed].dirty = false;
		fpst[fpstUsed].ref = v;
	}

	void Builder::emitPullF64()
	{
		if (fpst[fpstUsed].dirty)
			emit(_fld, 0, 0, (int)fpst[fpstUsed].ref);
	}

	unsigned char Builder::emitOperation(opcode o, unsigned char r0, unsigned char r1)
	{
		if (regDirty(r0) == false && regDirty(r1) == false)
		{
			switch(o)
			{
				case _add: regValue(r0, regValue(r0)+regValue(r1)); break;
				case _sub: regValue(r0, regValue(r0)-regValue(r1)); break;
				case _mul: regValue(r0, regValue(r0)*regValue(r1)); break;
				case _and: regValue(r0, regValue(r0)&regValue(r1)); break;
				case _or: regValue(r0, regValue(r0)|regValue(r1)); break;
				case _xor: regValue(r0, regValue(r0)^regValue(r1)); break;
				default: emit(_const, r0, 0, regValue(r0));
				         emit(_const, r1, 0, regValue(r1));
				         emit(o, r0, r1, 0);
			}
		}
		else
		{
			regDirty(r0, true);
			emit(_const, r0, 0, regValue(r0));
			emit(_const, r1, 0, regValue(r1));
			emit(o, r0, r1, 0);
			return r0;
		}
	}

	void Builder::emitFloatingOperation(opcode o)
	{
		if (o >= _fadd && o <= _fdiv)
		{
			if (fpDirty(0) == false && fpDirty(1))
			{
				if (o == _fadd)
					fpValue(0, fpValue(0)+fpValue(1));
				else if (o == _fsub)
					fpValue(0, fpValue(0)-fpValue(1));
				else if (o == _fmul)
					fpValue(0, fpValue(0)*fpValue(1));
				else if (o == _fdiv)
					fpValue(0, fpValue(0)/fpValue(1));
			}
			else
			{
				if (fpDirty(0) == false)
					emit(_fld, 0, 0, (int)fpRef(0));
				fpDirty(0, true);
				if (fpDirty(1) == false)
					emit(_fld, 0, 0, (int)fpRef(1));
				unsigned char tmp0 = regAlloc(true);
				emit(_const, tmp0, 0, 0);
				unsigned char tmp1 = regAlloc(true);
				emit(_const, tmp1, 0, 1);
				emit(o, tmp0, tmp1, 0);
			}
		}
		else if (o == _fchs)
		{
			if (fpDirty(0))
				emit(_fchs, 0, 0, 0);
			else
				fpValue(0, -fpValue(0));
		}
		else if (o == _ftan)
		{
			if (fpDirty(0))
				emit(_ftan, 0, 0, 0);
			else
				fpValue(0, tan(fpValue(0)));
		}
		else if (o == _fatan)
		{
			if (fpDirty(0))
				emit(_fatan, 0, 0, 0);
			else
				fpValue(0, atan(fpValue(0)));
		}
		else if (o == _fsin)
		{
			if (fpDirty(0))
				emit(_fsin, 0, 0, 0);
			else
				fpValue(0, sin(fpValue(0)));
		}
		else if (o == _fcos)
		{
			if (fpDirty(0))
				emit(_fcos, 0, 0, 0);
			else
				fpValue(0, cos(fpValue(0)));
		}
		else if (o == _fsincos)
		{
			if (fpDirty(0))
				emit(_fsincos, 0, 0, 0);
			else
			{
				fpValue(0, sin(fpValue(0)));
				fpValue(1, cos(fpValue(0)));
			}
		}
		else if (o == _fsqrt)
		{
			if (fpDirty(0))
				emit(_fsqrt, 0, 0, 0);
			else
				fpValue(0, sqrt(fpValue(0)));
		}
	}

	void Builder::emitPushArg(unsigned char r)
	{
		if (regDirty(r) == false)
			emit(_const, r, 0, regValue(r));
		emit(_pusharg, r, 0, 0);
	}

	void Builder::emitPopArg(unsigned char r)
	{
		regDirty(r, true);
		emit(_poparg, r, 0, 0);
	}
}

