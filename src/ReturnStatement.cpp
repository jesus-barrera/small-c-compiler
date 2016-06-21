#include "../include/ReturnStatement.h"

ReturnStatement::ReturnStatement(Expression *expr) 
        : Node ("") {
    this->expr = expr;
}

ReturnStatement::~ReturnStatement() {
    delete(this->expr);
}

void ReturnStatement::display() {
    xml.openTag("return");
    if (this->expr) this->expr->display();
    xml.closeTag();
}

void ReturnStatement::checkSemantic() {
    SymTabRecord *context;

    context = symtable.getContext();

    if (this->expr) {
        this->expr->checkSemantic();
        
        if (context->data_type != expr->type) {
            error("En funcion " + context->symbol + ": return y tipo de la funcion no coincide");
        }
    }

    this->type = TYPE_VOID;
}

void ReturnStatement::generateCode(fstream &output) {
    SymTabRecord *context = symtable.getContext();

    if (this->expr) this->expr->generateCode(output);

    output << "jmp FIN_" << context->symbol << endl;
}