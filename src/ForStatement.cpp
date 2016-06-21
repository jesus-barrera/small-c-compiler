#include "../include/ForStatement.h"

ForStatement::ForStatement(Expression *initializer, Expression *condition, Expression *step, Node *statement)
        : Node("") {

    this->initializer = initializer;
    this->condition = condition;
    this->step = step;
    this->statement = statement;
}

ForStatement::~ForStatement() {
    delete(this->initializer);
    delete(this->condition);
    delete(this->step);
    delete(this->statement);
}

void ForStatement::display() {
    xml.openTag("for");
    if (this->initializer) this->initializer->display();
    if (this->condition) this->condition->display();
    if (this->step) this->step->display();
    displayList("block", this->statement);
    xml.closeTag();
}

void ForStatement::checkSemantic() {
    if (this->initializer) this->initializer->checkSemantic();
    if (this->condition) this->condition->checkSemantic();
    if (this->step) this->initializer->checkSemantic();

    checkSemanticOnList(this->statement);

    if (this->condition && this->condition->type != TYPE_INT) {
        error("Condicion no booleana en For");
    }

    this->type = TYPE_VOID;
}

void ForStatement::generateCode(fstream &output) {
    string for_label, end_label;

    for_label = generateUniqueLabel("FOR");
    end_label   = generateUniqueLabel("FIN_FOR");

    output << "; SENTENCIA FOR" << endl;

    if (this->initializer) this->initializer->generateCode(output);
    output << for_label << ": " << endl;
    if (this->condition) this->condition->generateCode(output);
    output << "cmpl %eax, $0" << endl;
    output << "je " << end_label << endl;
    generateCodeOnList(this->statement, output);
    if (this->step) this->step->generateCode(output);
    output << "jmp " << for_label << endl;
    output << end_label << ": " << endl;
}
