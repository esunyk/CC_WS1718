#ifndef PARSER_H
#define PARSER_H

#include "AST.h"

class Parser{

public:
	static AST* startParsing(std::vector<std::string>);

private:
	static AST* SourceFile();
	static AST* PackageDeclarationRest();
	static AST* ImportDeclaration();
	static AST* ImportPath(std::string);
	static AST* MainBody();
	static AST* Body();
	static AST* TopLevelDeclaration();
	static AST* MainFunc();
	static AST* PackageIdentifier(std::string);
	static AST* VoidFuncBody();
};

#endif