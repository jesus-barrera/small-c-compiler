#ifndef _NODE_H_INCLUDED_
#define _NODE_H_INCLUDED_

#include <string>
#include <map>
#include <fstream>

#include "XMLGenerator.h"
#include "SymbolsTable.h"

using namespace std;

#define MAX_PARAMS 6
#define AVAILABLE_REGS 7

class Node {
protected:
	static map<string, int> labels;
	static string params_registers[MAX_PARAMS];
	static string available_registres[AVAILABLE_REGS];

	static void error(string msg);
	static void checkSemanticOnList(Node* node, bool set_context = true);
	static void generateCodeOnList(Node* node, fstream &output);
	static void generateGlobalVarsCode(fstream &output);
	static string generateUniqueLabel(string label);
	static int countLocalFunctionVariables(string function_id);
	static string conditionalJump(string op);

public:
	static SymbolsTable symtable;
	static XMLGenerator xml;

	int type;
	string symbol;
	Node *next;

	Node(string simbolo);
	virtual ~Node();

	virtual void display();
	virtual void checkSemantic();
	virtual void generateCode(fstream &output);

	static void checkSemantic(Node *tree);
	static void generateCode(Node *tree, string filename);
	static void displayList(string wrapper_tag, Node*  node);
};

#endif