#include "../include/UnaryExpression.h"

UnaryExpression::UnaryExpression(string op, Expression* expr) {
    this->op = op;
    this->expr = expr;
}

UnaryExpression::~UnaryExpression() {
    delete(this->expr);
}

void UnaryExpression::display() {
    xml.openTag("unary-expression", this->op);
    this->expr->display();
    xml.closeTag();
}

void UnaryExpression::checkSemantic() {
    this->expr->checkSemantic();

    if (this->expr->type == TYPE_INT) {
        this->type = TYPE_INT;
    } else {
        error("Operador unario (" + this->op + ") sobre expresion tipo" + str_data_types[this->expr->type]);
    }
}

void UnaryExpression::generateCode(fstream &output) {
    output << "; EXPRESION UNARIA" << endl;

    this->expr->generateCode(output);

    if (this->op == "-") {
        output << "negl %eax" << endl;
    }
}