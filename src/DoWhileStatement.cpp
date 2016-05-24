#include "../include/DoWhileStatement.h"

DoWhileStatement::DoWhileStatement(Node *statement, Expression *expr)
        : WhileStatement(expr, statement) {}

void DoWhileStatement::display() {
    xml.openTag("do-while");
    displayList("block", this->statement);
    this->expr->display();
    xml.closeTag();
}