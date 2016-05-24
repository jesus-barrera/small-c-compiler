#ifndef _UNARY_EXPRESSION_H_INCLUDED_
#define _UNARY_EXPRESSION_H_INCLUDED_

#include "Expression.h"

class UnaryExpression: public Expression {
public:
	string op;
	Expression *expr;

	UnaryExpression(string op, Expression *expr);
	~UnaryExpression();

	void display();
	void checkSemantic();
};

#endif