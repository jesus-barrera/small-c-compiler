#ifndef _SYNTAX_H_
#define _SYNTAX_H_

#include <list>
#include <cstdlib>
#include <exception>

#include "Lexical.h"
#include "syntax_tree.h"

using namespace std;

class Syntax {
private:
	list<Token* > *tkn_stream;
	Token lookahead;
	Node* tree;

public:
	Syntax(list<Token *> *tkn_stream);

	void analyze();

	void externalDeclaration();
	void declaration();
	void declaratorList();
	void declarator();
	void functionDefinition();
	void typeEspecifier();
	void parameterList();
	void statement();
	void compoundStatement();
	void statementList();
	void elseStatement();
	void expressionOpt();
	void expression();
	void assignmentExpression();
	void logicalOrExpression();
	void logicalAndExpression();
	void equalityExpression();
	void relationalExpression() ;
	void additiveExpression();
	void mutiplicativeExpression();
	void unaryExpression();
	void primaryExpression();
	void functionCall();
	void arguementList();

	void nextToken();
	void match(int tkn_type);
	void match(string tkn_symbol);

	void error(string expected);
};

#endif // _SYNTAX_H_