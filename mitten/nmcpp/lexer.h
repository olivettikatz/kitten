#ifndef __NMC_LEXER_H
#define __NMC_LEXER_H

#include <iostream>
#include <vector>
#include "data.h"

namespace nmc
{
	using namespace std;

	class Lexer
	{
	private:
		Token::category castSpecialSymbols(Token t);
		void safePush(vector<Token> &rtn, Token tok);

	public:
		vector<Token> lex(string buf);
	};
}

#endif
