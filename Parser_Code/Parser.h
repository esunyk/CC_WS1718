#ifndef PARSER_H
#define PARSER_H

#include "AST.h"

class Parser{

public:
	static AST* startParsing(std::vector<std::string>);
	static std::string errorLocation;
	static std::string errorMessage;

private:
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

	static AST* addErrorNode(std::string);
};

#endif