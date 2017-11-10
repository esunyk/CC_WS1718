#include <iostream>
#include <fstream>
#include <string> 
#include <stdio.h>
#include <stdlib.h>

// contains 
enum Token {
  tok_eof = -1,

  // primary
  tok_id = -2,
  tok_number = -3,		// nötig?

  // commands
  tok_pid = -4,			// unterschied id?
  tok_lparen = -5,		// (
  tok_rparen = -6,		// )
  tok_lcurly = -7,		// {
  tok_rcurly = -8,		// }
  tok_semicolon = -9,	// ;
  tok_main = -10,		// main
  tok_package = -11,	// package
  tok_func = -12,		// func
  tok_imp = -13,		// import
  tok_var = -14,		// var
  tok_lcomment = -15
};

static std::string IdentifierStr; // Filled in if tok_identifier
static double NumVal;             // Filled in if tok_number

static int gettok() {
  static int LastChar = ' ';

	while (isspace(LastChar)) {
		LastChar = getchar();
	}

	if (isalpha(LastChar) || LastChar == "_") { 
		IdentifierStr = LastChar;

		while (isalnum((LastChar = getchar()))) {
			IdentifierStr += LastChar;

			// if (!IdentifierStr.startWith("_")

			if (IdentifierStr == "main")
				return tok_main;

			if (IdentifierStr == "package")
				return tok_pckg;

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
	if (isdigit(LastChar) || LastChar == '.') { 	// TODO 0.20.19 ?
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

int main() {
	return 0;
}

