#ifndef _RETURN_STATEMENT_H_INCLUDED_
#define _RETURN_STATEMENT_H_INCLUDED_

#include "Expression.h"

class ReturnStatement: public Node {
public:
	Expression *expr;

	ReturnStatement(Expression *expr);
	~ReturnStatement();

	void display();
	void checkSemantic();
};

#endif