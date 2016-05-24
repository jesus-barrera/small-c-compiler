#ifndef _NODE_H_INCLUDED_
#define _NODE_H_INCLUDED_

#include <string>

#include "XMLGenerator.h"
#include "SymbolsTable.h"

using namespace std;

class Node {
protected:
	static void error(string msg);
	
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
	static void checkSemanticOnList(Node* node, bool set_context = true);
	static void displayList(string wrapper_tag, Node*  node);
};

#endif