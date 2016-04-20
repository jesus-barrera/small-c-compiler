#ifndef LEXICAL_H
#define LEXICAL_H

#include <fstream>
#include <list>
#include <exception>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>

#define BUFFER_SIZE 4096

using namespace std;

enum {
	TKN_IDENTIFIER, 
	TKN_INTEGER, 
	TKN_FLOAT, 
	TKN_STRING, 
	TKN_OP_ADD, 
	TKN_OP_MULT, 
	TKN_DELIMITER,
	TKN_PARENT_OPEN,
	TKN_PARENT_CLOSE,
	TKN_COMMA,
	TKN_BLOCK_OPEN,
	TKN_BLOCK_CLOSE,
	TKN_OP_AND,
	TKN_OP_OR,
	TKN_OP_ASSIGN,
	TKN_OP_UNARY,
	TKN_OP_EQUALITY,
	TKN_OP_RELATIONAL,
	TKN_EOF,
	TKN_WHITE_SPACE,
	TKN_NOT_VALID, 
	NUM_TOKENS
};

/* States */
enum {
	Q0,
	Q1,
	Q2,
	Q3,
	Q4,
	Q5,
	Q6,
	Q7,
	Q8,
	Q9,
	Q10,
	Q11,
	Q12,
	Q13,
	Q14,
	Q15,
	Q16,
	Q17,
	Q18,
	Q19,
	Q20,
	Q21,
	Q22,
	Q23,
	Q24,
	Q25,
	Q26,
	Q27,
	Q28,
	Q29,
	NUM_STATES,
	K
};

extern const char* str_tokens[];

class Token {
public:
	int type;
	string symbol;

	Token();
	Token(int type, string symbol);

	bool is(int type);
	bool is(string symbol);
};

class Lexical {
private:
	char buffer[BUFFER_SIZE];
	int buff_pos;
	ifstream input;
	list<Token *> tkn_stream;

	void loadBuffer();

	char getChar();
	
	char peekChar();
	
	int getEntry();

	Token *nextToken();

	void error(string tkn_symbol);

public:
	Lexical(string file_name);
	
	~Lexical();

	list<Token *> *scan();

	void flush();
};

#endif