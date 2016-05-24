#include <iostream>
#include <sstream>
#include <iomanip>
#include "../include/SymbolsTable.h"

const char *str_data_types[] = {
    "int",
    "void",
    "error"
};

const char *str_sym_types[] = {
    "variable",
    "funcion"
};

SymbolsTable::SymbolsTable() {
    // Set global context
    curr_context.push_back("");
}

vector<SymTabRecord> *SymbolsTable::getContainer() {
    return &table;
}

/**
 * Inserts a new symbol in the table
 */
int SymbolsTable::insert(string &symbol, int sym_type, int data_type, ParamList *params, bool defined, bool use_context) {
    SymTabRecord record;

    record.symbol = symbol;
    
    if (use_context)
        record.context = curr_context.back();
    else 
        record.context = "";

    record.sym_type = sym_type;
    record.data_type = data_type;
    
    if (params) {
        record.params = *params;
    }
    
    record.defined = defined;

    table.push_back(record);
}

/**
 * Retrieves the first symbol accessible by its context.
 * If bubble is true, the function will search for the symbol in the current and upper context. 
 * Otherwise only the current context will be search.
 */
SymTabRecord *SymbolsTable::get(string &symbol, bool bubble) {
    string context;
    vector<SymTabRecord>::iterator record;

    int i, j;

    i = curr_context.size() - 1;

    j = ((bubble) ? 0 : i);

    for (; i >= j; i--) {
        context = curr_context[i];

        for (record = table.begin(); record != table.end(); record++) {
            if ((*record).context == context && (*record).symbol == symbol) {
                return &(*record);
            }
        }
    }

    return NULL;
}

/**
 * Gets the current function context record in the symbols table
 */
SymTabRecord *SymbolsTable::getContext() {
    string func_id;
    int i;

    // Get first not anonymous context (function)
    i = curr_context.size() - 1;
    while (curr_context[i].find("@") == 0) i--;

    func_id = curr_context[i];

    if (func_id.empty()) {
        // global context, no record
        return NULL;
    } else {
        return get(func_id); // Get function info from table
    }
}

/**
 * Sets the current context by the given string or creates an anonymous one as necesary
 */
void SymbolsTable::setContext(string context) {
    static int count = 0;

    if (!context.empty()) {
        curr_context.push_back(context);
    } else {
        // Create anonymous context
        stringstream new_context;
        string parent_context;

        parent_context = (getContext())->symbol;

        new_context << "@" << parent_context << "#" << count;
        curr_context.push_back(new_context.str());

        count++;
    }
}

void SymbolsTable::exitContext() {
    // Discard last inserted context
    if (curr_context.size() > 1) curr_context.pop_back();
}

void SymbolsTable::display() {
    ParamList::iterator param_iter, end;

    cout << setw(10) << left << "Simbolo";
    cout << setw(10) << left << "TipoSimb";
    cout << setw(10) << left << "TipoDato";
    cout << setw(10) << left << "Contexto";
    cout << setw(10) << left << "Params";

    cout << endl;

    for (int i = 0, j = table.size(); i < j; i++) {
        cout << setw(10) << left << table[i].symbol;
        cout << setw(10) << left << str_sym_types[table[i].sym_type];
        cout << setw(10) << left << str_data_types[table[i].data_type];
        cout << setw(10) << left << table[i].context;

        param_iter = table[i].params.begin();
        end = table[i].params.end();

        while (param_iter != end) {
            cout << str_data_types[*param_iter] << " ";
            param_iter ++;
        }

        cout << endl;
    }
}

void SymbolsTable::clear() {
    table.clear();
    while (curr_context.size() > 1) curr_context.pop_back();
}