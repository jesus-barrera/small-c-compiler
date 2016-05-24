#ifndef _FOR_STATEMENT_H_INCLUDED_
#define _FOR_STATEMENT_H_INCLUDED_

#include "Expression.h"

class ForStatement: public Node {
public:
	Expression *initializer;
	Expression *condition;
	Expression *step;
	Node *statement;

	ForStatement(Expression *initializer, Expression *condition, Expression *step, Node *statement);
	~ForStatement();

	void display();
	void checkSemantic();
};

#endif