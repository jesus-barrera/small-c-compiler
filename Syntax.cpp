#include "Syntax.h"
 
Syntax::Syntax(list<Token *> *tkn_stream) {
	this->tkn_stream = tkn_stream;
}

void Syntax::analyze() {
	nextToken();
	
	externalDeclaration();

	match(TKN_EOF);
}

void Syntax::externalDeclaration() {
	if (lookahead.is("void") || lookahead.is("int")) {
		typeEspecifier();
		match(TKN_IDENTIFIER);
		declaration();
		externalDeclaration();
	}
}

void Syntax::declaration() {
	if (lookahead.is("=")) {
		match("=");
		assignmentExpression();
		declaratorList();
		match(";");
	} else if (lookahead.is("(")) {
		match("("); 
		parameterList(); 
		match(")");
		functionDefinition();
	} else {
		declaratorList();
		match(";");
	}
}

void Syntax::declaratorList() {
	if (lookahead.is(",")) {
		match(",");
		match(TKN_IDENTIFIER);
		declarator();
		declaratorList();
	}
}

void Syntax::declarator() {
	if (lookahead.is("=")) {
		match("=");
		assignmentExpression();
	} else if (lookahead.is("(")) {
		match("(");
		parameterList();
		match(")");	
	}
}

void Syntax::functionDefinition() {
	if (lookahead.is("{")) {
		compoundStatement();
	} else {
		declaratorList();
		match(";");
	}
}

void Syntax::typeEspecifier() {
	if (lookahead.is("int")) {
		match("int");
	} else {
		match("void");
	}
}

void Syntax::parameterList() {
	if (lookahead.is("void") || lookahead.is("int")) {
		typeEspecifier();
		match(TKN_IDENTIFIER);

		while (lookahead.is(",")) {
			match(",");

			typeEspecifier();
			match(TKN_IDENTIFIER);
		}
	}
}

void Syntax::statement() {
	if (lookahead.is("if")) {
		match("if");
		match("(");
		expression();
		match(")");
		statement();
		elseStatement();
	} else if (lookahead.is("while")) {
		match("while");
		match("(");
		expression();
		match(")");
		statement();
	} else if (lookahead.is("do")) {
		match("do");
		statement();
		match("while");
		match("(");
		expression();
		match(")");
		match(";");
	} else if (lookahead.is("for")) {
		match("for");
		match("(");
		expressionOpt();
		match(";");
		expressionOpt();
		match(";");
		expressionOpt();
		match(")");
		statement();
	} else if (lookahead.is("void") || lookahead.is("int")) {
		typeEspecifier();
		match(TKN_IDENTIFIER);
		declarator();
		declaratorList();
		match(";");
	} else if (lookahead.is("return")) {
		match("return");
		expressionOpt();
		match(";");
	} else if (lookahead.is("{")) {
		compoundStatement();
	} else {
		expressionOpt();
		match(";");
	}
}

void Syntax::compoundStatement() {
	match("{");
	statementList();
	match("}");
}

void Syntax::statementList() {
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
		lookahead.is(TKN_IDENTIFIER)) {
	
		statement();
		statementList();
	}
}

void Syntax::elseStatement() {
	if (lookahead.is("else")) {
		match("else");
		statement();
	}
}

void Syntax::expressionOpt() {
	if (lookahead.is(TKN_OP_ADD) ||
		lookahead.is("(") ||
		lookahead.is(TKN_INTEGER) ||
		lookahead.is(TKN_IDENTIFIER)) {

		expression();
	}
}

void Syntax::expression() {
	assignmentExpression();

	while (lookahead.is(",")) {
		match(",");
		assignmentExpression();
	}
}

void Syntax::assignmentExpression() {
	// A second token is taken
	if (lookahead.is(TKN_IDENTIFIER) && tkn_stream->front()->is("=")) {
		match(TKN_IDENTIFIER);
		match("=");
		assignmentExpression();
	} else {
		logicalOrExpression();
	}
}

void Syntax::logicalOrExpression() {
	logicalAndExpression();

	while (lookahead.is("||")) {
		match("||");
		logicalAndExpression();
	}
}

void Syntax::logicalAndExpression() {
	equalityExpression();

	while (lookahead.is("&&")) {
		match("&&");
		equalityExpression();
	}
}

void Syntax::equalityExpression() {
	relationalExpression();

	while (lookahead.is(TKN_OP_EQUALITY)) {
		match(TKN_OP_EQUALITY);
		relationalExpression();
	}
}

void Syntax::relationalExpression() {
	additiveExpression();

	while (lookahead.is(TKN_OP_RELATIONAL)) {
		match(TKN_OP_RELATIONAL);
		additiveExpression();
	}
}

void Syntax::additiveExpression() {
	mutiplicativeExpression();

	while (lookahead.is(TKN_OP_ADD)) {
		match(TKN_OP_ADD);
		mutiplicativeExpression();
	}
}

void Syntax::mutiplicativeExpression() {
	unaryExpression();

	while (lookahead.is(TKN_OP_MULT)) {
		match(TKN_OP_MULT);
		unaryExpression();
	}
}

void Syntax::unaryExpression() {
	if (lookahead.is(TKN_OP_ADD)) {
		match(TKN_OP_ADD);
		unaryExpression();
	} else {
		primaryExpression();
	}
}

void Syntax::primaryExpression() {
	if (lookahead.is(TKN_IDENTIFIER)) {
		match(TKN_IDENTIFIER);
		functionCall();
	} else if (lookahead.is(TKN_INTEGER)) {
		match(TKN_INTEGER);
	} else {
		match("(");
		expression();
		match(")");
	}
}

void Syntax::functionCall() {
	if (lookahead.is("(")) {
		match("(");
		arguementList();
		match(")");
	}
}

void Syntax::arguementList() {
	if (lookahead.is(TKN_OP_ADD) ||
		lookahead.is("(") ||
		lookahead.is(TKN_INTEGER) ||
		lookahead.is(TKN_IDENTIFIER)) {

		assignmentExpression();

		while (lookahead.is(",")) {
			match(",");
			assignmentExpression();
		}

	}
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

void Syntax::error(string expected) {
	cout << "[Error de sintaxis] Se esperaba '" << expected << "'" << endl;
	cout << "Se encontro '" << lookahead.symbol << "' (" << str_tokens[lookahead.type] << ")" << endl;
	
	throw 0;	
}