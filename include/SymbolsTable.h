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

struct SymTabRecord {
	string symbol;
	int sym_type;
	int data_type;
	string context;
	vector<int> params;
	bool defined;
};

typedef vector<int> ParamList;

extern const char *str_data_types[];
extern const char *str_sym_types[];

class SymbolsTable {
private:
	vector<SymTabRecord> table;
	vector<string> curr_context;

public:
	SymbolsTable();
	vector<SymTabRecord> *getContainer();

	int insert(string &symbol, int sym_type, int data_type, ParamList *params = NULL, bool defined = true, bool use_context = true);
	SymTabRecord *get(string &symbol, bool bubble = true);
	SymTabRecord *getContext();
	void setContext(string context = "");
	void exitContext();
	
	void display();
	void clear();
};

#endif // _SYMBOLS_TABLE_H_INCLUDED_