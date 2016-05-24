#ifndef _DECLARATOR_H_INCLUDED_
#define _DECLARATOR_H_INCLUDED_

#include "Identifier.h"

class Declarator: public Node {
public:
	Identifier *id;
	Declarator(Identifier *id);
	~Declarator();

	virtual void checkSemantic(int type);
};

#endif