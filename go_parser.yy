%skeleton "lalr1.cc" /* -*- C++ bison -*- */
%require "2.6.90.8-d4fe"
%defines
%define parser_class_name {"go_parser"}

%code requires {
#include <string>
#include "Token.h"
class go_driver;
}

// The parsing context.
%parse-param { go_driver& driver }
%lex-param   { go_driver& driver }

%locations
%initial-action
{
  // Initialize the initial location.
  @$.begin.filename = @$.end.filename = &driver.file;
};

%debug
%error-verbose

// Symbols.
// 
// Compare Bison Manual
// Section 3.8.4 The Collection of Value Types
// https://www.gnu.org/software/bison/manual/html_node/Union-Decl.html
%union
{ 
  std::string* sval;
};

%code {
# include "go_driver.hh" 
SymbolTable* symbolTable;
}

%define api.token.prefix {TOK_}
// same as lexer
%token        	END      	0 		"end of file" 
%token	<std::string>	id		 	"identifier"  
%token	<std::string>	tok_semicolon		";"
%token	<std::string>	tok_lparen		"("
%token	<std::string>	tok_rparen		")"
%token	<std::string>	tok_lcurly		"{"
%token	<std::string>	tok_rcurly		"}" 
%token	<std::string>	tok_package "package"
%token	<std::string>	tok_main 		"main"
%token	<std::string>	tok_imp 		"import"
%token	<std::string>	tok_func 		"func"
%token	<int>	NUMBER     			"number" 
;  
%type 	<sval>	importpath   
%type 	<sval>	packageidentifier   

// Printer Macro is used for tracing the parser,
// Compare Bison Manual
// Section 3.8.8 Printing Semantic Values
%printer    { yyoutput << *$$; } "identifier"

// Destructor Macro is used with error recovery
// Compare Bison Manual
// Section 3.8.7 Freeing Discarded Symbols
%destructor { delete $$; } "identifier"

%printer    { yyoutput << $$; } <sval>


%%
/*
	startet bei s
	TODO: eventuell für jede regel: { $$ = createAST(token, $1...n); } erstellen?
*/
%start s;

s
	: sourcefile  { driver.result = 1; };
sourcefile
	:  "package" packagedeclarationrest {};

packagedeclarationrest
	: "main" ";" importdeclaration mainbody
    | packageidentifier ";" importdeclaration body;

importdeclaration
	: "import" importpath ";" importdeclaration
	| %empty;

importpath
	: id { $$ = $1;}; 

mainbody
	: topleveldeclaration mainfunc topleveldeclaration;

body
	: topleveldeclaration;

topleveldeclaration
	: %empty;

mainfunc
	: "func" "main" "(" ")" "{" voidfuncbody "}";

voidfuncbody
	: %empty;

packageidentifier
	: id { $$ = $1;}; 

%%

void
yy::go_parser::error(const yy::go_parser::location_type& l, const std::string& m) {
  driver.error (l, m);
}
