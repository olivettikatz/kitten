#include "parsing.h"

namespace nmc
{
	Tokenizer createTokenizer()
	{
		Tokenizer t = Tokenizer();
		t.noDelim(t() == "\"", t() == "\"");
		t.noDelim(t() == "'", t() == "'");
		t.skip(t() == "//", t() == "\n" || t() == "\r");
		t.skip(t() == "#", t() == "\n" || t() == "\r");
		t.skip(t() == "/*", t() == "*/");
		t.whitespace(t() == " ");
		t.whitespace(t() == "\t");
		t.whitespace(t() == "\r");
		t.whitespace(t() == "\n");
		t.categorizer("Where", t() == "where");
		t.categorizer("InternalAssemblySymbol", t() == "asm");
		t.token("BoundaryBeginExpression", t() == "(");
		t.token("BoundaryEndExpression", t() == ")");
		t.token("BoundaryBeginComplex", t() == "[");
		t.token("BoundaryEndComplex", t() == "]");
		t.token("BoundaryBeginScope", t() == "{");
		t.token("BoundaryEndScope", t() == "}");
		t.token("ArgumentSeparator", t() == ",");
		t.token("OperatorAssign", t() == "=");
		t.token("OperatorAccess", t() == ".");
		t.token("OperatorAdd", t() == "+");
		t.token("OperatorSubtract", t() == "-");
		t.token("OperatorMultiply", t() == "*");
		t.token("OperatorDivide", t() == "/");
		t.token("OperatorModulate", t() == "%");
		t.token("OperatorAndBitwise", t() == "&");
		t.token("OperatorAndLogical", t() == "&&");
		t.token("OperatorOrBitwise", t() == "|");
		t.token("OperatorOrLogical", t() == "||");
		t.token("OperatorXOrBitwise", t() == "^");
		t.token("OperatorNegateBitwise", t() == "~");
		t.token("OperatorNegateLogical", t() == "!");
		t.token("OperatorBitShiftLeft", t() == "<<");
		t.token("OperatorBitShiftRight", t() == ">>");
		t.token("OperatorLessThan", t() == "<");
		t.token("OperatorLessThanOrEqualTo", t() == "<=");
		t.token("OperatorGreaterThan", t() == ">");
		t.token("OperatorGreaterThanOrEqualTo", t() == ">=");
		t.token("OperatorEqualTo", t() == "==");
		t.token("OperatorNotEqualTo", t() == "!=");
		t.token("OperatorAddAssign", t() == "+=");
		t.token("OperatorIncrement", t() == "++");
		t.token("OperatorSubtractAssign", t() == "-=");
		t.token("OperatorDecrement", t() == "--");
		t.token("OperatorMultiplyAssign", t() == "*=");
		t.token("OperatorDivideAssign", t() == "/=");
		t.token("OperatorModulateAssign", t() == "%=");
		t.token("OperatorAndBitwiseAssign", t() == "&=");
		t.token("OperatorOrBitwiseAssign", t() == "|=");
		t.token("OperatorXOrBitwiseAssign", t() == "^=");
		t.token("OperatorNegateBitwiseAssign", t() == "~=");
		t.token("OperatorBitShiftLeftAssign", t() == "<<=");
		t.token("OperatorBitShiftRightAssign", t() == ">>=");
		t.token("TypeExpressionPrefix", t() == "@");
		t.token("EndOfLine", t() == ";");
		t.token("ValueBool", t() == "true" || t() == "True" || t() == "TRUE" || t() == "false" || t() == "False" || t() == "FALSE");
		t.token("ValueChar", t() < "'" && t() > "'");
		t.token("ValueString", t() < "\"" && t() > "\"");
		t.token("ValueInt", t() += "0123456789-");
		t.token("ValueFloat", t() += "0123456789-.");
		t.token("Symbol", t() += lettersLower+lettersUpper+digits+"_");
		t.combine(t() == "-", t() == ".");
		t.combine(t() == "-.", t() += "-.0123456789");
		t.combine(t() == "-", t() += "-.0123456789");
		t.combine(t() == ".", t() += "-.0123456789");
		t.combine(t() += "-.0123456789", t() += "-.0123456789");
		t.combine(t() == "x", t() < "\"" && t() > "\"");

		return t;
	}

	void Parser::createParser()
	{
		odb = OneDB();

		symbolOrValueInt = Parallel("SymbolOrValueInt");
		symbolOrValueInt.append(odb.keep("Symbol"));
		symbolOrValueInt.append(odb.keep("ValueInt"));

		internalAssemblyArgs = Many("InternalAssemblyArgs", symbolOrValueInt);

		oneInternalAssemblyLine = Sequence("OneInternalAssemblyLine");
		oneInternalAssemblyLine.append(odb["Symbol"]);
		oneInternalAssemblyLine.append(internalAssemblyArgs);
		oneInternalAssemblyLine.append(odb.one("EndOfLine"));

		internalAssemblyLines = Many("InternalAssemblyLines", oneInternalAssemblyLine);

		internalAssemblyExpression = Sequence("InternalAssemblyExpression");
		internalAssemblyExpression.append(odb.one("InternalAssemblySymbol"));
		internalAssemblyExpression.append(odb.one("BoundaryBeginScope"));
		internalAssemblyExpression.append(internalAssemblyLines);
		internalAssemblyExpression.append(odb.one("BoundaryEndScope"));

		typeQualifiers = Many("TypeQualifiers", odb["Symbol"]);

		typeExpression = Parallel("TypeExpression");
		typeExpression.assumeSize(1);

		typeExpressionArguments = Sequence("TypeExpressionArguments");
		typeExpressionArguments.assumeSize(5);
		typeExpressionArguments.append(odb.one("TypeExpressionPrefix"));
		typeExpressionArguments.append(typeQualifiers);
		typeExpressionArguments.append(odb.one("BoundaryBeginExpression"));
		typeExpressionArguments.append(typeExpression);
		typeExpressionArguments.append(odb.one("BoundaryEndExpression"));

		typeExpressionNoArguments = Sequence("TypeExpressionNoArguments");
		typeExpressionNoArguments.assumeSize(1);
		typeExpressionNoArguments.append(typeQualifiers);

		typeExpression.append(typeExpressionArguments);
		typeExpression.append(typeExpressionNoArguments);

		any = Parallel("Any");
		any.assumeSize(0);

		oneComplexValueElement = Sequence("ComplexValueElements");
		oneComplexValueElement.append(any);
		oneComplexValueElement.append(odb.one("ArgumentSeparator"));

		complexValueElements = Many("ComplexValueElements", oneComplexValueElement);

		complexValue = Sequence("ComplexValue");
		complexValue.append(odb.one("BoundaryBeginComplex"));
		complexValue.append(complexValueElements);
		complexValue.append(oneComplexValueElement);
		complexValue.append(odb.one("BoundaryEndComplex"));

		operatorUnaryLeft = Parallel("OperatorUnaryLeft");
		operatorUnaryLeft.append(odb.keep("OperatorNegateBitwise"));
		operatorUnaryLeft.append(odb.keep("OperatorNegateLogical"));

		operatorUnaryRight = Parallel("OperatorUnaryRight");
		operatorUnaryRight.append(odb.keep("OperatorIncrement"));
		operatorUnaryRight.append(odb.keep("OperatorDecrement"));

		operatorBinary = Parallel("OperatorBinary");
		operatorBinary.append(odb.keep("OperatorAssign"));
		operatorBinary.append(odb.keep("OperatorAccess"));
		operatorBinary.append(odb.keep("OperatorAdd"));
		operatorBinary.append(odb.keep("OperatorSubtract"));
		operatorBinary.append(odb.keep("OperatorMultiply"));
		operatorBinary.append(odb.keep("OperatorDivide"));
		operatorBinary.append(odb.keep("OperatorModulate"));
		operatorBinary.append(odb.keep("OperatorAndBitwise"));
		operatorBinary.append(odb.keep("OperatorAndLogical"));
		operatorBinary.append(odb.keep("OperatorOrBitwise"));
		operatorBinary.append(odb.keep("OperatorOrLogical"));
		operatorBinary.append(odb.keep("OperatorXOrBitwise"));
		operatorBinary.append(odb.keep("OperatorBitShiftLeft"));
		operatorBinary.append(odb.keep("OperatorBitShiftRight"));
		operatorBinary.append(odb.keep("OperatorLessThan"));
		operatorBinary.append(odb.keep("OperatorLessThanOrEqualTo"));
		operatorBinary.append(odb.keep("OperatorGreaterThan"));
		operatorBinary.append(odb.keep("OperatorGreaterThanOrEqualTo"));
		operatorBinary.append(odb.keep("OperatorEqualTo"));
		operatorBinary.append(odb.keep("OperatorNotEqualTo"));
		operatorBinary.append(odb.keep("OperatorAddAssign"));
		operatorBinary.append(odb.keep("OperatorSubtractAssign"));
		operatorBinary.append(odb.keep("OperatorMultiplyAssign"));
		operatorBinary.append(odb.keep("OperatorDivideAssign"));
		operatorBinary.append(odb.keep("OperatorModulateAssign"));
		operatorBinary.append(odb.keep("OperatorAndBitwiseAssign"));
		operatorBinary.append(odb.keep("OperatorOrBitwiseAssign"));
		operatorBinary.append(odb.keep("OperatorXOrBitwiseAssign"));
		operatorBinary.append(odb.keep("OperatorNegateAssign"));
		operatorBinary.append(odb.keep("OperatorBitShiftLeftAssign"));
		operatorBinary.append(odb.keep("OperatorBitShiftRightAssign"));

		operationUnaryLeft = Sequence("OperationUnaryLeft");
		operationUnaryLeft.append(operatorUnaryLeft);
		operationUnaryLeft.append(any);

		operationUnaryRight = Sequence("OperationUnaryRight");
		operationUnaryRight.append(any);
		operationUnaryRight.append(operatorUnaryRight);

		operationBinary = Sequence("OperationBinary");
		operationBinary.append(any);
		operationBinary.append(operatorBinary);
		operationBinary.append(any);

		anyUnboundedOperation = Parallel("AnyUnboundedOperation");
		anyUnboundedOperation.append(operationUnaryLeft);
		anyUnboundedOperation.append(operationUnaryRight);
		anyUnboundedOperation.append(operationBinary);

		operation = Parallel("Operation");
		operation.assumeSize(2);
		operation.append(anyUnboundedOperation);

		anyBoundedOperation = Sequence("AnyBoundedOperation");
		anyBoundedOperation.append(odb["BoundaryBeginExpression"]);
		anyBoundedOperation.append(operation);
		anyBoundedOperation.append(odb["BoundaryEndExpression"]);

		operation.append(anyBoundedOperation);

		oneArgumentVectorElement = Sequence("OneArgumentVectorElement");
		oneArgumentVectorElement.append(any);
		oneArgumentVectorElement.append(odb["ArgumentSeparator"]);

		argumentVectorElements = Many("ArgumentVectorElements", oneArgumentVectorElement);

		argumentVectorNonNull = Sequence("ArgumentVectorNonNull");
		argumentVectorNonNull.append(argumentVectorElements);
		argumentVectorNonNull.append(any);

		argumentVectorNull = Sequence("ArgumentVectorNull");

		argumentVectorContents = Parallel("ArgumentVectorContents");
		argumentVectorContents.append(argumentVectorNonNull);
		argumentVectorContents.append(argumentVectorNull);

		argumentVector = Sequence("ArgumentVector");
		argumentVector.append(odb["BoundaryBeginScope"]);
		argumentVector.append(argumentVectorContents);
		argumentVector.append(odb["BoundaryBeginScope"]);

		oneLine = Sequence("OneLine");
		oneLine.append(any);
		oneLine.append(odb["EndOfLine"]);

		lines = Many("Lines", oneLine);

		body = Sequence("Body");
		body.append(odb.one("BoundaryBeginScope"));
		body.append(lines);
		body.append(odb.one("BoundaryEndScope"));

		where = Sequence("Where");
		where.append(odb.keep("Where"));
		where.append(body);

		expressionSymbolComplex = Sequence("ExpessionSymbolComplex");
		expressionSymbolComplex.append(odb["Symbol"]);
		expressionSymbolComplex.append(complexValue);

		expressionSymbolArgument = Sequence("ExpessionSymbolArgument");
		expressionSymbolArgument.append(odb["Symbol"]);
		expressionSymbolArgument.append(argumentVector);

		expressionSymbolBody = Sequence("ExpessionSymbolBody");
		expressionSymbolBody.append(odb["Symbol"]);
		expressionSymbolBody.append(body);

		expressionSymbolBodyWhere = Sequence("ExpessionSymbolBodyWhere");
		expressionSymbolBodyWhere.append(odb["Symbol"]);
		expressionSymbolBodyWhere.append(body);
		expressionSymbolBodyWhere.append(where);

		expressionSymbolComplexArgument = Sequence("ExpessionSymbolComplexArgument");
		expressionSymbolComplexArgument.append(odb["Symbol"]);
		expressionSymbolComplexArgument.append(complexValue);
		expressionSymbolComplexArgument.append(argumentVector);

		expressionSymbolArgumentBody = Sequence("ExpessionSymbolArgumentBody");
		expressionSymbolArgumentBody.append(odb["Symbol"]);
		expressionSymbolArgumentBody.append(argumentVector);
		expressionSymbolArgumentBody.append(body);

		expressionSymbolArgumentBodyWhere = Sequence("ExpessionSymbolArgumentBodyWhere");
		expressionSymbolArgumentBodyWhere.append(odb["Symbol"]);
		expressionSymbolArgumentBodyWhere.append(argumentVector);
		expressionSymbolArgumentBodyWhere.append(body);
		expressionSymbolArgumentBodyWhere.append(where);

		expressionSymbolComplexArgumentBody = Sequence("ExpessionSymbolComplexArgumentBody");
		expressionSymbolComplexArgumentBody.append(odb["Symbol"]);
		expressionSymbolComplexArgumentBody.append(complexValue);
		expressionSymbolComplexArgumentBody.append(argumentVector);
		expressionSymbolComplexArgumentBody.append(body);

		expressionSymbolComplexArgumentBodyWhere = Sequence("ExpessionSymbolComplexArgumentBodyWhere");
		expressionSymbolComplexArgumentBodyWhere.append(odb["Symbol"]);
		expressionSymbolComplexArgumentBodyWhere.append(complexValue);
		expressionSymbolComplexArgumentBodyWhere.append(argumentVector);
		expressionSymbolComplexArgumentBodyWhere.append(body);
		expressionSymbolComplexArgumentBodyWhere.append(where);

		expressionTypeSymbolComplex = Sequence("ExpessionTypeSymbolComplex");
		expressionTypeSymbolComplex.append(typeExpression);
		expressionTypeSymbolComplex.append(odb["Symbol"]);
		expressionTypeSymbolComplex.append(complexValue);

		expressionTypeSymbolArgument = Sequence("ExpessionTypeSymbolArgument");
		expressionTypeSymbolArgument.append(typeExpression);
		expressionTypeSymbolArgument.append(odb["Symbol"]);
		expressionTypeSymbolArgument.append(argumentVector);

		expressionTypeSymbolBody = Sequence("ExpessionTypeSymbolBody");
		expressionTypeSymbolBody.append(typeExpression);
		expressionTypeSymbolBody.append(odb["Symbol"]);
		expressionTypeSymbolBody.append(body);

		expressionTypeSymbolBodyWhere = Sequence("ExpessionTypeSymbolBodyWhere");
		expressionTypeSymbolBodyWhere.append(typeExpression);
		expressionTypeSymbolBodyWhere.append(odb["Symbol"]);
		expressionTypeSymbolBodyWhere.append(body);
		expressionTypeSymbolBodyWhere.append(where);

		expressionTypeSymbolComplexArgument = Sequence("ExpessionTypeSymbolComplexArgument");
		expressionTypeSymbolComplexArgument.append(typeExpression);
		expressionTypeSymbolComplexArgument.append(odb["Symbol"]);
		expressionTypeSymbolComplexArgument.append(complexValue);
		expressionTypeSymbolComplexArgument.append(argumentVector);

		expressionTypeSymbolArgumentBody = Sequence("ExpessionTypeSymbolArgumentBody");
		expressionTypeSymbolArgumentBody.append(typeExpression);
		expressionTypeSymbolArgumentBody.append(odb["Symbol"]);
		expressionTypeSymbolArgumentBody.append(argumentVector);
		expressionTypeSymbolArgumentBody.append(body);

		expressionTypeSymbolArgumentBodyWhere = Sequence("ExpessionTypeSymbolArgumentBodyWhere");
		expressionTypeSymbolArgumentBodyWhere.append(typeExpression);
		expressionTypeSymbolArgumentBodyWhere.append(odb["Symbol"]);
		expressionTypeSymbolArgumentBodyWhere.append(argumentVector);
		expressionTypeSymbolArgumentBodyWhere.append(body);
		expressionTypeSymbolArgumentBodyWhere.append(where);

		expressionTypeSymbolComplexArgumentBody = Sequence("ExpessionTypeSymbolComplexArgumentBody");
		expressionTypeSymbolComplexArgumentBody.append(typeExpression);
		expressionTypeSymbolComplexArgumentBody.append(odb["Symbol"]);
		expressionTypeSymbolComplexArgumentBody.append(complexValue);
		expressionTypeSymbolComplexArgumentBody.append(argumentVector);
		expressionTypeSymbolComplexArgumentBody.append(body);

		expressionTypeSymbolComplexArgumentBodyWhere = Sequence("ExpessionTypeSymbolComplexArgumentBodyWhere");
		expressionTypeSymbolComplexArgumentBodyWhere.append(typeExpression);
		expressionTypeSymbolComplexArgumentBodyWhere.append(odb["Symbol"]);
		expressionTypeSymbolComplexArgumentBodyWhere.append(complexValue);
		expressionTypeSymbolComplexArgumentBodyWhere.append(argumentVector);
		expressionTypeSymbolComplexArgumentBodyWhere.append(body);
		expressionTypeSymbolComplexArgumentBodyWhere.append(where);

		expression = Parallel("Expression");
		expression.append(expressionSymbolComplex);
		expression.append(expressionSymbolArgument);
		expression.append(expressionSymbolBody);
		expression.append(expressionSymbolBodyWhere);
		expression.append(expressionSymbolComplexArgument);
		expression.append(expressionSymbolArgumentBody);
		expression.append(expressionSymbolArgumentBodyWhere);
		expression.append(expressionSymbolComplexArgumentBody);
		expression.append(expressionSymbolComplexArgumentBodyWhere);
		expression.append(expressionTypeSymbolComplex);
		expression.append(expressionTypeSymbolArgument);
		expression.append(expressionTypeSymbolBody);
		expression.append(expressionTypeSymbolBodyWhere);
		expression.append(expressionTypeSymbolComplexArgument);
		expression.append(expressionTypeSymbolArgumentBody);
		expression.append(expressionTypeSymbolArgumentBodyWhere);
		expression.append(expressionTypeSymbolComplexArgumentBody);
		expression.append(expressionTypeSymbolComplexArgumentBodyWhere);

		any.append(internalAssemblyExpression);
		any.append(typeExpression);
		any.append(complexValue);
		any.append(operation);
		any.append(expression);

		page = Many("Page", any);
		page.debug();
	}

	AST Parser::parse(vector<Token> toks)
	{
		unsigned int off = 0;
		vector<Expectation *> stack;
		return page.parse(toks, off, ebuf, stack);
	}

	vector<Error> Parser::getErrors()
	{
		return ebuf;
	}

	void Parser::clearErrors()
	{
		ebuf.clear();
	}
}
