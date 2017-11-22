#include <iostream>
#include <fstream>
#include <string> 
#include <stdlib.h>
#include <regex>
#include "stdio.h"
#include "Token.h"
#include "Lexer.h"
#include "ParseException.h"

//TODO: include recursion from grammar
static std::string IdentifierStr; 	// Filled in if tok_identifier
static std::string code;			//single line of code
static int position = 0;			//position within the line
static int linecount = 1;			//which line is currently being processed
static int savedPosition = 0;
static std::vector<std::string> codeVec;

//TODO: enum for regex?
static std::regex letter("[[:alpha:]]"); //same as isalpha, but works with strings
static std::regex alnum_string ("\"[[:alnum:]]+\"");
static std::regex long_pid("[_[:alpha:]][[:alnum:]_]+");

void Lexer::setCode(std::vector<std::string> input) {
	codeVec = input;
}

void Lexer::resetLinecount(){
	linecount = 1;
}
void Lexer::resetPosition(){
	position = 0;
}

void Lexer::resetSavedPosition(){
	savedPosition = 0;
}
int Lexer::getLinecount(){
	return linecount;
}

int Lexer::getPosition(){
	return position;
}
void Lexer::savePosition(){
	//TODO: save line? -> play through possible scenario
	savedPosition = position;
}

void Lexer::backtrack(){
	//TODO: restore line? -> play through possible scenario
	position = savedPosition;
}

std::string Lexer::getIdentifierStr(){
	return IdentifierStr;
}

int Lexer::gettok() {
	code = codeVec.at(linecount-1);
	if (position == code.length() && linecount == codeVec.size()){
		return tok_eof;
	}
	else if(position >= code.length() || code == ""){
		linecount++;
		code = codeVec.at(linecount - 1);
		position = 0;
		savedPosition = 0;
	}
	static char LastChar = ' ';
	LastChar = code[position];
	while (isspace(LastChar)) {
		// read from input
		LastChar = code[++position];
	}
	if (LastChar == '"'){
		IdentifierStr = LastChar;
		while ((LastChar = code[++position]) != '"'){
			IdentifierStr += LastChar;
			if (position == code.length()){
				std::string msg = "Error: unterminated String in line " + linecount;
				throw ParseException(msg);
			}
		}
		IdentifierStr += code[position++];
		if (std::regex_match(IdentifierStr, alnum_string)){
			return tok_string;
		}
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
		if (std::regex_match(IdentifierStr, long_pid) || std::regex_match(IdentifierStr, letter)){
			return tok_pid; //explicitly make sure that tok_pid is also accepted where tok_id is since {tok_pid} = {tok_id} \"_"
		}

		//_ is the empty identifier, can be written to but never read from
		return tok_id;

	}
	std::string errormsg;

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
		tokCode = tok_eof;
		break;
	default:
		errormsg = "Error: undefined character " + LastChar;
		errormsg += " in line " + linecount;
		errormsg += " at position " + position;
		throw ParseException(errormsg);
		break;

	}
	return tokCode;
}



