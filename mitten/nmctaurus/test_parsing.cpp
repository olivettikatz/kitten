#include "parsing.h"

using namespace nmc;

int main()
{
	Tokenizer t = createTokenizer();
	Parser p = createParser(t);

	vector<Token> iasm_toks = t.tokenize("asm\n{\n\tmovi eax 2;\n}\n");
	for (vector<Token>::iterator i = iasm_toks.begin(); i != iasm_toks.end(); i++)
		cout << "'" << i->get() << "'\n";

	unsigned int off = 0;
	vector<Error> ebuf;
	AST iasm_ast = p.parse("InternalAssemblyExpression", iasm_toks, off, ebuf);
	for (vector<Error>::iterator i = ebuf.begin(); i != ebuf.end(); i++)
		cout << "error in --@" << i->getSource().getLine() << ":" << i->getSource().getColumn() << " ('" << i->getSource().get() << "') " << i->getExpectationID() << " expected " << i->getExpectedType() << "\n";
	cout << "result: " << iasm_ast.display() << "\n";

	return 0;
}
