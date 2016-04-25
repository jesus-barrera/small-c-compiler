#include "../include/SymbolsTable.h"

int insert(string &symbol, int sym_type, int type, Param *params) {
	SymTabRecord record;
	record.context = curr_context.front();
	record.sym_type = sym_type;
	record.type = type;
}

SymTabRecord get(string &symbol) {

}

void setContext(string &context) {

}

void exitContext(string &context) {

}