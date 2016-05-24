#include "../include/Declarator.h"

Declarator::Declarator(Identifier *id) 
        : Node("") {
    this->id = id;
}

Declarator::~Declarator() {
    delete(this->id);
}

void Declarator::checkSemantic(int type) {
    this->type = TYPE_VOID;
}