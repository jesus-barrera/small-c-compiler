#include "../include/syntax_tree.h"

XMLGenerator xml;

void displayList(string wrapper_tag, Node*  node) {
	xml.openTag(wrapper_tag);

	while (node != NULL) {
		node->display();
		
		node = node->next;
	}

	xml.closeTag();
}

// Node methods
Node::Node(string symbol) {
	this->symbol = symbol;

	this->next = NULL;
}

Node::~Node() {
	delete(this->next);
}

void Node::display() {}

// DataType methods
DataType::DataType(string symbol) 
		: Node(symbol) {}

void DataType::display() {
	xml.oneLineTag("type", this->symbol);
}

// Statement methods
Statement::Statement(string symbol) 
		: Node (symbol) {}

// Expression methods
Expression::Expression(string symbol) 
		: Statement(symbol) {}

// Expression methods
Identifier::Identifier(string symbol) 
		: Expression(symbol) {}

// Identifier methods
void Identifier::display() {
	xml.oneLineTag("id", this->symbol);
}

// Integer methods
Integer::Integer(string symbol) 
		: Expression(symbol) {}

void Integer::display() {
	xml.oneLineTag("integer", this->symbol);
}

// BinaryExpression methods
BinaryExpression::BinaryExpression(string op, Expression *left, Expression *right) 
		: Expression("") {
	this->op = op;
	this->left = left;
	this->right = right;
}

BinaryExpression::~BinaryExpression() {
	delete(this->left);
	delete(this->right);
}

void BinaryExpression::display() {
	xml.openTag("binary-expression", this->op);
	this->left->display();
	this->right->display();
	xml.closeTag();
}

// UnaryExpression methods
UnaryExpression::UnaryExpression(string op, Expression* expr)
		: Expression("") {
	this->op = op;
	this->expr = expr;
}

UnaryExpression::~UnaryExpression() {
	delete(this->expr);
}

void UnaryExpression::display() {
	xml.openTag("unary-expression", this->op);
	this->expr->display();
	xml.closeTag();
}

// FunctionCall methods
FunctionCall::FunctionCall(Identifier *id, Expression *args) 
		: Expression("") {
	this->id = id;
	this->args = args;
}

FunctionCall::~FunctionCall() {
	delete(id);
	delete(args);
}

void FunctionCall::display() {
	xml.openTag("function-call");
	this->id->display();
	displayList("argument-list", this->args);
	xml.closeTag();
}

// IfStatement methods
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
	xml.openTag("if");
	this->exp->display();
	displayList("block", this->statement);
	displayList("else-block", this->elseStatement);
	xml.closeTag();
}

// WhileStatement methods
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
	xml.openTag("while");
	this->exp->display();
	displayList("block", this->statement);
	xml.closeTag();
}

// DoWhileStatement methods
DoWhileStatement::DoWhileStatement(Statement *statement, Expression *exp)
		: WhileStatement(exp, statement) {}

void DoWhileStatement::display() {
	xml.openTag("do-while");
	displayList("block", this->statement);
	this->exp->display();
	xml.closeTag();
}

// ReturnStatement methods
ReturnStatement::ReturnStatement(Expression *exp)
		: Statement("") {
	this->exp = exp;
}

ReturnStatement::~ReturnStatement() {
	delete(this->exp);
}

void ReturnStatement::display() {
	xml.openTag("return");
	if (this->exp) this->exp->display();
	xml.closeTag();
}

// ForStatement methods
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
	xml.openTag("for");
	if (this->initializer) this->initializer->display();
	if (this->condition) this->condition->display();
	if (this->step) this->step->display();
	displayList("block", this->statement);
	xml.closeTag();
}

// Declarator methods
Declarator::Declarator(Identifier *id) 
		: Node("") {
	this->id = id;
}

Declarator::~Declarator() {
	delete(this->id);
}

// VariableDeclarator methods
VariableDeclarator::VariableDeclarator(Identifier *id, Expression *init)
		: Declarator(id) {
	this->init = init;
}

VariableDeclarator::~VariableDeclarator() {
	delete(this->init);
}

void VariableDeclarator::display() {
	xml.openTag("variable-declarator");
	this->id->display();
	if (this->init) this->init->display();
	xml.closeTag();
}

// FunctionDeclarator methods
FunctionDeclarator::FunctionDeclarator(Identifier *id, Parameter *params)
		: Declarator(id) {

	this->params = params;
}

FunctionDeclarator::~FunctionDeclarator() {
	delete(this->params);
}

void FunctionDeclarator::display() {
	xml.openTag("function-declarator");
	this->id->display();
	displayList("parameter-list", this->params);
	xml.closeTag();
}

// Parameter methods
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
	xml.openTag("parameter");
	this->type->display();
	this->id->display();
	xml.closeTag();
}

// Declaration methods
Declaration::Declaration(DataType *type, Declarator *declarator)
		: Statement("") {
	this->type = type;
	this->declarator = declarator;
}

Declaration::~Declaration() {
	delete(this->type);
	delete(this->declarator);
}

void Declaration::display() {
	xml.openTag("declaration");
	this->type->display();
	displayList("declarator-list", this->declarator);
	xml.closeTag();
}

// FunctionDefinition methods
FunctionDefinition::FunctionDefinition(DataType *type, Identifier *id, Parameter *param, Statement *statement)
		: Node("") { 

	this->type = type;
	this->id = id;
	this->param = param;
	this->statement = statement;
}

FunctionDefinition::~FunctionDefinition() {
	delete(this->type);
	delete(this->id);
	delete(this->param);
	delete(this->statement);
}

void FunctionDefinition::display() {
	xml.openTag("function-definition");
	this->type->display();
	this->id->display();
	displayList("parameter-list", this->param);
	displayList("block", this->statement);
	xml.closeTag();
}
