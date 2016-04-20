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
	list<Token *> *tkn_stream;
	Token lookahead;
	Node *tree;

public:
	Syntax(list<Token *> *tkn_stream);

	void analyze();

	Node *externalDeclaration();
	Node *declaration(DataType *type, Identifier *identifier);
	Declarator *declaratorList();
	Declarator *declarator();
	Node *functionDefinition(DataType *type, Identifier *identifier, Parameter *param);
	DataType *typeEspecifier();
	Parameter *parameterList();
	Statement *statement();
	Statement *compoundStatement();
	Statement *statementList();
	Statement *elseStatement();
	Expression *expressionOpt();
	Expression *expression();
	Expression *assignmentExpression();
	Expression *logicalOrExpression();
	Expression *logicalAndExpression();
	Expression *equalityExpression();
	Expression *relationalExpression() ;
	Expression *additiveExpression();
	Expression *mutiplicativeExpression();
	Expression *unaryExpression();
	Expression *primaryExpression();
	Expression *functionCall(Identifier *id);
	Expression *argumentList();

	void nextToken();
	void match(int tkn_type);
	void match(string tkn_symbol);

	void error(string expected);

	void treeToXml(const char *outfilename);
};

#endif // _SYNTAX_H_