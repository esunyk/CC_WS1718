#include "Parser.h"
#include "Lexer.cpp"

AST* Parser::S(){
	AST* s_ast = new AST("S");
	s_ast->addNode(SourceFile());
	return s_ast;
}

AST* Parser::SourceFile(){
	AST* ast = new AST("SourceFile");
	switch (gettok()){
	case tok_package:
		ast->addNode(new AST("Terminal", "package"));
		ast->addNode(PackageDeclarationRest());
	default:
		ast->addNode(new AST("ERROR", "Expected package declaration, found: " + getIdentifierValue()));
	}
	return ast;
}

AST* Parser::PackageDeclarationRest(){
	AST* ast = new AST("PackageDeclarationRest");
	switch (gettok()){
	case tok_main:
		ast->addNode(new AST("Terminal", "main"));
		if (gettok() == tok_semicolon){
			ast->addNode(new AST("Terminal", ";"));
			ast->addNode(ImportDeclaration());
			ast->addNode(MainBody());
			return ast;
		}
		else{
			ast->addNode(new AST("ERROR", "Expected semicolon, found: " + getIdentifierValue()));
		}
	case tok_pid:
		//TODO: lexer header to remove global variables?
		ast->addNode(PackageIdentifier(getIdentifierValue()));
		if (gettok() == tok_semicolon){
			ast->addNode(new AST("Terminal", ";"));
			ast->addNode(ImportDeclaration());
			ast->addNode(Body());
			return ast;
		}
		else{
			ast->addNode(new AST("ERROR", "Expected semicolon, found: " + getIdentifierValue()));
		}
	default:
		ast->addNode(new AST("ERROR", "Expected package identifier, found: " + getIdentifierValue()));
	}
	return ast;

}

AST* Parser::ImportDeclaration(){
	AST* ast = new AST("ImportDeclaration");
	switch (gettok()){
	case tok_imp:
		ImportPath();
		if (gettok() == tok_semicolon){
			ast->addNode(new AST("Terminal", ";"));
			ast->addNode(ImportDeclaration());
		}
		else{ 
			ast->addNode(new AST("ERROR", "Expected semicolon, found: " + getIdentifierValue()));
		}
		return ast;
	default: 
		delete ast;
		return nullptr;
	}

}

AST* Parser::ImportPath(){
	AST* ast = new AST("ImportPath");

	//TODO: import path, cannot be empty -> error
	delete ast;
	return nullptr;

}

//TODO: change grammar so that MainFunc is a production of TopLevelDeclaration -> FuncDeclaration, change code accordingly
AST* Parser::MainBody(){
	AST* ast = new AST("MainBody");
	ast->addNode(TopLevelDeclaration());
	ast->addNode(MainFunc()); //should be called from TLD
	ast->addNode(TopLevelDeclaration());
	return ast;

}

AST* Parser::Body(){
	AST* ast = new AST("Body");
	ast->addNode(TopLevelDeclaration());
	return ast;


}

AST* Parser::TopLevelDeclaration(){
	AST* ast = new AST("TopLevelDeclaration");

	delete ast;
	return nullptr; //TODO: non-empty TLD

}

AST* Parser::MainFunc(){
	AST* ast = new AST("MainFunc");
	switch (gettok()){
	case tok_func:
		ast->addNode(new AST("Terminal", "func"));
		if (gettok() == tok_main){
			ast->addNode(new AST("Terminal", "main"));
			if (gettok() == tok_lparen){
				ast->addNode(new AST("Terminal", "("));
				if (gettok() == tok_rparen){
					ast->addNode(new AST("Terminal", ")"));
					if (gettok() == tok_lcurly){
						ast->addNode(new AST("Terminal", "{"));
						ast->addNode(VoidFuncBody());
						if (gettok() == tok_rcurly){
							ast->addNode(new AST("Terminal", "}"));
						}
						else{
							ast->addNode(new AST("ERROR", "Expected }, found: " + getIdentifierValue()));
						}
					}
					else{
						ast->addNode(new AST("ERROR", "Expected {, found: " + getIdentifierValue()));
					}
				}
				else{
					ast->addNode(new AST("ERROR", "Expected ), found: " + getIdentifierValue()));
				}
			}
			else{
				ast->addNode(new AST("ERROR", "Expected (, found: " + getIdentifierValue()));
			}
		}
		else{
			ast->addNode(new AST("ERROR", "Expected Identifier \"main\", found: " + getIdentifierValue()));
		}
	default:
		ast->addNode(new AST("ERROR", "Expected function declaration, found: " + getIdentifierValue()));
	}
	return ast;
	

}

AST* Parser::PackageIdentifier(std::string id){
	AST* ast = new AST("PackageDeclaration");
	ast->addNode(new AST("Terminal", id));
	return ast;

}

AST* Parser::VoidFuncBody(){
	AST* ast = new AST("VoidFuncBody");

	delete ast;
	return nullptr; //TODO: non-empty body

}







