#include <iostream>
#include <fstream>
#include <string> 
#include "stdio.h"
#include <stdlib.h>
#include "Token.h"

//TODO: regex for identifiers
static std::string IdentifierStr; 	// Filled in if tok_identifier
static double NumVal;             	// Filled in if tok_number

static std::string code;			//single line of code
static int position = 0;			//position within the line
static int savedPosition;

// TODO how to pass code to lexer?
static void read(std::string input) {
	code = input;
}

static void savePosition(){
	savedPosition = position;
}

static void backtrack(){
	position = savedPosition;
}

static std::string getIdentifierValue(){
	return IdentifierStr;
}

static int gettok() {
	if (position == code.length()){
		return tok_eof;
	}
	static char LastChar = ' ';
	LastChar = code[position];
	while (isspace(LastChar)) {
		// read from input
		LastChar = code[++position];
	}

	if (isalpha(LastChar) || LastChar == '_') {
		IdentifierStr = LastChar;

		while (isalnum((LastChar = code[++position]))) {
			IdentifierStr += LastChar;
		}

		if (IdentifierStr == "main"){
			return tok_main;
		}

		if (IdentifierStr == "package"){
				return tok_package;
		}
		if (IdentifierStr == "func"){
			return tok_func;
		}
		if (IdentifierStr == "import"){
				return tok_imp;
		}
		if (IdentifierStr == "var"){
			return tok_var;
		}
			return tok_id;
		
	}

	// Number: [0-9.]+
	if (isdigit(LastChar)) {
		bool isDecimal;
		std::string NumStr;
		do {
			NumStr += LastChar;
			LastChar = code[++position];
		} while (isdigit(LastChar) || LastChar == '.');

		NumVal = strtod(NumStr.c_str(), NULL);

		return tok_number;
	}

	int tokCode;
	switch (LastChar){
	case '(':
		++position;
		tokCode = tok_lparen;
		break;
	case ')':
		++position;
		tokCode = tok_rparen;
		break;
	case '{':
		++position;
		tokCode = tok_lcurly;
		break;
	case '}':
		++position;
		tokCode = tok_rcurly;
		break;
	case ';':
		++position;
		tokCode = tok_semicolon;
		break;
	case EOF:
		//don't eat eof
		tokCode = tok_eof;
		break;
	default:
		//return ascii value of char
		//TODO: ERROR
		int ThisChar = LastChar;
		LastChar = code[++position];
		tokCode = ThisChar;
		break;

	}
	return tokCode;
}



