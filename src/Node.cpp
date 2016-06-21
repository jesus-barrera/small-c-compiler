#include <iostream>
#include <sstream>
#include <cstdlib>

#include "../include/Node.h"

map<string, int> Node::labels;
string Node::params_registers[MAX_PARAMS]        = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};
string Node::available_registres[AVAILABLE_REGS] = {"rbx", "r10", "r11", "r12", "r13", "r14", "r15"};

XMLGenerator Node::xml;
SymbolsTable Node::symtable;

Node::Node(string symbol) {
    this->symbol = symbol;
    this->next = NULL;
    this->type = TYPE_ERROR;
}

Node::~Node() {
    delete(this->next);
}

void Node::display() {}

void Node::checkSemantic() {}

void Node::generateCode(fstream &output) {}

void Node::displayList(string wrapper_tag, Node*  node) {
    xml.openTag(wrapper_tag);

    while (node != NULL) {
        node->display();
        
        node = node->next;
    }

    xml.closeTag();
}

void Node::checkSemantic(Node *tree) {
    vector<SymTabRecord> *table;
    vector<SymTabRecord>::iterator record_it;
    SymTabRecord *record;
    
    checkSemanticOnList(tree, false);

    // Look for undefined functions
    table = symtable.getContainer();

    for (record_it = table->begin(); record_it != table->end(); record_it++) {
        record = &(*record_it);

        if (record->sym_type == SYM_FUNCTION && !record->defined) {
            error("Referencia a funcion no definida \"" + record->symbol + "\"");
        }
    }
}

void Node::generateCode(Node *tree, string filename) {
    Node *node;
    fstream output(filename.c_str(), ios::out);

    output << ".data" << endl;
    generateGlobalVarsCode(output);
    cout << endl;

    output << ".text" << endl;
    generateCodeOnList(tree, output);

    output << ".end" << endl;

    output.close();
}

void Node::error(string msg) {
    cout << "[Error Semantico] " << msg << endl;
    throw 0;
}

void Node::checkSemanticOnList(Node* node, bool set_context) {
    if (set_context) symtable.setContext();
    
    while (node) {
        node->checkSemantic();
        node = node->next;
    }

    if (set_context) symtable.exitContext();
}

void Node::generateCodeOnList(Node *node, fstream &output) {
    while (node) {
        node->generateCode(output);
        node = node->next;
    }
}

void Node::generateGlobalVarsCode(fstream &output) {
    vector<SymTabRecord>::iterator it;
    vector<SymTabRecord> *table;
    SymTabRecord *record;

    table = symtable.getContainer();

    for (it = table->begin(); it != table->end(); ++it) {
        record = &(*it);

        if (record->context.empty() && record->sym_type == SYM_VARIABLE) {
            output << record->symbol << ":" << endl;
            output << ".long 0" << endl;
        }
    }
}

string Node::generateUniqueLabel(string label) {
    stringstream unique_label;

    if (labels.count(label)) {
        labels[label] ++;
    } else {
        labels[label] = 0;
    }
    
    unique_label << label << labels[label];
    
    return unique_label.str();
}

int Node::countLocalFunctionVariables(string function_id) {
    vector<SymTabRecord>::iterator record_it;
    vector<SymTabRecord> *table;
    SymTabRecord *record;
    int total_vars;
    string anonym_context_prefix;

    total_vars = 0;
    anonym_context_prefix = "@" + function_id + "#";

    table = symtable.getContainer();

    for (record_it = table->begin(); record_it != table->end(); ++record_it) {
        record = &(*record_it);

        if ((record->context == function_id || record->context.find(anonym_context_prefix) == 0) &&
            record->sym_type == SYM_VARIABLE) {
            total_vars++;

            record->stack_offset = total_vars * 4;        
        }
    }

    return total_vars;
}

string Node::conditionalJump(string op) {
    if (op == ">") {
        return "jg";
    } else if (op == "<") {
        return "jl";
    } else if (op == ">=") {
        return "jge";
    } else if (op == "<=") {
        return "jle";
    } else if (op == "==") {
        return "je";
    } else if (op == "!=") {
        return "jne";
    } else {
        return "";
    }
}