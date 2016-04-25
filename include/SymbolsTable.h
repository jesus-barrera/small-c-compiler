#ifndef _SYMBOLS_TABLE_H_INCLUDED_
#define _SYMBOLS_TABLE_H_INCLUDED_

#include <vector>
#include <string>

using namespace std;

// Symbol types
enum {
	SYM_VARIABLE,
	SYM_FUNCTION
};

// Data types
enum {
	TYPE_INT,
	TYPE_VOID,
	TYPE_ERROR
};

// Params list
struct Param {
	int type;
	Param *next;
};

struct SymTabRecord {
	string symbol;
	int sym_type;
	int type;
	string context;
	Param *params;
	bool defined;
};

class SymbolsTable {
private:
	vector<SymTabRecord> table;
	vector<string> curr_context;

public:
	// Insets new symbol to the table
	int insert(string &symbol, int sym_type, int type, Param *params = NULL);

	// Gets the symbol record
	SymTabRecord get(string &symbol);

	// Sets the current context
	void setContext(string &context);

	// Exits the current context
	void exitContext();
};

#endif // _SYMBOLS_TABLE_H_INCLUDED_