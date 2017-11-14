#include <iostream>
#include <fstream>
#include <string> 
#include "stdio.h"
#include <stdlib.h>
#include "Token.h"

static std::string IdentifierStr; 	// Filled in if tok_identifier
static double NumVal;             	// Filled in if tok_number
static std::string code;			// TODO type of input?

// TODO how to pass code to lexer?
static void read(std::string input) {
	code = input;
}

static int gettok() {
	static int LastChar = ' ';

	while (isspace(LastChar)) {
		// read from input
		LastChar = getchar();
	}

	if (isalpha(LastChar) || LastChar == '_') { 
		IdentifierStr = LastChar;

		while (isalnum((LastChar = getchar()))) {
			IdentifierStr += LastChar;

			// if (!IdentifierStr.startWith("_")

			if (IdentifierStr == "main")
				return tok_main;

			if (IdentifierStr == "package")
				return tok_package;

			if (IdentifierStr == "func")
				return tok_func;
				
			if (IdentifierStr == "import")
				return tok_imp;

			if (IdentifierStr == "var")
				return tok_var;
				
			return tok_id;
		}
	}

	// Number: [0-9.]+
	if (isdigit(LastChar)) { 
		bool isDecimal;
		std::string NumStr; 
		do {
			NumStr += LastChar;
			LastChar = getchar();
		} while (isdigit(LastChar) || LastChar == '.');

		NumVal = strtod(NumStr.c_str(), NULL);

		return tok_number;
	}

	if (LastChar == '(') 
		return tok_lparen;
			
	if (LastChar == ')') 
		return tok_rparen;
		
	if (LastChar == '{') 
		return tok_lcurly;
	
	if (LastChar == '}') 
		return tok_rcurly;
	
	if (LastChar == ';') 
		return tok_semicolon;
	
	/* Comment until end of line.
	if (LastChar == '/') { 	
		do {
			LastChar = getchar();
		} while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

		if (LastChar != EOF)
			return gettok();
	}
	*/

	// Check for end of file.  Don't eat the EOF.
	if (LastChar == EOF)
		return tok_eof;

	// Otherwise, just return the character as its ascii value.
	int ThisChar = LastChar;
	LastChar = getchar();

	return ThisChar;
}



