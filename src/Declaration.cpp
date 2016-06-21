#include "../include/Declaration.h"

Declaration::Declaration(DataType *data_type, Declarator *declarator) :
        Node("") {

    this->data_type = data_type;
    this->declarator = declarator;
}

Declaration::~Declaration() {
    delete(this->data_type);
    delete(this->declarator);
}

void Declaration::display() {
    xml.openTag("declaration");
    this->data_type->display();
    displayList("declarator-list", this->declarator);
    xml.closeTag();
}

void Declaration::checkSemantic() {
    Declarator *node;
    int data_type;

    data_type = this->data_type->getType();

    node = this->declarator;

    while (node) {
        node->checkSemantic(data_type);

        node = (Declarator *)node->next;
    }
}

void Declaration::generateCode(fstream &output) {
    Node::generateCodeOnList(this->declarator, output);
}