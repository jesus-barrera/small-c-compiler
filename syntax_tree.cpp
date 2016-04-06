#include "syntax_tree.h"

Node::Node(string symbol) {
	this->symbol = symbol;

	this->next = NULL;
}

Node::~Node() {
	delete(this->next);
}

void Node::display() {
	cout << this->symbol;
}

DataType::DataType(string symbol) 
		: Node(symbol) {}

Statement::Statement(string symbol) 
		: Node (symbol) {}

void displayStatements(Node *statement) {
	while (statement != NULL) {
		statement->display();
		cout << endl;

		statement = statement->next;
	}
}

Expression::Expression(string symbol) 
		: Statement(symbol) {}

Identifier::Identifier(string symbol) 
		: Expression(symbol) {}

Integer::Integer(string symbol) 
		: Expression(symbol) {}

AssignmentExpression::AssignmentExpression(Identifier *id, Expression *expr) 
		: Expression("=") {
	this->id = id;
	this->expr = expr;
}

AssignmentExpression::~AssignmentExpression() {
	delete(this->id);
	delete(this->expr);
}

void AssignmentExpression::display() {
	this->id->display();
	cout << " ";
	Node::display();
	cout << " ";
	this->expr->display();
}

BinaryExpression::BinaryExpression(string op, Expression *left, Expression *right) 
		: Expression(op){
	this->left = left;
	this->right = right;
}

BinaryExpression::~BinaryExpression() {
	delete(this->left);
	delete(this->right);
}

void BinaryExpression::display() {
	this->left->display();
	cout << " ";
	Node::display();
	cout << " ";
	this->right->display();
}

UnaryExpression::UnaryExpression(string symbol, Expression* expr)
		: Expression(symbol) {
	this->expr = expr;
}

UnaryExpression::~UnaryExpression() {
	delete(this->expr);
}

void UnaryExpression::display() {
	Node::display();
	cout << " ";
	this->expr->display();
}

IfStatement::IfStatement(Expression *exp, Statement *statement, Statement *elseStatement)
		: Statement("") {
	this->exp = exp;
	this->statement = statement;
	this->elseStatement = elseStatement;
}

IfStatement::~IfStatement() {
	delete(this->exp);
	delete(this->statement);
	delete(this->elseStatement);
}

void IfStatement::display() {
	cout << "if (";
	this->exp->display();
	cout << ") {" << endl;
	displayStatements(this->statement);
	cout << "}" << endl;
	
	if (this->elseStatement) {
		cout << "else {" << endl;
		displayStatements(this->elseStatement);
		cout << "}" << endl;
	}
}

WhileStatement::WhileStatement(Expression *exp, Statement *statement) 
		: Statement("") {
	this->exp = exp;
	this->statement = statement;
}

WhileStatement::~WhileStatement() {
	delete(this->exp);
	delete(this->statement);
}

void WhileStatement::display() {
	cout << "while (";
	this->exp->display();
	cout << ") {" << endl;
	displayStatements(this->statement);
	cout << "}" << endl;
}

DoWhileStatement::DoWhileStatement(Statement *statement, Expression *exp)
		: WhileStatement(exp, statement) {}

void DoWhileStatement::display() {
	cout << "do {" << endl;
	displayStatements(this->statement);
	cout << "} while (";
	this->exp->display();
	cout << ")" << endl;
}

ReturnStatement::ReturnStatement(Expression *exp)
		: Statement("") {
	this->exp = exp;
}

ReturnStatement::~ReturnStatement() {
	delete(this->exp);
}

void ReturnStatement::display() {
	cout << "return ";
	this->exp->display();
	cout << endl;
}

ForStatement::ForStatement(Expression *initializer, Expression *condition, Expression *step, Statement *statement) 
		: Statement("") {
	this->initializer = initializer;
	this->condition = condition;
	this->step = step;
	this->statement = statement;
}

ForStatement::~ForStatement() {
	delete(this->initializer);
	delete(this->condition);
	delete(this->step);
	delete(this->statement);
}

void ForStatement::display() {
	cout << "for (";
	this->initializer->display();
	cout << ";";
	this->condition->display();
	cout << ";";
	this->step->display();
	cout << ") {" << endl;
	displayStatements(this->statement);
	cout << "}" << endl;
}

Declarator::Declarator(Identifier *id, Parameter *param, Expression *exp) 
		: Node("") {
	this->id = id;
	this->param = param;
	this->exp = exp;
}

Declarator::~Declarator() {
	delete(this->id);
	delete(this->param);
	delete(this->exp);
}

void Declarator::display() {
	this->id->display();

	if (this->param) {
		cout << " (";
		this->display();
		cout << ")";
	} else if (this->exp) {
		cout << " = ";
		this->display();
	}

	if (this->next) {
		cout << ", ";
		this->next->display();
	}
}

Parameter::Parameter(DataType *type, Identifier *id) 
		: Node("") {
	this->type = type;
	this->id = id;
}

Parameter::~Parameter() {
	delete(type);
	delete(id);
}

void Parameter::display() {
	this->type->display();
	cout << " ";
	this->id->display();

	if (this->next) {
		cout << ", ";
		this->next->display();
	}
}

Declaration::Declaration(DataType *type, Declarator *declarator)
		: Node("") {
	this->type = type;
	this->declarator = declarator;
}

void Declaration::display() {
	this->type->display();
	cout << " ";
	this->declarator->display();
}

Declaration::~Declaration() {
	delete(this->type);
	delete(this->declarator);
}

FunctionDefinition::FunctionDefinition(DataType *type, Identifier *id, Parameter *param, Statement *statement)
		: Node("") { 

	this->type = type;
	this->id = id;
	this->param = param;
	this->statement = statement;
}

void FunctionDefinition::display() {
	this->type->display();
	cout << " ";
	this->id->display();
	cout << "(";
	this->param->display();
	cout << ") {" << endl;
	displayStatements(this->statement);
	cout << "}" << endl;
}

FunctionDefinition::~FunctionDefinition() {
	delete(this->type);
	delete(this->id);
	delete(this->param);
	delete(this->statement);
}