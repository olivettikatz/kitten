#include "parsing.h"

using namespace nmc;

string page = "import(\"std.n\");\
			   \
			   print(\"hello, world\");\n";

int main()
{
	Tokenizer t = createTokenizer();
	Parser p = createParser(t);

	vector<Token> toks = t.tokenize(page);
	for (vector<Token>::iterator i = toks.begin(); i != toks.end(); i++)
		cout << "'" << i->get() << "'\n";

	return 0;
}
