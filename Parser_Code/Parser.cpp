#include "Parser.h"
#include "Lexer.h"
#include "Token.h"

//TODO: throw exception, error handling instead of error type
//TODO: regression testing -> no framework, automated in main
//TODO: symbol table

AST* Parser::startParsing(std::vector<std::string> input){
	std::string entirecode = "";
	for (std::string loc : input){
		entirecode += loc;
	}
	Lexer::setCode(entirecode);
	AST* s_ast = new AST("S");
	s_ast->addNode(SourceFile());
	return s_ast;
}

AST* Parser::SourceFile(){
	AST* ast = new AST("SourceFile");
	switch (Lexer::gettok()){
	case tok_package:
		ast->addNode(new AST("Terminal", "package"));
		ast->addNode(PackageDeclarationRest());
		break;
	default:
		ast->addNode(new AST("ERROR", "Expected package declaration."));
		break;
	}
	return ast;
}

AST* Parser::PackageDeclarationRest(){
	AST* ast = new AST("PackageDeclarationRest");
	switch (Lexer::gettok()){
	case tok_main:
		ast->addNode(new AST("Terminal", "main"));
		if (Lexer::gettok() == tok_semicolon){
			ast->addNode(new AST("Terminal", ";")); //necessary?
			ast->addNode(ImportDeclaration());
			ast->addNode(MainBody());
		}
		else{
			ast->addNode(new AST("ERROR", "Expected semicolon."));
		}
		break;
		//TODO: include pid
	case tok_pid:
		ast->addNode(PackageIdentifier(Lexer::getIdentifierStr()));
		if (Lexer::gettok() == tok_semicolon){
			ast->addNode(new AST("Terminal", ";")); //necessary?
			ast->addNode(ImportDeclaration());
			ast->addNode(Body());
		}
		else{
			ast->addNode(new AST("ERROR", "Expected semicolon."));
		}
		break;
	default:
		ast->addNode(new AST("ERROR", "Expected package identifier."));
		break;
	}
	return ast;

}

AST* Parser::ImportDeclaration(){
	AST* ast = new AST("ImportDeclaration");
	Lexer::savePosition();
	switch (Lexer::gettok()){
	case tok_imp:
		ImportPath(Lexer::getIdentifierStr());
		if (Lexer::gettok() == tok_semicolon){
			ast->addNode(new AST("Terminal", ";")); //necessary?
			ast->addNode(ImportDeclaration());
		}
		else{
			ast->addNode(new AST("ERROR", "Expected semicolon."));
		}
		break;
	default:
		Lexer::backtrack();
		delete ast;
		return nullptr;
	}
	return ast;

}

AST* Parser::ImportPath(std::string id){
	AST* ast = new AST("ImportPath");

	//TODO: import path, cannot be empty -> error
	delete ast;
	return nullptr;

}

//TODO: change grammar so that MainFunc is a production of TopLevelDeclaration -> FuncDeclaration, change code accordingly
//OR: switch case in other method (TLD -> return nullptr, go to mainfunc)
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
	switch (Lexer::gettok()){
	case tok_func:
		ast->addNode(new AST("Terminal", "func"));
		if (Lexer::gettok() == tok_main){
			ast->addNode(new AST("Terminal", "main"));
			if (Lexer::gettok() == tok_lparen){
				ast->addNode(new AST("Terminal", "(")); //necessary?
				if (Lexer::gettok() == tok_rparen){
					ast->addNode(new AST("Terminal", ")")); //necessary?
					if (Lexer::gettok() == tok_lcurly){
						ast->addNode(new AST("Terminal", "{")); //necessary?
						ast->addNode(VoidFuncBody());
						if (Lexer::gettok() == tok_rcurly){
							ast->addNode(new AST("Terminal", "}")); //necessary?
						}
						else{
							ast->addNode(new AST("ERROR", "Expected }."));
						}
					}
					else{
						ast->addNode(new AST("ERROR", "Expected {."));
					}
				}
				else{
					ast->addNode(new AST("ERROR", "Expected )."));
				}
			}
			else{
				ast->addNode(new AST("ERROR", "Expected (."));
			}
		}
		else{
			ast->addNode(new AST("ERROR", "Expected Identifier \"main\"."));
		}
		break;
	default:
		ast->addNode(new AST("ERROR", "Expected function declaration."));
		break;
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







