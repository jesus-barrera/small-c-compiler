#include "../include/Identifier.h"

Identifier::Identifier(string symbol) {
    this->symbol = symbol;
}

void Identifier::display() {
    xml.oneLineTag("id", this->symbol);
}

void Identifier::checkSemantic() {
    SymTabRecord *record = symtable.get(this->symbol);

    if (record) {
        if (record->sym_type == SYM_VARIABLE) {
            this->type = record->data_type;
        } else {
            error("\"" + this->symbol + "\" es una funcion");
        }
    } else {
        error("Variable no definida: \"" +  this->symbol + "\"");
    }
}

void Identifier::generateCode(fstream &output) {
    
}