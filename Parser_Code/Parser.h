#ifndef PARSER_H
#define PARSER_H

#include "AST.h"
#include "SymbolTableEntry.h"

class Parser{

public:
	static AST* startParsing(std::vector<std::string>);
	static std::string errorLocation;
	static std::string errorMessage;

private:
	static AST* SourceFile(AST* parent);
	static AST* PackageDeclarationRest(AST* parent);
	static AST* ImportDeclaration(AST* parent);
	static AST* ImportPath(AST* parent);
	static AST* MainBody(AST* parent);
	static AST* Body(AST* parent);
	static AST* TopLevelDeclaration(AST* parent);
	static AST* MainFunc(AST* parent);
	static AST* PackageIdentifier(std::string, AST* parent);
	static AST* VoidFuncBody(AST* parent);

	static void generateErrorMessage(std::string);
	static AST* lookForScopeNode(AST*, int);
};

#endif