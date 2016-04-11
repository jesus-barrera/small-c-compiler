#include <iostream>

#include "Syntax.h"
#include "Lexical.h"

using namespace std;

int main(int argc, char *args[]) {
	string 	input_file;
	Lexical *lex = NULL;
	Syntax *syntax = NULL;

	input_file = args[1];
	
	try {
		lex = new Lexical(input_file);
		syntax = new Syntax(lex->scan());
		syntax->analyze();
		syntax->treeToXml();
	} catch (int e) {
		
	}

	delete(lex);
	delete(syntax);
}