#include <sstream>
#include "../include/syntax_tree.h"

XMLGenerator Node::xml;
SymbolsTable Node::symbols;

// Node methods
Node::Node(string symbol) {
	this->symbol = symbol;
	this->next = NULL;
	this->type = TYPE_ERROR;
}

Node::~Node() {
	delete(this->next);
}

void Node::display() {}

void Node::checkSemantic() {
	this->type = TYPE_ERROR;
}

void Node::displayList(string wrapper_tag, Node*  node) {
	xml.openTag(wrapper_tag);

	while (node != NULL) {
		node->display();
		
		node = node->next;
	}

	xml.closeTag();
}

void Node::checkSemanticOnList(Node* node) {
	symbols.newContext();

	while (node != NULL) {
		node->checkSemantic();
		
		node = node->next;
	}

	symbols.exitContext();
}

void Node::error(string msg) {
	cout << "[Error Semantico] " << msg << endl;
}

// DataType methods
DataType::DataType(string symbol) 
		: Node(symbol) {}

void DataType::display() {
	xml.oneLineTag("type", this->symbol);
}

int DataType::getType() {
	if (symbol == "int") {
		return TYPE_INT;
	} else if (symbol == "void") {
		return TYPE_VOID;
	} else {
		return TYPE_ERROR;
	}
}

// Statement methods
Statement::Statement(string symbol) 
		: Node (symbol) {}

// Expression methods
Expression::Expression(string symbol) 
		: Statement(symbol) {}

// Identifier methods
Identifier::Identifier(string symbol) 
		: Expression(symbol) {}

void Identifier::display() {
	xml.oneLineTag("id", this->symbol);
}

void Identifier::checkSemantic() {
	SymTabRecord *record = symbols.get(this->symbol);

	if (record) {
		if (record->sym_type == SYM_VARIABLE) {
			this->type = record->type;
		} else {
			error("Funcion \"" + this->symbol + "\" es usada como variable");
		}
	} else {
		error("Variable no definida: " +  this->symbol);
	}
}

// Integer methods
Integer::Integer(string symbol) 
		: Expression(symbol) {}

void Integer::display() {
	xml.oneLineTag("integer", this->symbol);
}

void Integer::checkSemantic() {
	this->type = TYPE_INT;
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

void BinaryExpression::checkSemantic() {
	this->left->checkSemantic();
	this->right->checkSemantic();

	if (this->left->type == TYPE_INT && this->right->type == TYPE_INT) {
		this->type = TYPE_INT;
	} else {
		error("Expresion binaria entre tipos no compatibles");
	}
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

void UnaryExpression::checkSemantic() {
	this->expr->checkSemantic();

	if (this->expr->type == TYPE_INT) {
		this->type = this->expr->type;
	} else {
		error("El operador unario en expresion void");
	}
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

void FunctionCall::checkSemantic() {
	SymTabRecord *func;
	Param *param;
	Expression *arg;

	err << "Llamada a funcion \"" << this->id->symbol << "\": ";

	func = symbols.get(this->id->symbol);
	
	if (func  != NULL) {
		identifier = func->symbol;

		// Check symbol is a function identifier
		if (func->sym_type == SYM_FUNCTION) {
			param = func->params;
			arg = this->args;

			// Check arguments match function definition
			while (arg != NULL && param != NULL) {
				arg->checkSemantic();

				if (arg->type != param->type) {
					error("En llamada a funcion \"" + identifier + "\": los tipos de argumentos no coinciden");
				}

				arg = arg->next;
				param = param->next;
			}

			if (arg != NULL || param != NULL) {
				error("En llamada a funcion \"" + identifier + "\": el numero de parametros no coincide");
			} else {
				this->type = func->type;
			}
		} else {
			error("\"" + identifier + "\" no es una funcion");
		}
	} else {
		error("\"" +  identifier + "\" no ha sido declarado");
	}
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

void IfStatement::checkSemantic() {
	this->exp->checkSemantic();
	checkSemanticOnList(this->statement);
	checkSemanticOnList(this->elseStatement);

	if (this->exp->type != TYPE_INT) {
		error("Condicion no booleana");
	} else {
		this->type = TYPE_VOID;
	}
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

void WhileStatement::checkSemantic() {
	this->exp->checkSemantic();

	checkSemanticOnList(this->statement);

	if (this->exp->type != TYPE_INT) {
		error("Condicion no booleana");
	} else {
		this->type = TYPE_VOID;
	}
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
ReturnStatement::ReturnStatement(Expression *expr)
		: Statement("") {
	this->expr = expr;
}

ReturnStatement::~ReturnStatement() {
	delete(this->expr);
}

void ReturnStatement::display() {
	xml.openTag("return");
	if (this->expr) this->expr->display();
	xml.closeTag();
}

void ReturnStatement::checkSemantic() {
	SymTabRecord *context;
	
	context = symbols.getContext();

	if (this->expr) {
		this->expr->checkSemantic();
		
		if (context->type == TYPE_VOID) {
			error("En funcion " + context->symbol + ": return debe ser void");
		}
	}
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

void ForStatement::checkSemantic() {
	if (this->initializer) this->initializer->checkSemantic();
	if (this->condition) this->condition->checkSemantic();
	if (this->step) this->initializer->checkSemantic();

	checkSemanticOnList(this->statement);

	this->type = TYPE_VOID;
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

void VariableDeclarator::checkSemantic() {
	
	if (this->init) {
		this->init->checkSemantic();

		if (this->init->type != TYPE_INT) {
			error("Asignacion de tipo no compatible");
		} else {

		}
	}
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
Declaration::Declaration(DataType *data_type, Declarator *declarator)
		: Statement("") {
	this->data_type = data_type;
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

void Declaration::checkSemantic() {
	Declarator *node;
	int data_type;

	data_type = this->data_type->getType();

	node = this->declarator;

	while (node != NULL) {
		node->checkSemantic(type);

		node = this->node->next;
	}
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
