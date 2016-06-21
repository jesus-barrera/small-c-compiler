#include "../include/Integer.h"

Integer::Integer(string symbol) {
    this->symbol = symbol;
}

void Integer::display() {
    xml.oneLineTag("integer", this->symbol);
}

void Integer::checkSemantic() {
    this->type = TYPE_INT;
}

void Integer::generateCode(fstream &output) {
	output << "movl $" << this->symbol << ", %eax" << endl;
}