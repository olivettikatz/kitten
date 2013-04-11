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

	vector<Token> exprtoks = Lexer().lex("int main(vector args){f(5);f(2)}where{}\n");

	Expression expr = Expression();
	i = 0;
	expr.parse(exprtoks, i);
	cout << expr.display() << "\n";

	Any any = Any();
	i = 0;
	any.parse(optoks, i);
	cout << any.display() << "\n";
	i = 0;
	any.parse(exprtoks, i);
	cout << any.display() << "\n";

	vector<Token> decltoks = Lexer().lex("int x = 5\n");
	i = 0;
	any.parse(decltoks, i);
	cout << any.display() << "\n";

	return 0;
}
