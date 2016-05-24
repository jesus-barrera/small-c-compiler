#include "../include/FunctionCall.h"

FunctionCall::FunctionCall(Identifier *id, Expression *args) {
    this->id = id;
    this->args = args;
}

FunctionCall::~FunctionCall() {
    delete(id);
    delete(args);
}

void FunctionCall::display() {
    xml.openTag("function-call");
    this->id->display();
    displayList("argument-list", this->args);
    xml.closeTag();
}

void FunctionCall::checkFunctionArgs(SymTabRecord *function_def, Expression *args) {
    ParamList params;
    string identifier;
    Expression *arg;
    int i, j;

    identifier = function_def->symbol;
    params = function_def->params;

    arg = args;

    for (i = 0, j = params.size(); i < j && arg != NULL; i++) {
        arg->checkSemantic();

        if (arg->type != params[i]) {
            error("En llamada a funcion \"" + identifier + "\": los tipos de los argumentos no coinciden");
        }

        arg = (Expression *)arg->next;
    }

    if (arg != NULL || i < j) {
        error("En llamada a funcion \"" + identifier + "\": el numero de parametros no coincide");
    } 
}

void FunctionCall::checkSemantic() {
    SymTabRecord *function_def;
    string identifier;

    identifier = this->id->symbol;
    
    // Look for the symbol in the current context
    function_def = symtable.get(identifier);
    
    if (function_def != NULL) {
        if (function_def->sym_type == SYM_FUNCTION) {
            checkFunctionArgs(function_def, this->args);
            this->type = function_def->data_type;
        } else {
            error("\"" + identifier + "\" no es una funcion");
        }
    } else {
        error("En llamada a funcion \"" +  identifier + "\": la funcion no ha sido declarada");
    }
}