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

void IfStatement::generateCode(fstream &output) {
    string end_label, else_label;

    else_label = generateUniqueLabel("ELSE_IF");
    end_label = generateUniqueLabel("FIN_IF");
    
    output << "; SENTENCIA IF"  << endl;

    this->expr->generateCode(output);
    output << "cmpl $0, %eax" << endl;
    output << "je " << else_label << endl;
    generateCodeOnList(this->statement, output);
    output << "jmp " << end_label << endl;
    output << else_label << ": " << endl;
    generateCodeOnList(this->elseStatement, output);
    output << end_label << ": " << endl;
}