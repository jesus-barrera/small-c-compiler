#ifndef _INTEGER_H_INCLUDED_
#define _INTEGER_H_INCLUDED_

#include "Expression.h"

class Integer: public Expression {
public:
	Integer(string symbol);

	void display();
	void checkSemantic();
	void generateCode(fstream &output);
};

#endif