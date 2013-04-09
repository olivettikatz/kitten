#include "iasm.h"

iasm_reg parse_iasm_reg(token t)
{
	if (tokcmp(t, "eax") == 0)
		return eax;
	else if (tokcmp(t, "ecx") == 0)
		return ecx;
	else if (tokcmp(t, "edx") == 0)
		return edx;
	else if (tokcmp(t, "ebx") == 0)
		return ebx;
	else if (tokcmp(t, "esp") == 0)
		return esp;
	else if (tokcmp(t, "ebp") == 0)
		return ebp;
	else if (tokcmp(t, "esi") == 0)
		return esi;
	else if (tokcmp(t, "edi") == 0)
		return edi;
	else if (tokcmp(t, "ax") == 0)
		return ax;
	else if (tokcmp(t, "cx") == 0)
		return cx;
	else if (tokcmp(t, "dx") == 0)
		return dx;
	else if (tokcmp(t, "bx") == 0)
		return bx;
	else if (tokcmp(t, "al") == 0)
		return al;
	else if (tokcmp(t, "cl") == 0)
		return cl;
	else if (tokcmp(t, "bl") == 0)
		return bl;
	else
	{
		// error
	}
}

int parse_iasm_tok2int(token t, symtable *st)
{
	if (t.cat == VALUE)
	{
		char *tmp = strndup(t.str, t.strsize);
		int rtn = atoi(tmp);
		free(tmp);
		return rtn;
	}
	else if (t.cat == SYMBOL)
	{
		char *tmp = strndup(t.str, t.strsize);
		int q = symtable_search(st, tmp);
		free(tmp);
		if (q == -1)
		{
			// error
		}
		else
		{
			return *(int *)(st->buf[q].heapaddr);
		}
	}
	else
	{
		// error
	}
}

int parse_iasm_tok2addr(token t, symtable *st)
{
	if (t.cat == VALUE)
	{
		char *tmp = strndup(t.str, t.strsize);
		int rtn = atoi(tmp);
		free(tmp);
		return rtn;
	}
	else if (t.cat == SYMBOL)
	{
		char *tmp = strndup(t.str, t.strsize);
		int q = symtable_search(st, tmp);
		free(tmp);
		if (q == -1)
		{
			// error
		}
		else
		{
			return (int)(st->buf[q].heapaddr);
		}
	}
	else
	{
		// error
	}
}

iasm *parse_iasm(token *toks, symtable *st)
{
	for (int i = 0; toks[i].str != NULL; i++)
	{
		iasm tmp;
		tmp.inst = nop;
		tmp.r0 = eax;
		tmp.r1 = eax;
		tmp.val = 0;
		if (tokcmp(toks[i], "nop") == 0)
			tmp.inst = nop;
		else if (tokcmp(toks[i], "const32") == 0)
		{
			tmp.inst = const32;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.val = parse_iasm_tok2int(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "mov8") == 0)
		{
			tmp.inst = mov8;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "mov16") == 0)
		{
			tmp.inst = mov16;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "mov32") == 0)
		{
			tmp.inst = mov32;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "mov32_disp") == 0)
		{
			tmp.inst = mov32_disp;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
			tmp.val = parse_iasm_tok2int(toks[i+3], st);
		}
		else if (tokcmp(toks[i], "ld8") == 0)
		{
			tmp.inst = ld8;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.val = parse_iasm_tok2addr(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "ld16") == 0)
		{
			tmp.inst = ld16;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.val = parse_iasm_tok2addr(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "ld32") == 0)
		{
			tmp.inst = ld32;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.val = parse_iasm_tok2addr(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "st8") == 0)
		{
			tmp.inst = st8;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.val = parse_iasm_tok2addr(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "st16") == 0)
		{
			tmp.inst = st16;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.val = parse_iasm_tok2addr(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "st32") == 0)
		{
			tmp.inst = st32;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.val = parse_iasm_tok2addr(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "add8") == 0)
		{
			tmp.inst = add8;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "add16") == 0)
		{
			tmp.inst = add16;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "add32") == 0)
		{
			tmp.inst = add32;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "sub8") == 0)
		{
			tmp.inst = sub8;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "sub16") == 0)
		{
			tmp.inst = sub16;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "sub32") == 0)
		{
			tmp.inst = sub32;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "mul32") == 0)
		{
			tmp.inst = mul32;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "and8") == 0)
		{
			tmp.inst = and8;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "and16") == 0)
		{
			tmp.inst = and16;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "and32") == 0)
		{
			tmp.inst = and32;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "or8") == 0)
		{
			tmp.inst = or8;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "or16") == 0)
		{
			tmp.inst = or16;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "or32") == 0)
		{
			tmp.inst = or32;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "xor8") == 0)
		{
			tmp.inst = xor8;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "xor16") == 0)
		{
			tmp.inst = xor16;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "xor32") == 0)
		{
			tmp.inst = xor32;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "pusharg32") == 0)
		{
			tmp.inst = pusharg32;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
		}
		else if (tokcmp(toks[i], "poparg32") == 0)
		{
			tmp.inst = poparg32;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
		}
		else if (tokcmp(toks[i], "leave") == 0)
			tmp.inst = leave;
		else if (tokcmp(toks[i], "ret") == 0)
			tmp.inst = ret;
		else if (tokcmp(toks[i], "int32") == 0)
		{
			tmp.inst = int32;
			tmp.val = parse_iasm_tok2int(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "cmp8") == 0)
		{
			tmp.inst = cmp8;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.val = parse_iasm_tok2int(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "cmp16") == 0)
		{
			tmp.inst = cmp16;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.val = parse_iasm_tok2int(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "cmp32") == 0)
		{
			tmp.inst = cmp32;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.val = parse_iasm_tok2int(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "breq8") == 0)
		{
			tmp.inst = breq8;
			tmp.val = parse_iasm_tok2int(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "breq32") == 0)
		{
			tmp.inst = breq32;
			tmp.val = parse_iasm_tok2int(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "brne8") == 0)
		{
			tmp.inst = brne8;
			tmp.val = parse_iasm_tok2int(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "brne32") == 0)
		{
			tmp.inst = brne32;
			tmp.val = parse_iasm_tok2int(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "brlt8") == 0)
		{
			tmp.inst = brlt8;
			tmp.val = parse_iasm_tok2int(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "brlt32") == 0)
		{
			tmp.inst = brlt32;
			tmp.val = parse_iasm_tok2int(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "brle8") == 0)
		{
			tmp.inst = brle8;
			tmp.val = parse_iasm_tok2int(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "brle32") == 0)
		{
			tmp.inst = brle32;
			tmp.val = parse_iasm_tok2int(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "brgt8") == 0)
		{
			tmp.inst = brgt8;
			tmp.val = parse_iasm_tok2int(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "brgt32") == 0)
		{
			tmp.inst = brgt32;
			tmp.val = parse_iasm_tok2int(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "brge8") == 0)
		{
			tmp.inst = brge8;
			tmp.val = parse_iasm_tok2int(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "brge32") == 0)
		{
			tmp.inst = brge32;
			tmp.val = parse_iasm_tok2int(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "jmp8") == 0)
		{
			tmp.inst = jmp8;
			tmp.val = parse_iasm_tok2int(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "jmp32") == 0)
		{
			tmp.inst = jmp32;
			tmp.val = parse_iasm_tok2int(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "call32") == 0)
		{
			tmp.inst = call32;
			tmp.val = parse_iasm_tok2int(toks[i+2], st);
		}
		else if (tokcmp(toks[i], "fld32") == 0)
		{
			tmp.inst = fld32;
			tmp.val = parse_iasm_tok2addr(toks[i+1], st);
		}
		else if (tokcmp(toks[i], "fst32") == 0)
		{
			tmp.inst = fst32;
			tmp.val = parse_iasm_tok2addr(toks[i+1], st);
		}
		else if (tokcmp(toks[i], "fadd") == 0)
		{
			tmp.inst = fadd;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "fsub") == 0)
		{
			tmp.inst = fsub;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "fmul") == 0)
		{
			tmp.inst = fmul;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "fdiv") == 0)
		{
			tmp.inst = fdiv;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "fcmp") == 0)
		{
			tmp.inst = fcmp;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
			tmp.r1 = parse_iasm_reg(toks[i+2]);
		}
		else if (tokcmp(toks[i], "fchs") == 0)
		{
			tmp.inst = fchs;
			tmp.r0 = parse_iasm_reg(toks[i+1]);
		}
		else if (tokcmp(toks[i], "fst_rot") == 0)
			tmp.inst = fst_rot;
		else if (tokcmp(toks[i], "fnop") == 0)
			tmp.inst = fnop;
		else if (tokcmp(toks[i], "ftan") == 0)
			tmp.inst = ftan;
		else if (tokcmp(toks[i], "fatan") == 0)
			tmp.inst = fatan;
		else if (tokcmp(toks[i], "fsin") == 0)
			tmp.inst = fsin;
		else if (tokcmp(toks[i], "fcos") == 0)
			tmp.inst = fcos;
		else if (tokcmp(toks[i], "fsincos") == 0)
			tmp.inst = fsincos;
		else if (tokcmp(toks[i], "fsqrt") == 0)
			tmp.inst = fsqrt;
		else if (tokcmp(toks[i], "label") == 0)
		{
			void *tmp = malloc(sizeof(int));
			*(int *)tmp = i;
			symtable_append(st, toks[i+1], tmp);
		}
		else
		{
			// error
		}

		
	}
}
