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

void BinaryExpression::generateCode(fstream &output) {
    if (this->op == "=") {
        SymTabRecord *record;
        this->right->generateCode(output);

        record = symtable.get(this->left->symbol);

        output << "movl %eax, " << "-" << record->stack_offset << "(%rbp)" << endl;
    } else {
        this->left->generateCode(output);
        output << "movl %eax, %ebx" << endl;
        this->right->generateCode(output);


        if (this->op == "==" || this->op == "!=" ||
            this->op == ">"  || this->op == "<"  ||
            this->op == ">="  || this->op == "<=") {

            string true_label, end_label;

            true_label  = generateUniqueLabel("VERDADERO");
            end_label   = generateUniqueLabel("FIN");

            output << "; EXPRESION IGUALDAD" << endl;

            output << "cmpl %ebx, %eax" << endl;

            output << conditionalJump(this->op) << " " << true_label << endl;
            output << "movl $1, %eax" << endl;
            output << "jmp " << end_label << endl;
            output << true_label << ": " << endl;
            output << "movl $0, %eax" << endl;
            output << end_label << ": " << endl;

        } else if (this->op == "-" || this->op == "+") {
            output << "; EXPRESION ADDITIVA" << endl;
    
            if (this->op == "+") {
                output << "addl %ebx, %eax" << endl;
            } else if (this->op == "-") {
                output << "subl %ebx, %eax" << endl;
            }
        } else if (this->op == "*" || this->op == "/" || this->op == "%") {
            if (this->op == "*") {
                output << "imul %ebx" << endl;

            } else if (this->op == "/" || this->op == "%") {
                output << "xorl %edx, %edx" << endl;
                output << "idivl %ebx" << endl;

                if (this->op == "%") {
                    output << "movl %edx, %eax" << endl;
                }
            }
        }
    }
}