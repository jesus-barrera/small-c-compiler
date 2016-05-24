#include "../include/IfStatement.h"

IfStatement::IfStatement(Expression *expr, Node *statement, Node *elseStatement) 
        : Node ("") {
    this->expr = expr;
    this->statement = statement;
    this->elseStatement = elseStatement;
}

IfStatement::~IfStatement() {
    delete(this->expr);
    delete(this->statement);
    delete(this->elseStatement);
}

void IfStatement::display() {
    xml.openTag("if");
    this->expr->display();
    displayList("block", this->statement);
    displayList("else-block", this->elseStatement);
    xml.closeTag();
}

void IfStatement::checkSemantic() {
    this->expr->checkSemantic();
    checkSemanticOnList(this->statement);
    checkSemanticOnList(this->elseStatement);

    if (this->expr->type != TYPE_INT) {
        error("Expresion no booleana en If");
    } else {
        this->type = TYPE_VOID;
    }
}