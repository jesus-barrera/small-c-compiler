#ifndef _WHILE_STATEMENT_H_INCLUDED_
#define _WHILE_STATEMENT_H_INCLUDED_

#include "Expression.h"

class WhileStatement: public Node {
public:
	Expression *expr;
	Node *statement;

	WhileStatement(Expression *expr, Node *statement);
	~WhileStatement();

	void display();
	void checkSemantic();
	void generateCode(fstream &output);
};

#endif