#ifndef _SYNTAX_TREE_H_INCLUDED_

#define _SYNTAX_TREE_H_INCLUDED_

#include <iostream>
#include <string>

#include "XMLGenerator.h"
#include "SymbolsTable.h"

using namespace std;

class Node {
protected:
	static void error(string msg);
	static void displayList(string wrapper_tag, Node*  node);
	static void checkSemanticOnList(Node* node, bool set_context = true);
	
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
};

class DataType: public Node {
public:
	DataType(string symbol);
	int getType();

	void display();
};

class Statement: public Node {
public:
	Statement();
};

class Expression: public Statement {
public:
	Expression();
};

class Identifier: public Expression {
public:
	Identifier(string symbol);

	void display();
	void checkSemantic();
};

class Integer: public Expression {
public:
	Integer(string symbol);

	void display();
	void checkSemantic();
};

class BinaryExpression: public Expression {
public:
	string op;
	Expression *left;
	Expression *right;

	BinaryExpression(string op, Expression *left, Expression* right);
	~BinaryExpression();
	
	void display();
	void checkSemantic();
};

class UnaryExpression: public Expression {
public:
	string op;
	Expression *expr;

	UnaryExpression(string op, Expression *expr);
	~UnaryExpression();

	void display();
	void checkSemantic();
};

class FunctionCall: public Expression {
public:
	Identifier *id;
	Expression *args;

	FunctionCall(Identifier *id, Expression *args);
	~FunctionCall();

	void display();
	void checkSemantic();
};

class IfStatement: public Statement {
public:
	Expression *expr;
	Statement *statement;
	Statement *elseStatement;

	IfStatement(Expression *expr, Statement *statement, Statement *elseStatement);
	~IfStatement();

	void display();
	void checkSemantic();
};

class WhileStatement: public Statement {
public:
	Expression *exp;
	Statement *statement;

	WhileStatement(Expression *exp, Statement *statement);
	~WhileStatement();

	void display();
	void checkSemantic();
};

class DoWhileStatement: public WhileStatement {
public:
	DoWhileStatement(Statement *statement, Expression *exp);

	void display();
};

class ReturnStatement: public Statement {
public:
	Expression *expr;

	ReturnStatement(Expression *expr);
	~ReturnStatement();

	void display();
	void checkSemantic();
};

class ForStatement: public Statement {
public:
	Expression *initializer;
	Expression *condition;
	Expression *step;
	Statement *statement;

	ForStatement(Expression *initializer, Expression *condition, Expression *step, Statement *statement);
	~ForStatement();

	void display();
	void checkSemantic();
};

class Parameter: public Node {
public:
	DataType *data_type;
	Identifier *id;

	Parameter(DataType *data_type, Identifier *id);
	~Parameter();

	void display();
	void checkSemantic();
};

class Declarator: public Node {
public:
	Identifier *id;
	Declarator(Identifier *id);
	~Declarator();

	virtual void checkSemantic(int type);
};

class VariableDeclarator: public Declarator {
public:
	Expression *init;

	VariableDeclarator(Identifier *id, Expression *initializer);
	~VariableDeclarator();

	void display();
	void checkSemantic(int type);
};

class FunctionDeclarator: public Declarator {
public:
	Parameter *params;

	FunctionDeclarator(Identifier *id, Parameter *params);
	~FunctionDeclarator();

	void display();
	void checkSemantic(int type);
};

class Declaration: public Statement {
public:
	DataType *data_type;
	Declarator *declarator;

	Declaration(DataType *data_type, Declarator *declarator);
	~Declaration();

	void display();
	void checkSemantic();
};

class FunctionDefinition: public Node {
public:
	DataType *data_type;
	Identifier *id;
	Parameter *params;
	Statement *statement;


	FunctionDefinition(DataType *data_type, Identifier *Identifier, Parameter *params, Statement *statement);
	~FunctionDefinition();

	void display();
	void checkSemantic();
};

#endif