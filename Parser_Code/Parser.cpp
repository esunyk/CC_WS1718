#include "Parser.h"
#include "Lexer.h"
#include "Token.h"

//TODO: throw exception, error handling instead of error type
//TODO: symbol table

std::string Parser::errorLocation;
std::string Parser::errorMessage;

AST* Parser::startParsing(std::vector<std::string> input){
	//TODO: separate function to update location, otherwise: line 1, position 0
	Lexer::setCode(input);
	AST* s_ast = new AST("S");
	s_ast->addNode(SourceFile());
	Lexer::resetPosition();
	Lexer::resetLinecount();
	return s_ast;
}

AST* Parser::addErrorNode(std::string message){
	Parser::errorLocation = "\nLine: " + std::to_string(Lexer::getLinecount()) + '\n' +
		+"Position: " + std::to_string(Lexer::getPosition()) + '\n';
	Parser::errorMessage = Parser::errorLocation + message;
	AST* ast = new AST("ERROR", Parser::errorMessage);
	return ast;
}

AST* Parser::SourceFile(){
	AST* ast = new AST("SourceFile");
	switch (Lexer::gettok()){
	case tok_package:
		ast->addNode(new AST("Terminal", "package"));
		ast->addNode(PackageDeclarationRest());
		break;
	default:
		ast->addNode(Parser::addErrorNode("Expected package declaration."));
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
			ast->addNode(Parser::addErrorNode("Expected semicolon."));
		}
		break;
		//TODO: include pid for proper error message
	case tok_pid:
		ast->addNode(PackageIdentifier(Lexer::getIdentifierStr()));
		if (Lexer::gettok() == tok_semicolon){
			ast->addNode(new AST("Terminal", ";")); //necessary?
			ast->addNode(ImportDeclaration());
			ast->addNode(Body());
		}
		else{
			ast->addNode(Parser::addErrorNode("Expected semicolon."));
		}
		break;
	default:
		ast->addNode(Parser::addErrorNode("Expected package identifier."));
		break;
	}
	return ast;

}

AST* Parser::ImportDeclaration(){
	AST* ast = new AST("ImportDeclaration");
	Lexer::savePosition();
	switch (Lexer::gettok()){
	case tok_imp:
		ast->addNode(ImportPath());
		if (Lexer::gettok() == tok_semicolon){
			ast->addNode(new AST("Terminal", ";")); //necessary?
			ast->addNode(ImportDeclaration());
		}
		else{
			ast->addNode(Parser::addErrorNode("Expected semicolon."));
		}
		break;
	default:
		Lexer::backtrack();
		delete ast;
		return nullptr;
	}
	return ast;

}

AST* Parser::ImportPath(){
	AST* ast = new AST("ImportPath");
	switch (Lexer::gettok()){
	case tok_string:
		ast->addNode(new AST("Terminal", Lexer::getIdentifierStr()));
		break;
	default:
		//TODO: error handling
		ast->addNode(Parser::addErrorNode("Expected import path."));
		break;
	}
	return ast;
}

AST* Parser::MainBody(){
	AST* ast = new AST("MainBody");
	ast->addNode(TopLevelDeclaration());
	ast->addNode(MainFunc());
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
							ast->addNode(Parser::addErrorNode("Expected }"));
						}
					}
					else{
						ast->addNode(Parser::addErrorNode("Expected {"));
					}
				}
				else{
					ast->addNode(Parser::addErrorNode("Expected )"));
				}
			}
			else{
				ast->addNode(Parser::addErrorNode("Expected ("));
			}
		}
		else{
			ast->addNode(Parser::addErrorNode("Expected Identifier \"main\""));
		}
		break;
	default:
		ast->addNode(Parser::addErrorNode("Expected function declaration."));
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







