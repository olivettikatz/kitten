#ifndef __NMC_PARSING_H
#define __NMC_PARSING_H

#include <iostream>
#include <parsing/parsing.h>

namespace nmc
{
	using namespace std;
	using namespace parsing;

	Tokenizer createTokenizer();
	Parser createParser(Tokenizer &t);
}

#endif
