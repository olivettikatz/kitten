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

	vector<Token> exprtoks = Lexer().lex("int main\n");

	Expression expr = Expression();
	i = 0;
	expr.parse(exprtoks, i);

	return 0;
}
