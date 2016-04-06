#ifndef _SYNTAX_TREE_H

#define _SYNTAX_TREE_H
#include <iostream>
#include <string>

using namespace std;

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
};

class Integer: public Expression {
public:
	Integer(string symbol);
};

class AssignmentExpression: public Expression {
public:
	Identifier *id;
	Expression *expr;

	AssignmentExpression(Identifier *id, Expression *expr);
	~AssignmentExpression();

	void display();
};

class BinaryExpression: public Expression {
public:
	Expression *left;
	Expression *right;

	BinaryExpression(string op, Expression *left, Expression* right);
	~BinaryExpression();
	
	void display();
};

class UnaryExpression: public Expression {
public:
	Expression *expr;

	UnaryExpression(string op, Expression* expr);
	~UnaryExpression();

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
	Parameter *param; // In case of function declaration
	Expression *exp; // In case of variable initialization

	Declarator(Identifier *id, Parameter *param = NULL, Expression *exp = NULL);
	~Declarator();

	void display();
};

class Declaration: public Node {
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