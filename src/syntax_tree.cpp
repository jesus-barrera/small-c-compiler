#include <sstream>
#include <cstdlib>
#include "../include/syntax_tree.h"

XMLGenerator Node::xml;
SymbolsTable Node::symtable;

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

void Node::checkSemanticOnList(Node* node, bool set_context) {
	if (set_context) symtable.setContext();
	
	while (node) {
		node->checkSemantic();
		node = node->next;
	}

	if (set_context) symtable.exitContext();
}

void Node::error(string msg) {
	cout << "[Error Semantico] " << msg << endl;
	throw 0;
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
Statement::Statement(): Node ("") {}

// Expression methods
Expression::Expression() {}

// Identifier methods
Identifier::Identifier(string symbol) {
	this->symbol = symbol;
}

void Identifier::display() {
	xml.oneLineTag("id", this->symbol);
}

void Identifier::checkSemantic() {
	SymTabRecord *record = symtable.get(this->symbol);

	if (record) {
		if (record->sym_type == SYM_VARIABLE) {
			this->type = record->type;
		} else {
			error("\"" + this->symbol + "\" es una funcion");
		}
	} else {
		error("Variable no definida: \"" +  this->symbol + "\"");
	}
}

// Integer methods
Integer::Integer(string symbol) {
	this->symbol = symbol;
}

void Integer::display() {
	xml.oneLineTag("integer", this->symbol);
}

void Integer::checkSemantic() {
	this->type = TYPE_INT;
}

// BinaryExpression methods
BinaryExpression::BinaryExpression(string op, Expression *left, Expression *right) {
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
		error("Expresion binaria (" + op + ") entre tipos no compatibles");
	}
}

// UnaryExpression methods
UnaryExpression::UnaryExpression(string op, Expression* expr) {
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
		this->type = TYPE_INT;
	} else {
		error("Operador unario " + this->op +" solo puede usarse en enteros");
	}
}

// FunctionCall methods
FunctionCall::FunctionCall(Identifier *id, Expression *args) {
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
	string identifier;

	identifier = this->id->symbol;
	
	// Check symbol is declared
	func = symtable.get(this->id->symbol);
	
	if (func  != NULL) {
		// Check symbol is a function
		if (func->sym_type == SYM_FUNCTION) {
			param = func->params;
			arg   = this->args;

			// Check arguments match function definition
			while (arg != NULL && param != NULL) {
				arg->checkSemantic();

				if (arg->type != param->type) {
					error("En llamada a funcion \"" + identifier + "\": los tipos de argumentos no coinciden");
				}

				arg = (Expression *)arg->next;
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
		error("Funcion \"" +  identifier + "\" no ha sido declarada");
	}
}

// IfStatement methods
IfStatement::IfStatement(Expression *expr, Statement *statement, Statement *elseStatement) {
	this->expr = expr;
	this->statement = statement;
	this->elseStatement = elseStatement;
}

IfStatement::~IfStatement() {
	delete(this->expr);
	delete(this->statement);
	delete(this->elseStatement);
}

void IfStatement::display() {
	xml.openTag("if");
	this->expr->display();
	displayList("block", this->statement);
	displayList("else-block", this->elseStatement);
	xml.closeTag();
}

void IfStatement::checkSemantic() {
	this->expr->checkSemantic();
	checkSemanticOnList(this->statement);
	checkSemanticOnList(this->elseStatement);

	if (this->expr->type != TYPE_INT) {
		error("Expresion no booleana en If");
	} else {
		this->type = TYPE_VOID;
	}
}

// WhileStatement methods
WhileStatement::WhileStatement(Expression *exp, Statement *statement) {
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
		error("Expresion no booleana en while");
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
ReturnStatement::ReturnStatement(Expression *expr) {
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

	context = symtable.getContext();

	if (this->expr) {
		this->expr->checkSemantic();
		
		if (context && context->type != expr->type) {
			error("En funcion " + context->symbol + ": tipo return no coincide");
		}
	}

	this->type = TYPE_VOID;
}

// ForStatement methods
ForStatement::ForStatement(Expression *initializer, Expression *condition, Expression *step, Statement *statement) {
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

void Declarator::checkSemantic(int type) {
	this->type = TYPE_VOID;
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

void VariableDeclarator::checkSemantic(int type) {
	// Check if the variable has been declared
	if (symtable.get(this->id->symbol, false) != NULL) {
		error("La variable \"" + this->id->symbol + "\" ya ha sido declarada");
	}

	// Check the declaration type is correct
	if (type != TYPE_INT) {
		error("Declaracion de variable \"" + this->id->symbol + "\" de tipo void");
	}

	// Check initializer
	if (this->init) {
		this->init->checkSemantic();

		if (this->init->type != type) {
			error("Inicializador de \"" + this->id->symbol + "\" tipo void");
		}
	}

	symtable.insert(this->id->symbol, SYM_VARIABLE, type, NULL);
	this->type = TYPE_VOID;
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

void FunctionDeclarator::checkSemantic(int type) {
	Parameter *node;
	Param *param = NULL, *aux = NULL;

	node = this->params;

	// Check if it is already declared
	if (symtable.get(this->id->symbol) != NULL) {
		error("Funcion \"" + this->id->symbol + "\" previamente declarada");
	}

	if (node) {
		node->checkSemantic();
		param = (Param *)malloc(sizeof(Param));
		param->type = node->data_type->getType();

		aux = param;
		node = (Parameter *)node->next;
	
		while (node) {
			node->checkSemantic();
			aux->next = (Param *)malloc(sizeof(Param));
			aux->next->type = node->data_type->getType();

			aux = aux->next;
			node = (Parameter *)node->next;
		}

		aux->next = NULL;
	}
	
	symtable.insert(this->id->symbol, SYM_FUNCTION, type, param, false);
}

// Parameter methods
Parameter::Parameter(DataType *data_type, Identifier *id) 
		: Node("") {
	this->data_type = data_type;
	this->id = id;
}

Parameter::~Parameter() {
	delete(data_type);
	delete(id);
}

void Parameter::display() {
	xml.openTag("parameter");
	this->data_type->display();
	this->id->display();
	xml.closeTag();
}

void Parameter::checkSemantic() {
	if (this->data_type->getType() == TYPE_VOID) {
		error("Parametro " + this->id->symbol + " no puede ser void");
	}

	this->type = TYPE_VOID;
}

// Declaration methods
Declaration::Declaration(DataType *data_type, Declarator *declarator) {
	this->data_type = data_type;
	this->declarator = declarator;
}

Declaration::~Declaration() {
	delete(this->data_type);
	delete(this->declarator);
}

void Declaration::display() {
	xml.openTag("declaration");
	this->data_type->display();
	displayList("declarator-list", this->declarator);
	xml.closeTag();
}

void Declaration::checkSemantic() {
	Declarator *node;
	int data_type;

	data_type = this->data_type->getType();

	node = this->declarator;

	while (node != NULL) {
		node->checkSemantic(data_type);

		node = (Declarator *)node->next;
	}
}

// FunctionDefinition methods
FunctionDefinition::FunctionDefinition(DataType *data_type, Identifier *id, Parameter *params, Statement *statement)
		: Node("") { 

	this->data_type = data_type;
	this->id = id;
	this->params = params;
	this->statement = statement;
}

FunctionDefinition::~FunctionDefinition() {
	delete(this->data_type);
	delete(this->id);
	delete(this->params);
	delete(this->statement);
}

void FunctionDefinition::display() {
	xml.openTag("function-definition");
	this->data_type->display();
	this->id->display();
	displayList("parameter-list", this->params);
	displayList("block", this->statement);
	xml.closeTag();
}

void FunctionDefinition::checkSemantic() {
	SymTabRecord *func;
	Parameter *def_param;
	string identifier;

	identifier = this->id->symbol;
	
	func = symtable.get(identifier);
	
	if (func) {
		Param *proto_param;

		if (func->defined) {
			error("Funcion \"" + this->id->symbol + "\": previamente declarada");
		}

		if (func->type != this->data_type->getType()) {
			error("Definicion de funcion \"" + this->id->symbol + "\": tipo no coincide con el prototipo");
		}

		// Check params match function prototype
		proto_param = func->params;
		def_param = this->params;

		while (def_param != NULL && proto_param != NULL) {
			def_param->checkSemantic();

			if (def_param->data_type->getType() != proto_param->type) {
				error("Definicion de funcion \"" + identifier + "\": tipos de argumentos no coincide con el prototipo");
			}

			def_param = (Parameter *)def_param->next;
			proto_param = proto_param->next;
		}

		if (def_param != NULL || proto_param != NULL) {
			error("Definicion de funcion \"" + identifier + "\": el numero de parametros no coincide con el prototipo");
		} else {
			func->defined = true;
			this->type = TYPE_VOID;
		}
	} else {
		Param *params = NULL, *aux = NULL;

		def_param = this->params;
		
		// Create paramss
		if (def_param) {
			def_param->checkSemantic();
			params = (Param *)malloc(sizeof(Param));
			params->type = def_param->data_type->getType();

			aux = params;
			def_param = (Parameter *)def_param->next;
		
			while (def_param) {
				def_param->checkSemantic();
				aux->next = (Param *)malloc(sizeof(Param));
				aux->next->type = def_param->data_type->getType();

				aux = aux->next;
				def_param = (Parameter *)def_param->next;
			}

			aux->next = NULL;
		}

		symtable.insert(identifier, SYM_FUNCTION, this->data_type->getType(), params);
	}

	symtable.setContext(identifier);

	// Insert parameters to symbols table
	def_param = this->params;
	while (def_param) {
		symtable.insert(def_param->id->symbol, SYM_VARIABLE, def_param->data_type->getType());
		def_param = (Parameter *)def_param->next;
	}

	checkSemanticOnList(this->statement, false);

	symtable.exitContext();
}
