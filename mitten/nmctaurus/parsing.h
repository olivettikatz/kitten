#ifndef __NMC_PARSING_H
#define __NMC_PARSING_H

#include <iostream>
#include "taurus/taurus.h"

namespace nmc
{
	using namespace std;
	using namespace taurus;

	Tokenizer createTokenizer();
	Parser createParser(Tokenizer &t);
}

#endif
