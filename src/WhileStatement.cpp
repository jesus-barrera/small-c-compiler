#include "../include/WhileStatement.h"

WhileStatement::WhileStatement(Expression *expr, Node *statement) 
        : Node ("") {
    this->expr = expr;
    this->statement = statement;
}

WhileStatement::~WhileStatement() {
    delete(this->expr);
    delete(this->statement);
}

void WhileStatement::display() {
    xml.openTag("while");
    this->expr->display();
    displayList("block", this->statement);
    xml.closeTag();
}

void WhileStatement::checkSemantic() {
    this->expr->checkSemantic();

    checkSemanticOnList(this->statement);

    if (this->expr->type != TYPE_INT) {
        error("Expresion no booleana en while");
    } else {
        this->type = TYPE_VOID;
    }
}