#ifndef _FUNCTION_DECLARATOR_H_INCLUDED_
#define _FUNCTION_DECLARATOR_H_INCLUDED_

#include "Declarator.h"
#include "Parameter.h"

class FunctionDeclarator: public Declarator {
public:
	Parameter *params;

	FunctionDeclarator(Identifier *id, Parameter *params);
	~FunctionDeclarator();

	void display();
	void checkSemantic(int type);
	static void checkFunctionDeclaration(int data_type, Parameter *params, SymTabRecord *function_def);
};

#endif