#include <iostream>

#include "Syntax.h"
#include "Lexical.h"
#include <list>

using namespace std;

void display(Node *n) {
	bool cont = true;

	while (n != NULL) {
		n->display();
		
		n = n->next;

		if (n != NULL) {
			cout << endl;
		}
	}
}

int main(int argc, char *args[]) {
	string 	file_name;
	Lexical *lex = NULL;
	Syntax *syntax = NULL;
	Node *tree = NULL;

	file_name = args[1];
	
	try {
		// Get token stream from lexical analysis
		lex = new Lexical(file_name);

		// Get syntactic tree from syntactic analysis
		syntax = new Syntax(lex->scan());
		syntax->analyze();

		// display(tree);
	} catch (int e) {
		
	}

	// delete(tree);
	delete(lex);
	delete(syntax);
}

