#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

enum Type {
	S, SourceFile, PackageDeclarationRest, ImportDeclaration, MainBody, Body, 
	TopLevelDeclaration, MainFunc, PackageIdentifier, VoidFuncBody, ImportPath, Terminal,
	ERROR //for error handling, since epsilon productions are permissible and thus nullptr is not an error
};

class AST {
public:
	AST(const std::string type);
	AST(const std::string type, const std::string v);
	virtual ~AST();
	void addNode(AST* node);
	const void traverse();
private:
	const int strtoint(const std::string type);
	const std::string inttostr(const int type);
	Type token;
	std::string value;
	std::vector<AST*> nodes;
};

#endif