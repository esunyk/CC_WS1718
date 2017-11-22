#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <map>
#include "SymbolTableEntry.h"

enum Type {
	SourceFile, PackageDeclarationRest, ImportDeclaration, MainBody, Body,
	TopLevelDeclaration, MainFunc, PackageIdentifier, VoidFuncBody, ImportPath, S,
	ERROR, //for error handling, since epsilon productions are permissible and thus nullptr is not an error
	Terminal
};

class AST {
public:
	AST(const std::string type, AST* parent);
	AST(const std::string type, const std::string v, AST* parent);
	AST* getParent();
	virtual ~AST();
	void addNode(AST* node);
	const void traverse(std::ostream&, int); //call by reference since copying of streams is not allowed
	std::map<std::string, SymbolTableEntry>& getSymbolTable();
	void addSymTabEntry(std::string, SymbolTableEntry);
	const void printSymbolTable(std::ostream&);
	Type getType();
private:
	const int strtoint(const std::string type);
	const std::string inttostr(const int type);
	Type token;
	std::string value;
	std::vector<AST*> nodes;
	AST* parent;
	std::map<std::string, SymbolTableEntry> symbolTable;
};

#endif