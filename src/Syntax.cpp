#include "../include/Syntax.h"
 
Syntax::Syntax(list<Token *> *tkn_stream) {
	this->tkn_stream = tkn_stream;
}

void Syntax::analyze() {
	nextToken();
	
	tree = externalDeclaration();

	match(TKN_EOF);
}

Node *Syntax::externalDeclaration() {
	Node *node = NULL;
	DataType *type = NULL;
	Identifier *identifier = NULL;

	if (lookahead.is("void") || lookahead.is("int")) {
		type = typeEspecifier();
		identifier = new Identifier(lookahead.symbol);
		match(TKN_IDENTIFIER);
		node = declaration(type, identifier);
		node->next = externalDeclaration();
	}

	return node;
}

Node *Syntax::declaration(DataType *type, Identifier *identifier) {
	Node *node = NULL;
	Declarator *declarator = NULL;

	if (lookahead.is("=")) {
		Expression *init = NULL;

		match("=");
		init = assignmentExpression();
		declarator = new VariableDeclarator(identifier, init);
		declarator->next = declaratorList();
		match(";");

		node = new Declaration(type, declarator); 
	} else if (lookahead.is("(")) {
		Parameter *param;

		match("("); 
		param = parameterList(); 
		match(")");
		
		node = functionDefinition(type, identifier, param);
	} else {
		declarator = new VariableDeclarator(identifier, NULL);
		declarator->next = declaratorList();
		match(";");

		node = new Declaration(type, declarator);
	}

	return node;
}

Declarator *Syntax::declaratorList() {
	Declarator *node = NULL;
	
	if (lookahead.is(",")) {		
		match(",");
		node = declarator();
		node->next = declaratorList();
	}

	return node;
}

Declarator *Syntax::declarator() {
	Declarator *node = NULL;
	Identifier *identifier = NULL;

	identifier = new Identifier(lookahead.symbol);
	match(TKN_IDENTIFIER);

	if (lookahead.is("(")) {
		Parameter *params;

		match("(");
		params = parameterList();
		match(")");	

		node = new FunctionDeclarator(identifier, params);
	} else {
		Expression *init = NULL;

		if (lookahead.is("=")) {
			match("=");
			init = assignmentExpression();
		}

		node = new VariableDeclarator(identifier, init);
	}

	return node;
}

Node *Syntax::functionDefinition(DataType *type, Identifier *identifier, Parameter *param) {
	Node *node = NULL;

	if (lookahead.is("{")) {
		Statement *statement = compoundStatement();

		node = new FunctionDefinition(type, identifier, param, statement);
	} else {
		FunctionDeclarator *declarator = new FunctionDeclarator(identifier, param);

		declarator->next = declaratorList();
		match(";");

		node = new Declaration(type, declarator);
	}

	return node;
}

DataType *Syntax::typeEspecifier() {
	DataType *node = NULL;

	node = new DataType(lookahead.symbol);

	if (lookahead.is("int")) {
		match("int");
	} else {
		match("void");
	}

	return node;
}

Parameter *Syntax::parameterList() {
	Parameter *node = NULL, *aux;

	if (lookahead.is("void") || lookahead.is("int")) {
		DataType *type = typeEspecifier();
		Identifier *id = new Identifier(lookahead.symbol);

		match(TKN_IDENTIFIER);

		node = new Parameter(type, id);

		aux = node;

		while (lookahead.is(",")) {
			match(",");

			type = typeEspecifier();
			id = new Identifier(lookahead.symbol);
			match(TKN_IDENTIFIER);

			aux->next = new Parameter(type, id);

			aux = (Parameter *)aux->next;
		}
	}

	return node;
}

Statement *Syntax::statement() {
	Statement *node = NULL;

	if (lookahead.is("if")) {
		Expression *expr;
		Statement *stm, *elseStm;

		match("if");
		match("(");
		expr = expression();
		match(")");
		stm = statement();
		elseStm = elseStatement();

		node = new IfStatement(expr, stm, elseStm);
	} else if (lookahead.is("while")) {
		Expression *expr;
		Statement *stm;

		match("while");
		match("(");
		expr = expression();
		match(")");
		stm = statement();

		node = new WhileStatement(expr, stm);
	} else if (lookahead.is("do")) {
		Statement *stm;
		Expression *expr;

		match("do");
		stm = statement();
		match("while");
		match("(");
		expr = expression();
		match(")");
		match(";");

		node = new DoWhileStatement(stm, expr);
	} else if (lookahead.is("for")) {
		Expression *initializer, *condition, *step;
		Statement *stm;

		match("for");
		match("(");
		initializer = expressionOpt();
		match(";");
		condition = expressionOpt();
		match(";");
		step = expressionOpt();
		match(")");
		stm = statement();

		node = new ForStatement(initializer, condition, step, stm);
	} else if (lookahead.is("void") || lookahead.is("int")) {
		DataType *type;
		Declarator *dec;

		type = typeEspecifier();
		dec = declarator();
		dec->next = declaratorList();
		match(";");

		node = new Declaration(type, dec);
	} else if (lookahead.is("return")) {
		Expression *expr;
		
		match("return");
		expr = expressionOpt();
		match(";");

		node = new ReturnStatement(expr);
	} else if (lookahead.is("{")) {
		node = compoundStatement();
	} else {
		node = expressionOpt();
		match(";");
	}

	return node;
}

Statement *Syntax::compoundStatement() {
	Statement *node = NULL;

	match("{");
	node = statementList();
	match("}");

	return node;
}

Statement *Syntax::statementList() {
	Statement *node = NULL;

	if (lookahead.is("if") || 
		lookahead.is("while") || 
		lookahead.is("do") || 
		lookahead.is("for") ||
		lookahead.is("void") || 
		lookahead.is("int") ||
		lookahead.is("{") ||
		lookahead.is("return") ||
		lookahead.is(TKN_OP_ADD) ||
		lookahead.is("(") ||
		lookahead.is(TKN_INTEGER) ||
		lookahead.is(TKN_IDENTIFIER) ||
		lookahead.is(";")) {
	
		node = statement();

		if (node) node->next = statementList();
	}

	return node;
}

Statement *Syntax::elseStatement() {
	Statement *node = NULL;
	if (lookahead.is("else")) {
		match("else");
		node = statement();
	}

	return node;
}

Expression *Syntax::expressionOpt() {
	Expression *expr = NULL;

	if (lookahead.is(TKN_OP_ADD) ||
		lookahead.is("(") ||
		lookahead.is(TKN_INTEGER) ||
		lookahead.is(TKN_IDENTIFIER)) {

		expr = expression();

	}

	return expr;
}

Expression *Syntax::expression() {
	Expression *expr, *aux;

	expr = aux = assignmentExpression();

	while (lookahead.is(",")) {
		match(",");
		aux->next = assignmentExpression();

		aux = (Expression *)aux->next;
	}

	return expr;
}

Expression *Syntax::assignmentExpression() {
	Expression *expr = NULL;

	// A second token is taken
	if (lookahead.is(TKN_IDENTIFIER) && tkn_stream->front()->is("=")) {
		Identifier *id;
		Expression *aux;
		string symbol;

		id = new Identifier(lookahead.symbol);
		match(TKN_IDENTIFIER);
		symbol = lookahead.symbol;
		match("=");
		aux = assignmentExpression();

		expr = new BinaryExpression(symbol, id, aux);
	} else {
		expr = logicalOrExpression();
	}

	return expr;
}

Expression *Syntax::logicalOrExpression() {
	Expression *expr = NULL;
	string symbol;

	expr = logicalAndExpression();

	while (lookahead.is("||")) {
		symbol = lookahead.symbol;
		match("||");
		expr = new BinaryExpression(symbol, expr, logicalAndExpression());
	}

	return expr;
}

Expression *Syntax::logicalAndExpression() {
	Expression *expr = NULL;
	string symbol;

	expr = equalityExpression();

	while (lookahead.is("&&")) {
		symbol = lookahead.symbol;
		match("&&");
		
		expr = new BinaryExpression(symbol, expr, equalityExpression());
	}

	return expr;
}

Expression *Syntax::equalityExpression() {
	Expression *expr = NULL;
	string symbol;

	expr = relationalExpression();

	while (lookahead.is(TKN_OP_EQUALITY)) {
		symbol = lookahead.symbol;
		match(TKN_OP_EQUALITY);

		expr = new BinaryExpression(symbol, expr, relationalExpression());
	}

	return expr;
}

Expression *Syntax::relationalExpression() {
	Expression *expr = NULL;
	string symbol;

	expr = additiveExpression();

	while (lookahead.is(TKN_OP_RELATIONAL)) {
		symbol = lookahead.symbol;
		match(TKN_OP_RELATIONAL);

		expr = new BinaryExpression(symbol, expr, additiveExpression());
	}

	return expr;
}

Expression *Syntax::additiveExpression() {
	Expression *expr = NULL;
	string symbol;

	expr = mutiplicativeExpression();

	while (lookahead.is(TKN_OP_ADD)) {
		symbol = lookahead.symbol;
		match(TKN_OP_ADD);

		expr = new BinaryExpression(symbol, expr, mutiplicativeExpression());
	}

	return expr;
}

Expression *Syntax::mutiplicativeExpression() {
	Expression *expr = NULL;
	string symbol;

	expr = unaryExpression();

	while (lookahead.is(TKN_OP_MULT)) {
		symbol = lookahead.symbol;
		match(TKN_OP_MULT);

		expr = new BinaryExpression(symbol, expr, unaryExpression());
	}

	return expr;
}

Expression *Syntax::unaryExpression() {
	Expression *expr = NULL;

	if (lookahead.is(TKN_OP_ADD)) {
		string op = lookahead.symbol;

		match(TKN_OP_ADD);

		expr = new UnaryExpression(op, unaryExpression());
	} else {
		expr = primaryExpression();
	}

	return expr;
}

Expression *Syntax::primaryExpression() {
	Expression *expr = NULL;

	if (lookahead.is(TKN_IDENTIFIER)) {
		Identifier *id = new Identifier(lookahead.symbol);
		match(TKN_IDENTIFIER);
		expr = functionCall(id);
	} else if (lookahead.is(TKN_INTEGER)) {
		expr = new Integer(lookahead.symbol);
		match(TKN_INTEGER);
	} else {
		match("(");
		expr = expression();
		match(")");
	}

	return expr;
}

Expression *Syntax::functionCall(Identifier *id) {
	Expression *expr = id;

	if (lookahead.is("(")) {
		Expression *args;

		match("(");
		args = argumentList();
		match(")");

		expr = new FunctionCall(id, args);
	}

	return expr;
}

Expression *Syntax::argumentList() {
	Expression *args = NULL, *aux;

	if (lookahead.is(TKN_OP_ADD) ||
		lookahead.is("(") ||
		lookahead.is(TKN_INTEGER) ||
		lookahead.is(TKN_IDENTIFIER)) {

		args = aux = assignmentExpression();

		while (lookahead.is(",")) {
			match(",");
			aux->next = assignmentExpression();

			aux = (Expression *)aux->next;
		}

	}

	return args;
}

void Syntax::match(int tkn_type) {
	if (lookahead.type == tkn_type) {
		nextToken();
	} else {
		error(str_tokens[tkn_type]);
	}
}

void Syntax::match(string tkn_symbol) {
	if (lookahead.symbol == tkn_symbol) {
		nextToken();
	} else {
		error(tkn_symbol);
	}
}

void Syntax::nextToken() {
	if (!tkn_stream->empty()) {
		lookahead = *(tkn_stream->front());

		tkn_stream->pop_front();
	}
}

Node *Syntax::getTree() {
	return tree;
}

void Syntax::treeToXml(const char *outfilename) {
	Node *n = tree;
	ofstream file(outfilename);

	if (file.is_open()) {
		Node::xml.setoutput(&file);
	} else {
		Node::xml.setoutput(&cout);		
	}

	Node::xml.openTag("program");

	while (n != NULL) {
		n->display();
		n = n->next;
	}
	
	Node::xml.closeTag();
	file.close();
}

void Syntax::error(string expected) {
	cout << "[Error de sintaxis] Se esperaba '" << expected << "'" << endl;
	cout << "Se encontro '" << lookahead.symbol << "' (" << str_tokens[lookahead.type] << ")" << endl;
	
	throw 0;	
}