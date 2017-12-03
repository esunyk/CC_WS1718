%skeleton "lalr1.cc" /* -*- C++ bison -*- */
%require "2.6.90.8-d4fe"
%defines
%define parser_class_name {"go_parser"}

%code requires {
#include <string>
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

// %define api.token.prefix {TOK_}
// same as lexer
%token        	END      	0 		"end of file" 
%token	<std::string>	ID		 	"identifier"  
%token	<std::string>	PID		 	"package_identifier" 
%token	<std::string>	SEMICOLON		";"
%token	<std::string>	LPAREN			"("
%token	<std::string>	RPAREN			")"
%token	<std::string>	LCURLY			"{"
%token	<std::string>	RCURLY			"}" 
%token	<std::string>	PACKAGE			 "package"
%token	<std::string>	IMPORT		 		"import"
%token	<std::string>	FUNC	 		"func"
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
	: source_file  { driver.result = 1; };
source_file
	: package_declaration SEMICOLON import_declaration toplevel_declaration {};
package_declaration
	: PACKAGE PID {};
import_declaration
	: import_declaration "import" import_path ";"
	| %empty;
import_path:
	STRING;
toplevel_declaration
	: toplevel_declaration “func” function_name function
	| %empty;
function_name
	:ID;
function 
	: signature_rest function_body;
signature_rest
	: LPAREN RPAREN;
function_body
	: LCURLY RCURLY; 

%%

void
yy::go_parser::error(const yy::go_parser::location_type& l, const std::string& m) {
  driver.error (l, m);
}
