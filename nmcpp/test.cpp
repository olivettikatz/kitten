#include <iostream>
#include "data.h"
#include "lexer.h"
#include "parser.h"

using namespace std;
using namespace nmc;

int main(int argc, char *argv[])
{
	string page = readEncoded(argv[1], encoding::UTF8);
	vector<Token> toks = Lexer().lex(page);

	vector<Token> optoks = Lexer().lex("(1+2)+3\n");

	Operation op = Operation();
	int i = 0;
	op.parse(optoks, i);
	cout << op.display() << "\n";

	return 0;
}
