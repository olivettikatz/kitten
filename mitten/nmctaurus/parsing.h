#ifndef __NMC_PARSING_H
#define __NMC_PARSING_H

#include <iostream>
#include <ktp/ktp.h>

namespace nmc
{
	using namespace std;
	using namespace ktp;

	Tokenizer createTokenizer();
	Parser createParser(Tokenizer &t);
}

#endif
