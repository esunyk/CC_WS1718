/***********************
* Parser for minimal subset of golang, based on Calc++ Bison example
*
 ***********************/

%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.2"
%defines
%define parser_class_name {go_parser}
%define api.token.constructor
%define api.value.type variant //save real data types onto value stack
%define parse.assert

%code requires
{
# include <string>
# include "AST.h"
class go_driver;
}

// The parsing context.
%param { go_driver& driver }

%locations
%initial-action
{
  // Initialize the initial location.
  @$.begin.filename = @$.end.filename = &driver.file;
};

%define parse.trace
%define parse.error verbose

%code
{
# include "go_driver.hh"
}

%define api.token.prefix {TOK_} //avoid naming conflicts
%token
  END  0  "end of file"
  SEMICOLON	";"
  LPAREN	"("
  RPAREN	")"
  LCURLY	"{"
  RCURLY	"}" 
  PACKAGE	"package"
  IMPORT	"import"
  FUNC	 	"func"
;
%token	<std::string>	ID		 	"identifier"  
%token	<std::string>	PID		 	"package_identifier" 
%token	<int>			NUMBER     	"number" 
%token	<std::string>	STRING     	"string" 
%token	<std::string>	LETTER    	"letter"
 
	//types for nonterminals
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

%printer { yyoutput << $$; } <*>;

%%
/* RULES 

*/
/*
start symbol: s
*/
%start s;

s
	: source_file{ driver.result = 1; };
source_file
	: package_declaration ";" import_declaration {};
package_declaration
	: "package" package_name {};
package_name
	: "package_identifier"{}
	| "letter" {};
import_declaration
	: "import" import_path ";" import_declaration {}
	|  toplevel_declaration {} ;  //skip for empty imports
	| "import" import_path ";" {} 
import_path:
	"string" {} ;
toplevel_declaration
	: "func" function_name function{} toplevel_declaration
	| "func" function_name function{};
function_name
	: "package_identifier" {}
	| "identifier"{};
function 
	: signature_rest function_body {};
signature_rest
	: "(" ")" {};
function_body
	: "{" "}" {} ; 
%%

void
yy::go_parser::error (const location_type& l,
                          const std::string& m)
{
  driver.error (l, m);
}
