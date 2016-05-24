#include "../include/FunctionDeclarator.h"

FunctionDeclarator::FunctionDeclarator(Identifier *id, Parameter *params)
        : Declarator(id) {

    this->params = params;
}

FunctionDeclarator::~FunctionDeclarator() {
    delete(this->params);
}

void FunctionDeclarator::display() {
    xml.openTag("function-declarator");
    this->id->display();
    displayList("parameter-list", this->params);
    xml.closeTag();
}

void FunctionDeclarator::checkSemantic(int type) {
    SymTabRecord *function_def;
    SymTabRecord *context;

    // Look for previous declarations
    function_def = symtable.get(this->id->symbol);
    context = symtable.getContext();

    if (function_def == NULL) {
        // Function has not been declared, add to table.
        Parameter *param_node;
        ParamList param_list;
        
        param_node = this->params;
        while (param_node) {
            param_node->checkSemantic();
            param_list.push_back(param_node->data_type->getType());

            param_node = (Parameter *)param_node->next;
        }
        
        if (context) {
            // Inside a function
            symtable.insert(this->id->symbol, SYM_FUNCTION, type, &param_list);
            symtable.insert(this->id->symbol, SYM_FUNCTION, type, &param_list, false, false);
        } else {
            // In global context, just one declaration
            symtable.insert(this->id->symbol, SYM_FUNCTION, type, &param_list, false);
        }
    } else {
        // Previous declaration found, check inconsistencies
        if (function_def->sym_type != SYM_FUNCTION) {
            error("Redeclaracion de \"" + function_def->symbol + "\" como funcion");
        }

        checkFunctionDeclaration(type, this->params, function_def);
        
        if ((!context && !function_def->context.empty()) || (context && context->context != function_def->context)) {
            // Add entry for current context
            symtable.insert(
                            function_def->symbol, 
                            function_def->sym_type, 
                            function_def->data_type, 
                            &function_def->params, 
                            true
                            );
        }
    }

    this->type = TYPE_VOID;
}

void FunctionDeclarator::checkFunctionDeclaration(int data_type, Parameter *params, SymTabRecord *function_def) {
    string identifier;
    Parameter *curr_param;
    ParamList *prev_params;
    int i, j;

    identifier = function_def->symbol;

    // Check return type
    if (data_type != function_def->data_type) {
        error("Conflicto con declaracion previa de \"" + identifier + "\": el tipo de retorno no coincide");
    }

    // Check parameters number and types
    curr_param = params;
    prev_params = &function_def->params;

    i = 0;
    j = prev_params->size();

    for (; i < j && curr_param != NULL; i++) {
        if (curr_param->data_type->getType() != (*prev_params)[i]) {
            error("Conflicto con declaracion previa de \"" + identifier + "\": los tipos de los parametros no coinciden");
        }

        curr_param = (Parameter *)curr_param->next;
    }

    if (curr_param != NULL || i < j) {
        error("Conflicto con declaracion previa de \"" + identifier + "\": el numero de parametros no coincide");
    }
}