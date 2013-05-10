#include <iostream>
#include "data.h"
#include "lexer.h"
#include "parser.h"

using namespace std;
using namespace nmc;

int main(int argc, char *argv[])
{
	enableDeveloperInfo = true;
	SymbolTable st = SymbolTable();

	string page = readEncoded(argv[1], encoding::UTF8);
	vector<Token> toks = Lexer().lex(page);

	string oppage = "(1+2)+3\n";
	vector<Token> optoks = Lexer().lex(oppage);
	initErrors("oppage.n", oppage);

	Operation op = Operation();
	int i = 0;
	op.parse(optoks, i, st);
	cout << op.display() << "\n";

	dieIfErrors();

	string exprpage = "int main(vector args){f(5);f(2)}where{}\n";
	vector<Token> exprtoks = Lexer().lex(exprpage);
	initErrors("exprpage.n", exprpage);

	Expression expr = Expression();
	i = 0;
	expr.parse(exprtoks, i, st);
	cout << expr.display() << "\n";

	Any any = Any();
	i = 0;
	any.parse(optoks, i, st);
	cout << any.display() << "\n";
	i = 0;
	any.parse(exprtoks, i, st);
	cout << any.display() << "\n";

	dieIfErrors();
	
	string declpage = "int x = 5\n";
	vector<Token> decltoks = Lexer().lex(declpage);
	initErrors("declpage.n", declpage);
	i = 0;
	any.parse(decltoks, i, st);
	cout << any.display() << "\n";

	dieIfErrors();

	return 0;
}
