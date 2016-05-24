#ifndef _FUNCTION_DEFINITION_H_INCLUDED_
#define _FUNCTION_DEFINITION_H_INCLUDED_

#include "DataType.h"
#include "Identifier.h"
#include "Parameter.h"

class FunctionDefinition: public Node {
public:
	DataType *data_type;
	Identifier *id;
	Parameter *params;
	Node *statement;


	FunctionDefinition(DataType *data_type, Identifier *Identifier, Parameter *params, Node *statement);
	~FunctionDefinition();

	void display();
	void checkSemantic();
};

#endif