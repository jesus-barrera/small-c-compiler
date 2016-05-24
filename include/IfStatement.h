#ifndef _IF_STATEMENT_INCLUDED_
#define _IF_STATEMENT_INCLUDED_

#include "Expression.h"

class IfStatement: public Node {
public:
	Expression *expr;
	Node *statement;
	Node *elseStatement;

	IfStatement(Expression *expr, Node *statement, Node *elseStatement);
	~IfStatement();

	void display();
	void checkSemantic();
};

#endif