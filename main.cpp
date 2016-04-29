#include <iostream>

#include "include/Syntax.h"
#include "include/Lexical.h"

using namespace std;

int main(int argc, char *args[]) {
	string 	input_file, output_file;
	Lexical *lex = NULL;
	Syntax *syntax = NULL;
	Node *n;

	if (argc >= 2) {
		input_file  = args[1];		
	}
	else {
		cout << "Argumentos no validos" << endl;
		return 0;
	}

	if (argc == 3) {
		output_file = args[2];
	}
	
	try {
		lex = new Lexical(input_file);
		syntax = new Syntax(lex->scan());
		syntax->analyze();
		n = syntax->getTree();

		syntax->treeToXml(output_file.empty() ? NULL : output_file.c_str());
		
		while (n != NULL) {
			n->checkSemantic();

			n = n->next;
		}

		syntax->getTree()->symtable.display();
	} catch (int e) {
		
	}

	delete(lex);
	delete(syntax);
}