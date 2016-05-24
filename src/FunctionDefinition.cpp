#include "../include/FunctionDeclarator.h"

#include "../include/FunctionDefinition.h"

FunctionDefinition::FunctionDefinition(DataType *data_type, Identifier *id, Parameter *params, Node *statement)
        : Node("") { 

    this->data_type = data_type;
    this->id = id;
    this->params = params;
    this->statement = statement;
}

FunctionDefinition::~FunctionDefinition() {
    delete(this->data_type);
    delete(this->id);
    delete(this->params);
    delete(this->statement);
}

void FunctionDefinition::display() {
    xml.openTag("function-definition");
    this->data_type->display();
    this->id->display();
    displayList("parameter-list", this->params);
    displayList("block", this->statement);
    xml.closeTag();
}

void FunctionDefinition::checkSemantic() {
    SymTabRecord *func;
    Parameter *def_param;
    string identifier;

    identifier = this->id->symbol;
    
    // Look for previous declaration
    func = symtable.get(identifier);
    
    if (func) {
        // Function already declared
        ParamList proto_params;
        int i, j;

        if (func->sym_type != SYM_FUNCTION) {
            error("Redeclaracion de \"" + identifier + "\" como funcion");
        }

        if (func->defined) {
            error("Funcion \"" + func->symbol + "\": previamente definida");
        }

        FunctionDeclarator::checkFunctionDeclaration(this->data_type->getType(), this->params, func);

        func->defined = true;
        this->type    = TYPE_VOID;
    } else {
        // New function declaration
        ParamList params;

        def_param = this->params;
        
        while (def_param) {
            def_param->checkSemantic();
            params.push_back(def_param->data_type->getType());

            def_param = (Parameter *)def_param->next;
        }

        symtable.insert(identifier, SYM_FUNCTION, this->data_type->getType(), &params);
    }

    symtable.setContext(identifier);

    // Insert parameters to symbols table
    def_param = this->params;

    while (def_param) {
        symtable.insert(def_param->id->symbol, SYM_VARIABLE, def_param->data_type->getType());
        def_param = (Parameter *)def_param->next;
    }

    checkSemanticOnList(this->statement, false);

    symtable.exitContext();
}