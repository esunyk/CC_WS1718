#pragma once
#include <string>
class SymbolTableEntry
{
public:

	//golang doesn't allow nested function declarations, so all named functions must be global -> TopLevelDeclarations
	//lambda functions are allowed
	//->scope is only relevant for variables
	//every function and block has its own scope with the corresponding table located at the first corresponding node
	//additionally, there is a global scope, with the corresponding table located at 
	//idea for variables(not implemented in grammar yet) -> start with full declaration node, iterate upwards through parent nodes until scope node is found (by type)
	// -> add to scope node's symbol table
	//same process for top level declarations: start with tld node, iterate upwards through parents until source file node is reached
	//shadowing is allowed (local variables can shadow imports, function arguments, ...) -> remember for later implementation
	//the package declaration is located at the PackageDeclarationRest node

	SymbolTableEntry(); //needed for switch cases
	SymbolTableEntry(bool, std::string, int, int);
	SymbolTableEntry(bool, std::string, int, int, SymbolTableEntry*);
	~SymbolTableEntry();

	bool isFunction();
	int getDecLine();
	int getDecPos();
	std::string getType();

	void setFunction(bool);
	void setDecLine(int);
	void setDecPos(int);
	void setType(std::string);
private:
	bool function;
	std::string type;
	int decLine;
	int decPos;
	SymbolTableEntry* typeDecl = nullptr; //declaration of type, for complex types
};

