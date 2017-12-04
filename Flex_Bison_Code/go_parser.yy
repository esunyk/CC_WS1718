/* DECLARATIONS */

%skeleton "lalr1.cc"
%language "c++"
%debug
%error-verbose
%defines
%define api.token.constructor //type safety
%define api.value.type variant //we use genuine types and not union 
%define parse.assert //proper usage
%code{
	#include <iostream>
	#include <string>
	#include "lex.yy.cc"
	#define YY_DECL yy::parser::symbol_type yylex() 
	#define yyterminate() parser::make_END(); //since default returns int and we're using types
	YY_DECL;
%}

//todo: add ast, add to types for nonterminals

//c++ variant with complete type instead of union type declaration recommended by bison
%token        	END      	0 		"end of file" 
%token	<std::string>	ID		 	"identifier"  
%token	<std::string>	PID		 	"package_identifier" 
%token	<std::string>	SEMICOLON	";"
%token	<std::string>	LPAREN		"("
%token	<std::string>	RPAREN		")"
%token	<std::string>	LCURLY		"{"
%token	<std::string>	RCURLY		"}" 
%token	<std::string>	PACKAGE		"package"
%token	<std::string>	IMPORT		"import"
%token	<std::string>	FUNC	 	"func"
%token	<int>			NUMBER     	"number" 
%token	<std::string>	STRING     	"string" 
%token	<std::string>	LETTER    	"letter" 

%type <std::string> s;
%type <std::string> source_file;
%type <std::string> package_declaration;
%type <std::string> import_declaration;
%type <std::string> import_path;
%type <std::string> toplevel_declaration;
%type <std::string> function_name;
%type <std::string> function;
%type <std::string> signature_rest;
%type <std::string> function_body;

%%
/* RULES */
/*
start symbol: s
*/
%start s;

s
	: source_file;
source_file
	: package_declaration SEMICOLON import_declaration toplevel_declaration;
package_declaration
	: PACKAGE package_name;
package_name
	: PID
	| LETTER; 
import_declaration
	: import_declaration IMPORT import_path SEMICOLON
	| IMPORT import_path SEMICOLON;
import_path:
	STRING;
toplevel_declaration
	: toplevel_declaration FUNC function_name function
	| FUNC function_name function;
function_name
	:ID
	| PID;
function 
	: signature_rest function_body;
signature_rest
	: LPAREN RPAREN;
function_body
	: LCURLY RCURLY; 

%%
/* PROGRAM */
