#include <iostream>
#include <fstream>
#include <string> 
#include <stdlib.h>
#include <regex>
#include "stdio.h"
#include "Token.h"
#include "Lexer.h"

//TODO: regex for identifiers
//TODO: include recursion from grammar
//TODO: make these static class members
static std::string IdentifierStr; 	// Filled in if tok_identifier
static std::string code;			//single line of code
static int position = 0;			//position within the line
static int savedPosition;
static std::vector<std::string> codeVec;

//TODO: enum for regex?
static std::regex imp_id ("\"[[:alpha:]]+\"");
static std::regex short_pid("[[:alpha:]]");
static std::regex long_pid("[_[:alpha:]][[:alnum:]_]+");

void Lexer::setCode(std::string input) {
	code = input;
}

void Lexer::savePosition(){
	savedPosition = position;
}

void Lexer::backtrack(){
	position = savedPosition;
}

std::string Lexer::getIdentifierStr(){
	return IdentifierStr;
}

int Lexer::gettok() {
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



