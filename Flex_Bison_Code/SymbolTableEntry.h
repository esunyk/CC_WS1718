#pragma once
#include <string>
class SymbolTableEntry
{
public:

	//golang doesn't allow nested function declarations, so all named functions must be global -> TopLevelDeclarations
	//lambda functions are allowed, but obviously not relevant as a symbol table entry
	
	SymbolTableEntry(); //needed for switch cases
	SymbolTableEntry(std::string, int, int);
	SymbolTableEntry(std::string); //no loc, no type
	SymbolTableEntry(int, int); //location only
	SymbolTableEntry(std::string, int, int, SymbolTableEntry*);
	~SymbolTableEntry();
	
	int getDecLine();
	int getDecPos();
	std::string getType();

	void setDecLine(int);
	void setDecPos(int);
	void setType(std::string);
private:
	std::string type = "none";
	int decLine = 1;
	int decPos = 0;
	SymbolTableEntry* typeDecl = nullptr; //points to entry of used type, for new types
};

