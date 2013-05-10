#include "ast.h"

namespace taurus
{
	bool AST::good()
	{
		return (eid.empty() == false);
	}

	bool AST::empty()
	{
		return content.empty();
	}

	AST &AST::add(AST a)
	{
		children.push_back(a);
		return *this;
	}

	string AST::getExpectationID()
	{
		return eid;
	}

	Token AST::getContent()
	{
		return content;
	}

	unsigned int AST::getChildrenSize()
	{
		return children.size();
	}

	AST AST::getChild(unsigned int idx)
	{
		return children[idx];
	}
}

