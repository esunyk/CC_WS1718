#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

enum Token {
	SourceFile, PackageDeclarationRest, ImportDeclaration, MainBody, Body, TopLevelDeclaration, MainFunc, PackageIdentifier, VoidFuncBody, ImportPath, Terminal
};

class AST {
	public:
		AST(const std::string type, const std::string v);
		virtual ~AST();
		void addNode(AST* node);
		const void traverse();
	private: 
		const int strtoint(const std::string type);
		const std::string inttostr(const int type);
		Token token;
		std::string value;
		std::vector<AST*> nodes;
};

#endif
