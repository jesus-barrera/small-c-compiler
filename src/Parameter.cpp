#include "../include/Parameter.h"

Parameter::Parameter(DataType *data_type, Identifier *id) 
        : Node("") {
    this->data_type = data_type;
    this->id = id;
}

Parameter::~Parameter() {
    delete(data_type);
    delete(id);
}

void Parameter::display() {
    xml.openTag("parameter");
    this->data_type->display();
    this->id->display();
    xml.closeTag();
}

void Parameter::checkSemantic() {
    int type = this->data_type->getType();

    if (type != TYPE_INT) {
        error("Parametro " + this->id->symbol + " de tipo " + str_data_types[type]);
    }

    this->type = TYPE_VOID;
}