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

	Parser createParser(Tokenizer &t)
	{
		Parser p = Parser();
		p.add("InternalAssemblyLine0", p("Symbol").keep());
		p.add("InternalAssemblyLine1", p("Symbol").keep() << (p["Symbol"].keep() || p["ValueInt"].keep()));
		p.add("InternalAssemblyLine2", p["Symbol"].keep() << p["Symbol"].keep() << (p["Symbol"].keep() || p["ValueInt"].keep()));
		p.add("InternalAssemblyLine3", p["Symbol"].keep() << p["Symbol"].keep() << p["Symbol"].keep() << (p["Symbol"].keep() || p["ValueInt"].keep()));
		p.add("InternalAssemblyLines", (p("InternalAssemblyLine0") || p("InternalAssemblyLine1") || p("InternalAssemblyLine2") || p("InternalAssemblyLine3")) << p["EndOfLine"]);
		p.many("InternalAssemblyLines");

		p.add("InternalAssemblyExpression", p["InternalAssemblySymbol"] << p["BoundaryBeginScope"] << p["internalAssemblyLines"] << p["BoundaryEndScope"]);

		p.add("TypeQualifiers", p["Symbol"]);
		p.many("TypeQualifiers");
		p.add("TypeExpression", p("TypeQualifiers") << p["BoundaryBeginExpression"] << p("TypeExpression") << p["BoundaryEndExpression"]);

		p.add("Any", p());

		p.add("ComplexValueElements", p("Any") << p("ArgumentSeparator"));
		p.many("ComplexValueElements");
		p.add("ComplexValue", p("BoundaryBeginComplex") << p("ComplexValueElements") << p("BoundaryEndComplex"));

		p.add("OperatorUnaryLeft", p("OperatorNegateBitwise") || p("OperatorNegateLogical"));
		p.add("OperatorUnaryRight", p("OperatorIncrement") || p("OperatorDecrement"));
		p.add("OperatorBinary", p("OperatorAssign") || p("OperatorAccess") || p("OperatorAdd") || p("OperatorSubtract") || p("OperatorMultiply") || p("OperatorDivide") || p("OperatorModulate") || p("OperatorAndBitwise") || p("OperatorAndLogical") || p("OperatorOrBitwise") || p("OperatorOrLogical") || p("OperatorXOrBitwise") || p("OperatorBitShiftLeft") || p("OperatorBitShiftRight") || p("OperatorLessThan") || p("OperatorLessThanOrEqualTo") || p("OperatorGreaterThen") || p("OperatorGreaterThanOrEqualTo") || p("OperatorEqualTo") || p("OperatorNotEqualTo") || p("OperatorAddAssign") || p("OperatorSubtractAssign") || p("OperatorMultiplyAssign") || p("OperatorDivideAssign") || p("OperatorModulateAssign") || p("OperatorAndBitwiseAssign") || p("OperatorOrBitwiseAssign") || p("OperatorXOrBitwiseAssign") || p("OperatorNegateAssign") || p("OperatorBitShiftLeftAssign") || p("OperatorBitShiftRightAssign"));

		p.add("OperationUnaryLeft", p("OperatorUnaryLeft") << p("Any"));
		p.add("OperationUnaryRight", p("Any") << p("OperatorUnaryLeft"));
		p.add("OperationBinary", p("Any") << p("OperatorBinary") << p("Any"));
		p.add("Operation", (p("OperationUnaryLeft") || p("OperationUnaryRight") || p("OperationBinary")) || (p("BoundaryBeginExpression") << p("Operation") << p("BoundaryEndExpression")));

		p.add("ArgumentVectorElements", p("Any") << p("ArgumentSeparator"));
		p.many("ArgumentVectorElements");
		p.add("ArgumentVector", p("ArgumentVectorElements"));
	
		p.add("Lines", p("Any") << p("EndOfLine"));
		p.many("Lines");
		p.add("Body", p("BoundaryBeginScope") << p("Lines") << p("BoundaryEndScope"));

		p.add("ExpressionSymbolComplex", p("Symbol") << p("ComplexValue"));
		p.add("ExpressionSymbolArgument", p("Symbol") << p("ArgumentVector"));
		p.add("ExpressionSymbolBody", p("Symbol") << p("Body"));
		p.add("ExpressionSymbolBodyWhere", p("Symbol") << p("Body") << p["Where"] << p("Body"));
		p.add("ExpressionSymbolComplexArgument", p("Symbol") << p("ComplexValue") << p("ArgumentVector"));
		p.add("ExpressionSymbolArgumentBody", p("Symbol") << p("ArgumentVector") << p("Body"));
		p.add("ExpressionSymbolArgumentBodyWhere", p("Symbol") << p("ArgumentVector") << p("Body") << p["Where"] << p("Body"));
		p.add("ExpressionSymbolComplexArgumentBody", p("Symbol") << p("ComplexValue") << p("ArgumentVector") << p("Body"));
		p.add("ExpressionSymbolComplexArgumentBodyWhere", p("Symbol") << p("ComplexValue") << p("ArgumentVector") << p("Body") << p["Where"] << p("Body"));
		p.add("ExpressionTypeSymbolComplex", p("TypeExpression") << p("Symbol") << p("ComplexValue"));
		p.add("ExpressionTypeSymbolArgument", p("TypeExpression") << p("Symbol") << p("ArgumentVector"));
		p.add("ExpressionTypeSymbolBody", p("TypeExpression") << p("Symbol") << p("Body"));
		p.add("ExpressionTypeSymbolBodyWhere", p("TypeExpression") << p("Symbol") << p("Body") << p["Where"] << p("Body"));
		p.add("ExpressionTypeSymbolComplexArgument", p("TypeExpression") << p("Symbol") << p("ComplexValue") << p("ArgumentVector"));
		p.add("ExpressionTypeSymbolArgumentBody", p("TypeExpression") << p("Symbol") << p("ArgumentVector") << p("Body"));
		p.add("ExpressionTypeSymbolArgumentBodyWhere", p("TypeExpression") << p("Symbol") << p("ArgumentVector") << p("Body") << p["Where"] << p("Body"));
		p.add("ExpressionTypeSymbolComplexArgumentBody", p("TypeExpression") << p("Symbol") << p("ComplexValue") << p("ArgumentVector") << p("Body"));
		p.add("ExpressionTypeSymbolComplexArgumentBodyWhere", p("TypeExpression") << p("Symbol") << p("ComplexValue") << p("ArgumentVector") << p("Body") << p["Where"] << p("Body"));
		p.add("Expression", p("ExpressionSymbolComplex") || p("ExpressionSymbolArgument") || p("ExpressionSymbolBody") || p("ExpressionSymbolBodyWhere") || p("ExpressionSymbolComplexArgument") || p("ExpressionSymbolArgumentBody") || p("ExpressionSymbolArgumentBodyWhere") || p("ExpressionSymbolComplexArgumentBody") || p("ExpressionSymbolComplexArgumentBodyWhere") || p("ExpressionTypeSymbolComplex") || p("ExpressionTypeSymbolArgument") || p("ExpressionTypeSymbolBody") || p("ExpressionTypeSymbolBody") || p("ExpressionTypeSymbolComplexArgument") || p("ExpressionTypeSymbolArgumentBody") || p("ExpressionTypeSymbolArgumentBody") || p("ExpressionTypeSymbolComplexArgumentBody") || p("ExpressionTypeSymbolComplexArgumentBody"));

		p.add("Any", p("InternalAssemblyExpression") || p("TypeExpression") || p("ComplexValue") || p("Operation") || p("Expression"));

		p.add("*", p("Expression"));
		p.many("*");

		return p;
	}
}
