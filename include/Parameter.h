#ifndef _PARAMETER_H_INCLUDED_
#define _PARAMETER_H_INCLUDED_

#include "DataType.h"
#include "Identifier.h"

class Parameter: public Node {
public:
	DataType *data_type;
	Identifier *id;

	Parameter(DataType *data_type, Identifier *id);
	~Parameter();

	void display();
	void checkSemantic();
};

#endif