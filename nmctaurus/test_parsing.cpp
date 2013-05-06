#include "parsing.h"

using namespace nmc;

int main()
{
	Tokenizer t = createTokenizer();
	Parser p = createParser(t);
	return 0;
}
