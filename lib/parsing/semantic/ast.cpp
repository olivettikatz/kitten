#include "ast.h"

namespace parsing
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

	string AST::display()
	{
		stringstream ss;
		ss << "(" << eid << ":" << content.get();
		for (vector<AST>::iterator i = children.begin(); i != children.end(); i++)
			ss << " " << i->display();
		ss << ")";
		return ss.str();
	}
}

