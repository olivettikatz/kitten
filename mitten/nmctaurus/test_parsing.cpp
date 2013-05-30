#include <unistd.h>
#include "parsing.h"

using namespace nmc;

void displayErrors(vector<Error> ebuf)
{
	if (ebuf.size() > 1)
		cout << "there were " << ebuf.size() << " errors...\n";
	else if (ebuf.size() == 1)
		cout << "there was an error...\n";
	for (vector<Error>::iterator i = ebuf.begin(); i != ebuf.end(); i++)
		cout << "error in --@" << i->getSource().getLine() << ":" << i->getSource().getColumn() << " ('" << i->getSource().get() << "') " << i->getExpectationID() << " expected " << i->getExpectedType() << "\n";
	if (ebuf.empty() == false)
		_exit(1);
}

int main()
{
	Tokenizer t = createTokenizer();
	Parser p = Parser();
	p.createParser();

	vector<Token> iasm_toks = t.tokenize("asm\n{\n\tmovi eax 2;\n}\n");
	for (vector<Token>::iterator i = iasm_toks.begin(); i != iasm_toks.end(); i++)
		cout << "'" << i->get() << "'\n";

	AST iasm_ast = p.parse(iasm_toks);
	displayErrors(p.getErrors());
	cout << "result: " << iasm_ast.display() << "\n";

	p.createParser();

	vector<Token> expr_toks = t.tokenize("int main(@vector(string) args)\n{\tprint(\"hello, world\\n\");\n}\n");
	for (vector<Token>::iterator i = expr_toks.begin(); i != expr_toks.end(); i++)
		cout << "'" << i->get() << "'\n";

	AST expr_ast = p.parse(expr_toks);
	displayErrors(p.getErrors());
	cout << "result: " << expr_ast.display() << "\n";

	return 0;
}
