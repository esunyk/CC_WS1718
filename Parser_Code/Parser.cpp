#include "Parser.h"
#include "Lexer.h"
#include "Token.h"
#include "ParseException.h"

//TODO: throw exception, error handling instead of error type

std::string Parser::errorLocation;
std::string Parser::errorMessage;

AST* Parser::startParsing(std::vector<std::string> input){
	Lexer::setCode(input);
	AST* s_ast = new AST("S", nullptr); //S is the root and doesn't have a parent
	s_ast->addNode(SourceFile(s_ast));
	Lexer::resetPosition();
	Lexer::resetLinecount();
	return s_ast;
}


void Parser::generateErrorMessage(std::string msg){
	Parser::errorLocation = "ERROR: \nLine: " + std::to_string(Lexer::getLinecount()) + '\n' +
		+"Position: " + std::to_string(Lexer::getPosition()) + '\n';
	Parser::errorMessage = Parser::errorLocation + msg;
}

AST* Parser::SourceFile(AST* parent){
	AST* ast = new AST("SourceFile", parent);
	switch (Lexer::gettok()){
	case tok_package:
		ast->addNode(new AST("Terminal", "package", ast));
		ast->addNode(PackageDeclarationRest(ast));
		break;
	default:
		Parser::generateErrorMessage("Expected package declaration.");
		throw ParseException(Parser::errorMessage);
		break;
	}
	return ast;
}

AST* Parser::PackageDeclarationRest(AST* parent){
	AST* ast = new AST("PackageDeclarationRest", parent);
	SymbolTableEntry entry;
	switch (Lexer::gettok()){
	case tok_main:
		ast->addNode(new AST("Terminal", "main", ast));
		if (Lexer::gettok() == tok_semicolon){
			ast->addNode(new AST("Terminal", ";", ast)); //necessary?
			entry.setFunction(false);
			entry.setDecLine(Lexer::getLinecount());
			entry.setDecPos(Lexer::getPosition() - 4);
			entry.setType("package");
			ast->addSymTabEntry("main", entry);
			ast->addNode(ImportDeclaration(ast));
			ast->addNode(MainBody(ast));
		}
		else{
			Parser::generateErrorMessage("Expected semicolon.");
			throw ParseException(Parser::errorMessage);
		}
		break;
	case tok_pid:
		ast->addNode(PackageIdentifier(Lexer::getIdentifierStr(), ast));
		if (Lexer::gettok() == tok_semicolon){
			ast->addNode(new AST("Terminal", ";", ast)); //necessary?
			ast->addNode(ImportDeclaration(ast));
			ast->addNode(Body(ast));
		}
		else{
			Parser::generateErrorMessage("Expected semicolon.");
			throw ParseException(Parser::errorMessage);
		}
		break;
	default:
		Parser::generateErrorMessage("Invalid package identifer.\nPackage identifiers must start with a letter or an underscore (if the length is bigger than 1) and can only contain letters, numbers and underscores.");
		throw ParseException(Parser::errorMessage);
		break;
	}
	return ast;

}

AST* Parser::ImportDeclaration(AST* parent){ //symbols: defined in imported documents
	AST* ast = new AST("ImportDeclaration", parent);
	Lexer::savePosition();
	switch (Lexer::gettok()){
	case tok_imp:
		ast->addNode(ImportPath(ast));
		if (Lexer::gettok() == tok_semicolon){
			ast->addNode(new AST("Terminal", ";", ast)); //necessary?
			ast->addNode(ImportDeclaration(ast));
		}
		else{
			Parser::generateErrorMessage("Expected semicolon.");
			throw ParseException(Parser::errorMessage);
		}
		break;
	default:
		//TODO: edit grammar to remove backtracking necessity -> move first token to previous production
		Lexer::backtrack();
		delete ast;
		return nullptr;
	}
	return ast;

}

AST* Parser::ImportPath(AST* parent){
	AST* ast = new AST("ImportPath", parent);
	SymbolTableEntry entry(false, "", 0, 0);
	AST* symTabNode;
	std::string key; 
	switch (Lexer::gettok()){
	case tok_string:
		ast->addNode(new AST("Terminal", Lexer::getIdentifierStr(), ast));
		symTabNode = Parser::lookForScopeNode(ast, 0); //import scope is global
		entry.setFunction(false);
		entry.setDecLine(Lexer::getLinecount());
		entry.setDecPos(Lexer::getPosition() - Lexer::getIdentifierStr().length());
		entry.setType("import");
		key = Lexer::getIdentifierStr().substr(1, Lexer::getIdentifierStr().length() - 2); //remove string delimiters
		symTabNode->addSymTabEntry(key, entry);
		//TODO: add symbol table entry, check for existing import -> parse error
		//map.insert returns pair, pair.second is false if key already exists
		break;
	default:
		Parser::generateErrorMessage("Expected import path.");
		throw ParseException(Parser::errorMessage);
		break;
	}
	return ast;
}

AST* Parser::MainBody(AST* parent){
	AST* ast = new AST("MainBody", parent);
	ast->addNode(TopLevelDeclaration(ast));
	ast->addNode(MainFunc(ast));
	ast->addNode(TopLevelDeclaration(ast));
	return ast;

}

AST* Parser::Body(AST* parent){
	AST* ast = new AST("Body", parent);
	ast->addNode(TopLevelDeclaration(ast));
	return ast;


}

AST* Parser::TopLevelDeclaration(AST* parent){
	AST* ast = new AST("TopLevelDeclaration", parent);

	delete ast;
	return nullptr;
	//TODO: non-empty TLD
	//symbol table entry in global table for each new symbol, exception throwing

}

AST* Parser::MainFunc(AST* parent){
	AST* ast = new AST("MainFunc", parent);
	switch (Lexer::gettok()){
	case tok_func:
		ast->addNode(new AST("Terminal", "func", ast));
		if (Lexer::gettok() == tok_main){
			ast->addNode(new AST("Terminal", "main", ast));
			//add main func to global symbol table
			SymbolTableEntry entry(true, "void", Lexer::getLinecount(), Lexer::getPosition() - 4);
			AST* symTabNode = Parser::lookForScopeNode(ast, 0);
			symTabNode->addSymTabEntry("main", entry);
			if (Lexer::gettok() == tok_lparen){
				ast->addNode(new AST("Terminal", "(", ast)); //necessary?
				if (Lexer::gettok() == tok_rparen){
					ast->addNode(new AST("Terminal", ")", ast)); //necessary?
					if (Lexer::gettok() == tok_lcurly){
						ast->addNode(new AST("Terminal", "{", ast)); //necessary?
						ast->addNode(VoidFuncBody(ast));
						if (Lexer::gettok() == tok_rcurly){
							ast->addNode(new AST("Terminal", "}", ast)); //necessary?
						}
						else{
							Parser::generateErrorMessage("Expected }");
							throw ParseException(Parser::errorMessage);
						}
					}
					else{
						Parser::generateErrorMessage("Expected {");
						throw ParseException(Parser::errorMessage);
					}
				}
				else{
					Parser::generateErrorMessage("Expected )");
					throw ParseException(Parser::errorMessage);
				}
			}
			else{
				Parser::generateErrorMessage("Expected (");
				throw ParseException(Parser::errorMessage);
			}
		}
		else{
			Parser::generateErrorMessage("Expected Identifier \"main\"");
			throw ParseException(Parser::errorMessage);
		}
		break;
	default:
		Parser::generateErrorMessage("Expected import or top level declaration.");
		throw ParseException(Parser::errorMessage);
		break;
	}
	return ast;


}

AST* Parser::PackageIdentifier(std::string id, AST* parent){
	AST* ast = new AST("PackageDeclaration", parent);
	ast->addNode(new AST("Terminal", id, ast));
	SymbolTableEntry entry(false, "package", Lexer::getLinecount(), Lexer::getPosition() - id.length());
	//no need to search for scope node, can only be located at one position in the hierarchy
	ast->getParent()->getParent()->addSymTabEntry("id", entry);
	return ast;

}

AST* Parser::VoidFuncBody(AST* parent){
	AST* ast = new AST("VoidFuncBody", parent);

	delete ast;
	return nullptr; //TODO: non-empty body

}

AST* Parser::lookForScopeNode(AST* ast, int type){
	AST* symTabNode = ast->getParent();
	while (symTabNode->getType() != type){
		AST* tmpNode = symTabNode;
		symTabNode = symTabNode->getParent();
	}
	return symTabNode;
}







