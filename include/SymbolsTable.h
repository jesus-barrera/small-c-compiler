#ifndef _SYMBOLS_TABLE_H_INCLUDED_
#define _SYMBOLS_TABLE_H_INCLUDED_

#include <vector>
#include <string>

#define ANONYM_CONTEXT_MARK "#"

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

	void deleteParams(Param *params);

public:
	SymbolsTable();

	int insert(string &symbol, int sym_type, int type, Param *params = NULL, bool defined = true);
	SymTabRecord *get(string &symbol, bool bubble = true);
	
	SymTabRecord *getContext();
	void setContext(string context = "");
	void exitContext();
	
	void display();
	void clear();
};

#endif // _SYMBOLS_TABLE_H_INCLUDED_