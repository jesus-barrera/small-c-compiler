#include "../include/DataType.h"

// DataType methods
DataType::DataType(string symbol) 
        : Node(symbol) {}

void DataType::display() {
    xml.oneLineTag("type", this->symbol);
}

int DataType::getType() {
    if (symbol == "int") {
        return TYPE_INT;
    } else if (symbol == "void") {
        return TYPE_VOID;
    } else {
        return TYPE_ERROR;
    }
}