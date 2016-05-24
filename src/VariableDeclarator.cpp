#include "../include/VariableDeclarator.h"

VariableDeclarator::VariableDeclarator(Identifier *id, Expression *init)
        : Declarator(id) {
    this->init = init;
}

VariableDeclarator::~VariableDeclarator() {
    delete(this->init);
}

void VariableDeclarator::display() {
    xml.openTag("variable-declarator");
    this->id->display();
    if (this->init) this->init->display();
    xml.closeTag();
}

void VariableDeclarator::checkSemantic(int type) {
    // Check if the variable has been declared
    if (symtable.get(this->id->symbol, false) != NULL) {
        error("El simbolo \"" + this->id->symbol + "\" ya ha sido declarado");
    }

    // Check the declaration type is correct
    if (type != TYPE_INT) {
        error("Variable \"" + this->id->symbol + "\" de tipo " + str_data_types[type]);
    }

    // Check initializer
    if (this->init) {
        this->init->checkSemantic();

        if (this->init->type != type) {
            error("Inicializador de \"" + this->id->symbol + "\" tipo " + str_data_types[type]);
        }
    }

    symtable.insert(this->id->symbol, SYM_VARIABLE, type, NULL);
    this->type = TYPE_VOID;
}