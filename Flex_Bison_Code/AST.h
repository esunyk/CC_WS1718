#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <map>
#include "SymbolTableEntry.h"

enum Type {
s, source_file, package_declaration, import_declaration, import_path, toplevel_declaration, function_name, function, signature_rest, function_body, package_name, Terminal
};

class AST {
public:
	AST();
	AST(const std::string type, AST* parent);
	AST(const std::string type, const std::string v, AST* parent);
	void setParent(AST*);
	AST* getParent();
	virtual ~AST();
	void addNode(AST* node);
	std::vector<AST*> getNodes();
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