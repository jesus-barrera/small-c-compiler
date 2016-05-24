#ifndef _DECLARATION_H_INCLUDED_
#define _DECLARATION_H_INCLUDED_

#include "DataType.h"
#include "Declarator.h"

class Declaration: public Node {
public:
	DataType *data_type;
	Declarator *declarator;

	Declaration(DataType *data_type, Declarator *declarator);
	~Declaration();

	void display();
	void checkSemantic();
};

#endif