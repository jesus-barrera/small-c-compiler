#include <iostream>
#include <sstream>
#include <iomanip>
#include "../include/SymbolsTable.h"

SymbolsTable::SymbolsTable() {
	// Set global context
	curr_context.push_back("");
}

/**
 * Inserts a new symbol in the table
 */
int SymbolsTable::insert(string &symbol, int sym_type, int type, Param *params, bool defined) {
	SymTabRecord record;

	record.symbol = symbol;
	record.context = curr_context.back();
	record.sym_type = sym_type;
	record.type = type;
	record.params = params;
	record.defined = defined;

	table.push_back(record);
}

/**
 * Retrieves the first symbol accessible symbol by its context
 */
SymTabRecord *SymbolsTable::get(string &symbol, bool bubble) {
	string context;
	vector<SymTabRecord>::iterator record;

	int i, j;

	j = (bubble) ? 0 : i;

	for (int i = curr_context.size() - 1; i >= j; i--) {
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
	while (curr_context[i].find(ANONYM_CONTEXT_MARK) == 0) i--;

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
		stringstream context_name;
		context_name << ANONYM_CONTEXT_MARK << count;
		curr_context.push_back(context_name.str());

		count++;
	}
}

void SymbolsTable::exitContext() {
	curr_context.pop_back(); // Discard last inserted context
}

void SymbolsTable::display() {
	Param *param;

	const char *str_types[] = {
		"int",
		"void",
		"error"
	};

	const char *str_sym_types[] = {
		"var",
		"func"
	};

	cout << setw(10) << left << "Simbolo";
	cout << setw(10) << left << "Tipo";
	cout << setw(10) << left << "TipoDato";
	cout << setw(10) << left << "Context";
	cout << setw(10) << left << "Params";

	cout << endl;

	for (int i = 0, j = table.size(); i < j; i++) {
		cout << setw(10) << left << table[i].symbol;
		cout << setw(10) << left << str_sym_types[table[i].sym_type];
		cout << setw(10) << left << str_types[table[i].type];
		cout << setw(10) << left << table[i].context;

		param = table[i].params;

		while (param) {
			cout << str_types[param->type] << " ";

			param = param->next;
		}

		cout << endl;
	}
}

void SymbolsTable::deleteParams(Param *params) {
	if (params) {
		deleteParams(params->next);
		delete(params);
	}
}

void SymbolsTable::clear() {
	for (int i = 0, j = table.size(); i < j; i++) {
		deleteParams(table[i].params);
	}

	table.clear();
	while (!curr_context.back().empty()) curr_context.pop_back();
}