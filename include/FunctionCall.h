#ifndef _FUNCTION_CALL_H_INCLUDED_
#define _FUNCTION_CALL_H_INCLUDED_

#include "Identifier.h"

class FunctionCall: public Expression {
public:
	Identifier *id;
	Expression *args;

	FunctionCall(Identifier *id, Expression *args);
	~FunctionCall();

	void display();
	void checkSemantic();
	void checkFunctionArgs(SymTabRecord *function_def, Expression *args);
};

#endif