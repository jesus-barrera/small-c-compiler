#ifndef _BINARY_EXPRESSION_H_INCLUDED_
#define _BINARY_EXPRESSION_H_INCLUDED_

#include "Expression.h"

class BinaryExpression: public Expression {
public:
	string op;
	Expression *left;
	Expression *right;

	BinaryExpression(string op, Expression *left, Expression* right);
	~BinaryExpression();
	
	void display();
	void checkSemantic();
};

#endif