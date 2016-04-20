#ifndef _SYNTAX_TREE_H_INCLUDED_

#define _SYNTAX_TREE_H_INCLUDED_

#include <iostream>
#include <string>

#include "XMLGenerator.h"

using namespace std;

extern XMLGenerator xml;

class Node {
public:
	string symbol;
	Node *next;

	Node(string simbolo);
	virtual ~Node();
	virtual void display();
};

class DataType: public Node {
public:
	DataType(string symbol);

	void display();
};

class Statement: public Node {
public:
	Statement(string symbol);
};

class Expression: public Statement {
public:
	Expression(string symbol);
};

class Identifier: public Expression {
public:
	Identifier(string symbol);

	void display();
};

class Integer: public Expression {
public:
	Integer(string symbol);

	void display();
};

class BinaryExpression: public Expression {
public:
	string op;
	Expression *left;
	Expression *right;

	BinaryExpression(string op, Expression *left, Expression* right);
	~BinaryExpression();
	
	void display();
};

class UnaryExpression: public Expression {
public:
	string op;
	Expression *expr;

	UnaryExpression(string op, Expression *expr);
	~UnaryExpression();

	void display();
};

class FunctionCall: public Expression {
public:
	Identifier *id;
	Expression *args;

	FunctionCall(Identifier *id, Expression *args);
	~FunctionCall();
	void display();
};

class IfStatement: public Statement {
public:
	Expression *exp;
	Statement *statement;
	Statement *elseStatement;

	IfStatement(Expression *exp, Statement *statement, Statement *elseStatement);
	~IfStatement();

	void display();
};

class WhileStatement: public Statement {
public:
	Expression *exp;
	Statement *statement;

	WhileStatement(Expression *exp, Statement *statement);
	~WhileStatement();

	void display();
};

class DoWhileStatement: public WhileStatement {
public:
	DoWhileStatement(Statement *statement, Expression *exp);

	void display();
};

class ReturnStatement: public Statement {
public:
	Expression *exp;

	ReturnStatement(Expression *exp);
	~ReturnStatement();

	void display();
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
};

class Parameter: public Node {
public:
	DataType *type;
	Identifier *id;

	Parameter(DataType *type, Identifier *id);
	~Parameter();

	void display();
};

class Declarator: public Node {
public:
	Identifier *id;
	Declarator(Identifier *id);
	~Declarator();
};

class VariableDeclarator: public Declarator {
public:
	Expression *init;

	VariableDeclarator(Identifier *id, Expression *initializer);
	~VariableDeclarator();

	void display();
};

class FunctionDeclarator: public Declarator {
public:
	Parameter *params;

	FunctionDeclarator(Identifier *id, Parameter *params);
	~FunctionDeclarator();

	void display();
};

class Declaration: public Statement {
public:
	DataType *type;
	Declarator *declarator;

	Declaration(DataType *type, Declarator *declarator);
	~Declaration();

	void display();
};

class FunctionDefinition: public Node {
public:
	DataType *type;
	Identifier *id;
	Parameter *param;
	Statement *statement;


	FunctionDefinition(DataType *type, Identifier *Identifier, Parameter *param, Statement *statement);
	~FunctionDefinition();

	void display();
};

#endif