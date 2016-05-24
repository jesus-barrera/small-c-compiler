#ifndef _DATA_TYPE_H_INCLUDED_
#define _DATA_TYPE_H_INCLUDED_

#include "Node.h"

class DataType: public Node {
public:
	DataType(string symbol);
	int getType();

	void display();
};

#endif
