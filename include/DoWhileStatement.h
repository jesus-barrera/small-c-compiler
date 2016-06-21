#ifndef _DO_WHILE_STATEMENT_H_INCLUDED_
#define _DO_WHILE_STATEMENT_H_INCLUDED_

#include "WhileStatement.h"

class DoWhileStatement: public WhileStatement {
public:
	DoWhileStatement(Node *statement, Expression *expr);

	void display();
	void generateCode(fstream &output);
};

#endif