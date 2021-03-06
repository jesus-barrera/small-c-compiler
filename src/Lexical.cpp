#include "../include/Lexical.h"

int moves[NUM_STATES][23] = {
/*   a-z  0-9  ' '   \n    .   "   +,-   *   %    /    ;    |    &    =   <,>   !    (    )    {    }    ,   EOF    *  */
	{ Q1,  Q2,  Q8,  Q8,   K, Q25,  Q3,  Q9, Q9,  Q4, Q10, Q18, Q16, Q20, Q23, Q22, Q11, Q13, Q14, Q15, Q12, Q27,   K},
	{ Q1,  Q1,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K,  Q2,   K,   K, Q28,   K,   K,   K,  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K,   K,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K,   K,   K,   K,   K,   K,   K,  Q5,  K,  Q7,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{ Q5,  Q5,  Q5,  Q5,  Q5,  Q5,  Q5,  Q6, Q5,  Q7,  Q5,  Q5,  Q5,  Q5,  Q5,  Q5,  Q5,  Q5,  Q5,  Q5,  Q5,   K,  Q5},
	{ Q5,  Q5,  Q5,  Q5,  Q5,  Q5,  Q5,  Q6, Q5,  Q8,  Q5,  Q5,  Q5,  Q5,  Q5,  Q5,  Q5,  Q5,  Q5,  Q5,  Q5,   K,  Q5},
	{ Q7,  Q7,  Q7,  Q8,  Q7,  Q7,  Q7,  Q7, Q7,  Q7,  Q7,  Q7,  Q7,  Q7,  Q7,  Q7,  Q7,  Q7,  Q7,  Q7,  Q7,  Q8,  Q7},
	{  K,   K,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K,   K,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K,   K,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K,   K,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K,   K,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K,   K,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K,   K,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K,   K,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K,   K,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K, Q17,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K,   K,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K,   K,   K,   K,   K,   K,   K,   K,  K,   K,   K, Q19,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K,   K,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K,   K,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K, Q21,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K,   K,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K,   K,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K, Q21,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K,   K,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K, Q24,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K,   K,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{Q25, Q25, Q25, Q25, Q25, Q26, Q25, Q25,Q25, Q25, Q25, Q25, Q25, Q25, Q25, Q25, Q25, Q25, Q25, Q25, Q25,   K, Q25},
	{  K,   K,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K,   K,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K, Q29,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
	{  K, Q29,   K,   K,   K,   K,   K,   K,  K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K,   K},
};

int actions[NUM_STATES] = {
	TKN_NOT_VALID,
	TKN_IDENTIFIER,
	TKN_INTEGER,
	TKN_OP_ADD,
	TKN_OP_MULT,
	TKN_NOT_VALID,
	TKN_NOT_VALID,
	TKN_NOT_VALID,
	TKN_WHITE_SPACE,
	TKN_OP_MULT,
	TKN_DELIMITER,
	TKN_PARENT_OPEN,
	TKN_COMMA,
	TKN_PARENT_CLOSE,
	TKN_BLOCK_OPEN,
	TKN_BLOCK_CLOSE,
	TKN_NOT_VALID,
	TKN_OP_AND,
	TKN_NOT_VALID,
	TKN_OP_OR,
	TKN_OP_ASSIGN,
	TKN_OP_EQUALITY,
	TKN_OP_UNARY,
	TKN_OP_RELATIONAL,
	TKN_OP_RELATIONAL,
	TKN_NOT_VALID,
	TKN_STRING,
	TKN_EOF,
	TKN_NOT_VALID,
	TKN_FLOAT
};

string reserved_words[] = {
	"auto",
	"break",
	"case",
	"char",
	"const",
	"continue",
	"default",
	"do",
	"double",
	"else",
	"enum",
	"extern", 
	"float",
	"for",
	"goto",
	"if",
	"int",
	"long",
	"register",
	"return",
	"short",
	"signed",
	"sizeof",
	"static",
	"struct",
	"switch",
	"typedef",
	"union",
	"unsigned",
	"void",
	"volatile",
	"while"
};

const char *str_tokens[NUM_TOKENS] = {
	"TKN_IDENTIFIER", 
	"TKN_INTEGER", 
	"TKN_FLOAT", 
	"TKN_STRING", 
	"TKN_OP_ADD", 
	"TKN_OP_MULT", 
	"TKN_DELIMITER",
	"TKN_PARENT_OPEN",
	"TKN_PARENT_CLOSE",
	"TKN_COMMA",
	"TKN_BLOCK_OPEN",
	"TKN_BLOCK_CLOSE",
	"TKN_OP_AND",
	"TKN_OP_OR",
	"TKN_OP_ASSIGN",
	"TKN_OP_UNARY",
	"TKN_OP_EQUALITY",
	"TKN_OP_RELATIONAL",
	"TKN_EOF",
	"TKN_WHITE_SPACE",
	"TKN_KEYWORD",
	"TKN_NOT_VALID"
};

Token::Token(int type, string symbol) {
	this->type = type;
	this->symbol = symbol;
}

Token::Token() {
	this->type = TKN_NOT_VALID;
	this->symbol = "";
}

bool Token::is(int type) {
	return this->type == type;
}

bool Token::is(string symbol) {
	return this->symbol == symbol;
}

void Lexical::loadBuffer() {
	input.read(buffer, BUFFER_SIZE);

	if (input.eof()) {
		buffer[input.gcount()] = EOF;
	}

	buff_pos = 0;
}

char Lexical::getChar() {
	char c;

	c = peekChar();

	if (c != EOF && (++buff_pos == BUFFER_SIZE)) {
		loadBuffer();
	}

	return c;
}

char Lexical::peekChar() {
	return buffer[buff_pos];
}

int Lexical::getInput() {
	char c = peekChar();

	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
		return 0;
	} else if (c >= '0' && c <= '9') {
		return 1;
	} else if (c == ' ' || c == '\t' || c == '\r') {
		return 2;
	} else if (c == '\n') {
		return 3;
	} else if (c == '.') {
		return 4;
	} else if (c == '"') {
		return 5;
	} else if (c == '+' || c == '-') {
		return 6;
	} else if (c == '*' ) {
		return 7;
	} else if (c == '%') {
		return 8;
	} else if (c == '/') {
		return 9;
	} else if (c == ';') {
		return 10;
	} else if (c == '|') {
		return 11;
	} else if (c  == '&') {
		return 12;
	} else if (c == '=') {
		return 13;
	} else if (c == '<' || c == '>') {
		return 14;
	} else if (c == '!') {
		return 15;
	} else if (c == '(') {
		return 16;
	} else if (c == ')') {
		return 17;
	} else if (c == '{') {
		return 18;
	} else if (c == '}') {
		return 19;
	} else if (c == ',') {
		return 20;
	} else if (c == EOF) {
		return 21;
	} else {
		return 22;
	}
}

TokenStream *Lexical::analyze(string filename) {
	char c;

	int state;
	int next_state;
	
	int tkn_type;
	string tkn_symbol;
	
	TokenStream *tokens;
    
    input.open(filename.c_str());

    if (!input.is_open()) {
        error("No se pudo abrir el archivo: \""  + filename + "\"");
    }

    tokens = new TokenStream();
    loadBuffer();

	do {
		do {
			state = Q0;
			tkn_symbol = "";

			next_state = moves[state][getInput()];

			while (next_state != K) {
				state = next_state;
				tkn_symbol.append(1, getChar());
				next_state = moves[state][getInput()];
			}

			tkn_type = actions[state];
		} while (tkn_type == TKN_WHITE_SPACE);

		if (tkn_type == TKN_NOT_VALID) {
			tkn_symbol.append(1, peekChar());
			error("Simbolo no reconocido \"" + tkn_symbol + "\"");
		} else {
			if (tkn_type == TKN_IDENTIFIER && isReservedWord(tkn_symbol)) {
				tkn_type = TKN_KEYWORD;
			}

			tokens->push_back(new Token(tkn_type, tkn_symbol));
		}
	} while (tokens->back()->type != TKN_EOF);

    input.close();
    input.clear();

	return tokens;
}

void Lexical::error(string msg) {
	cout << "[Error Lexico] " << msg << endl;
	throw 0;
}

bool Lexical::isReservedWord(string &identifier) {
	for (int i = 0; i < 32; i++) {
		if (identifier == reserved_words[i]) {
			return true;
		}
	}

	return false;
}

void Lexical::destroyTokens(TokenStream *tokens) {
    if (tokens) {
        while (!tokens->empty()) {
            delete(tokens->front());

            tokens->pop_front();
        }

        delete(tokens);
    }
}

void Lexical::displayTokens(TokenStream *tokens) {
    TokenStream::iterator i;

    for (i = tokens->begin(); i != tokens->end(); ++i) {
        cout << "<" << str_tokens[(*i)->type] << " " << (*i)->symbol << ">" << endl;
    }
}