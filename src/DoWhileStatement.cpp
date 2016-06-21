#include "../include/DoWhileStatement.h"

DoWhileStatement::DoWhileStatement(Node *statement, Expression *expr)
        : WhileStatement(expr, statement) {}

void DoWhileStatement::display() {
    xml.openTag("do-while");
    displayList("block", this->statement);
    this->expr->display();
    xml.closeTag();
}

void DoWhileStatement::generateCode(fstream &output) {
	string label;

    label = generateUniqueLabel("DO_WHILE");

    output << "; SENTENCIA DO_WHILE" << endl;

    output << label << ": " << endl;
    generateCodeOnList(this->statement, output);
    this->expr->generateCode(output);
    output << "cmpl %eax, $0" << endl;
    output << "jne " << label << endl;
}