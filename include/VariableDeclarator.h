#ifndef _VARIABLE_DECLARATOR_H_INCLUDED_
#define _VARIABLE_DECLARATOR_H_INCLUDED_

#include "Declarator.h"
#include "Expression.h"

class VariableDeclarator: public Declarator {
public:
	Expression *init;

	VariableDeclarator(Identifier *id, Expression *initializer);
	~VariableDeclarator();

	void display();
	void checkSemantic(int type);
};

#endif