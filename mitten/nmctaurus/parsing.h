#ifndef __NMC_PARSING_H
#define __NMC_PARSING_H

#include <iostream>
#include <parsing/parsing.h>

namespace nmc
{
	using namespace std;
	using namespace parsing;

	Tokenizer createTokenizer();

	class Parser
	{
	private:
		OneDB odb;
		Parallel symbolOrValueInt;
		Many internalAssemblyArgs;
		Sequence oneInternalAssemblyLine;
		Many internalAssemblyLines;
		Sequence internalAssemblyExpression;
		Many typeQualifiers;
		Sequence typeExpression;
		Parallel any;
		Sequence oneComplexValueElement;
		Many complexValueElements;
		Sequence complexValue;
		Parallel operatorUnaryLeft;
		Parallel operatorUnaryRight;
		Parallel operatorBinary;
		Sequence operationUnaryLeft;
		Sequence operationUnaryRight;
		Sequence operationBinary;
		Parallel anyUnboundedOperation;
		Parallel operation;
		Sequence anyBoundedOperation;
		Sequence oneArgumentVectorElement;
		Many argumentVectorElements;
		Sequence argumentVectorNonNull;
		Sequence argumentVectorNull;
		Parallel argumentVectorContents;
		Sequence argumentVector;
		Sequence oneLine;
		Many lines;
		Sequence body;
		Maybe maybeType;
		Maybe maybeComplex;
		Maybe maybeArgument;
		Maybe maybeBody;
		Sequence where;
		Maybe maybeWhere;
		Sequence expression;
		Many page;

		vector<Error> ebuf;

	public:
		Parser() {}
		void createParser();
		AST parse(vector<Token> toks);
		vector<Error> getErrors();
		void clearErrors();
	};
}

#endif
