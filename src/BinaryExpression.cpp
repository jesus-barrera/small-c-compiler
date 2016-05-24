#include "../include/BinaryExpression.h"

BinaryExpression::BinaryExpression(string op, Expression *left, Expression *right) {
    this->op = op;
    this->left = left;
    this->right = right;
}

BinaryExpression::~BinaryExpression() {
    delete(this->left);
    delete(this->right);
}

void BinaryExpression::display() {
    xml.openTag("binary-expression", this->op);
    this->left->display();
    this->right->display();
    xml.closeTag();
}

void BinaryExpression::checkSemantic() {
    this->left->checkSemantic();
    this->right->checkSemantic();

    if (this->left->type == TYPE_INT && this->right->type == TYPE_INT) {
        this->type = TYPE_INT;
    } else {
        error("Expresion binaria (" + op + ") entre tipos no compatibles");
    }
}