#ifndef _IDENTIFIER_H_INCLUDED_
#define _IDENTIFIER_H_INCLUDED_

#include "Expression.h"

class Identifier: public Expression {
public:
	Identifier(string symbol);

	void display();
	void checkSemantic();
	void generateCode(fstream &output);
};

#endif
