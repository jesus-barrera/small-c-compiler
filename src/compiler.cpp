#include <iostream>

#include "../include/Syntax.h"
#include "../include/Lexical.h"

using namespace std;

int main(int argc, char *args[]) {
    string input_file;
    string output_file;

    Lexical lex;
    Syntax syntax;
    
    TokenStream *tokens = NULL;
    Node *tree = NULL;

    if (argc == 3) {
        input_file  = args[1];
        
        output_file = args[2];
    } else {
        cout << "[Error] Se esperaban dos argumentos (archivo de entrada, archivo de salida)" << endl;
        return 0;
    }

    try {
        tokens = lex.analyze(input_file);
        tree   = syntax.analyze(tokens);

        // Display tree
        // Node::xml.setoutput(&cout);
        // Node::displayList("program", tree);
        
        // Semantic analysis
        Node::checkSemantic(tree);
        Node::generateCode(tree, output_file);
        Node::symtable.display();
    } catch (int e) {
        
    }

    lex.destroyTokens(tokens);
    delete(tree);
}