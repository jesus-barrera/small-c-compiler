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

void WhileStatement::generateCode(fstream &output) {
    string while_label, end_label;

    while_label = generateUniqueLabel("WHILE");
    end_label   = generateUniqueLabel("FIN_WHILE");

    output << "; SENTENCIA WHILE" << endl;

    output << while_label << ": " << endl;
    this->expr->generateCode(output);
    output << "cmpl %eax, $0" << endl;
    output << "je " << end_label << endl;
    generateCodeOnList(this->statement, output);
    output << "jmp " << while_label << endl;
    output << end_label << ": " << endl;
}