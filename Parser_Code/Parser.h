#ifndef PARSER_H
#define PARSER_H

#include "AST.h"

class Parser{

public:
	static AST* S();
	static AST* SourceFile();
	static AST* PackageDeclarationRest();
	static AST* ImportDeclaration();
	static AST* ImportPath();
	static AST* MainBody();
	static AST* Body();
	static AST* TopLevelDeclaration();
	static AST* MainFunc();
	static AST* PackageIdentifier(std::string);
	static AST* VoidFuncBody();

private:
	static int next_token; //necessary?
};

#endif