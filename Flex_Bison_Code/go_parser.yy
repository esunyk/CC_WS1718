/***********************
* Parser for minimal subset of golang, based on Calc++ Bison example
*
 ***********************/

%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.2"
%defines
%define parser_class_name {go_parser}
//%define api.token.constructor //only for value type variant
//%define api.value.type union //automatic union struct inferred from types
//%define parse.assert

%code requires //required in this file
{
# include <string>
# include <vector>
# include "AST.h"
class go_driver;
}

// The parsing context.
%param { go_driver& driver } //equivalent to both old calls

%locations
%initial-action
{
  // Initialize the initial location.
  @$.begin.filename = @$.end.filename = &driver.file;
};

%define parse.trace
%define parse.error verbose //verbose error messages

%code //required in target file -> pasted to top
{
# include "go_driver.hh"
# include "AST.h"
# include "SymbolTableEntry.h"
}

%union
{
AST *ast;
char *sval;
}

%define api.token.prefix {TOK_} //avoid naming conflicts
%token //tokens that can only have one semantic value
  END  0  "end of file"
  SEMICOLON 	";"
  LPAREN		"("
  RPAREN		")"
  LCURLY		"{"
  RCURLY		"}" 
  PACKAGE		"package"
  IMPORT		"import"
  FUNC	 		"func"
;
	//rest of tokens: different semantic values, but are syntactically equivalent
%token	<sval>			ID		 	"identifier"  
%token	<sval>			PID		 	"package_identifier" 
%token	<sval>			NUMBER     	"number" 
%token	<sval>			STRING     	"string" 
%token	<sval>			LETTER    	"letter"
 
	//types for nonterminals
%type <ast> source_file;
%type <ast> package_declaration;
%type <ast> import_declaration;
%type <ast> import_path;
%type <ast> toplevel_declaration;
%type <ast> function_name;
%type <ast> package_name;
%type <ast> function;
%type <ast> signature_rest;
%type <ast> function_body;

	//TODO?: add toString in AST
	//%printer { yyoutput << $$; } <*>;
%%
/* RULES 

*/
/*
start symbol: s
*/
%start s;

s
	: source_file{ 	driver.finalTree = new AST("s", nullptr);
					driver.finalTree->addNode($1);
					driver.result = 1;};
source_file
	: package_declaration ";" import_declaration { $$ = new AST("source_file", driver.finalTree);
												   $$->addNode($1);  $$->addNode(new AST("Terminal", ";" , nullptr));  $$->addNode($3);
												   std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }
												   };
package_declaration
	: "package" package_name 					{$$ = new AST("package_declaration", nullptr);
												   $$->addNode(new AST("Terminal", "package" , nullptr));  $$->addNode($2);
												   std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }};
package_name
	: "package_identifier"						{$$ = new AST("package_name", nullptr);
												   driver.package = std::string($1);
												   $$->addNode(new AST("Terminal", std::string($1) , nullptr));
												   std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }}
	| "letter" 									{$$ = new AST("package_name", nullptr);
												   driver.package = std::string($1);
												   $$->addNode(new AST("Terminal", std::string($1) , nullptr));
												   std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }};
import_declaration
	: "import" import_path ";" import_declaration {$$ = new AST("import_declaration", nullptr);
												   $$->addNode(new AST("Terminal", "import" , nullptr)); $$->addNode($2); 
												   $$->addNode(new AST("Terminal", ";" , nullptr)); $$->addNode($4);
												   std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }}
	| "import" import_path ";" 					{$$ = new AST("import_declaration", nullptr);
												   $$->addNode(new AST("Terminal", "import" , nullptr));
												   $$->addNode($2); $$->addNode(new AST("Terminal", ";" , nullptr));
												   std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }} 
	|  toplevel_declaration 					{$$ = new AST("import_declaration", nullptr);
												   $$->addNode($1);
												   std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }} ;  //when done with imports
import_path:
	"string" 								{$$ = new AST("import_path", nullptr);
												   SymbolTableEntry sym;
												   if(!driver.addSymTabEntry(std::string($1), sym, driver.imports))
												   {
												   std::string errmsg = "ERROR: Duplicate import: " + std::string($1);
												   driver.error(errmsg);
												   YYABORT;
												   }
												   $$->addNode(new AST("Terminal", std::string($1) , nullptr));
												   std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }};
toplevel_declaration
	: "func" function_name function toplevel_declaration {$$ = new AST("toplevel_declaration", nullptr);
												   $$->addNode(new AST("Terminal", "func" , nullptr));
												   $$->addNode($2); $$->addNode($3); $$->addNode($4);
												   std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }}
	| "func" function_name function				{$$ = new AST("toplevel_declaration", nullptr);
												   $$->addNode(new AST("Terminal", "func" , nullptr));
												   $$->addNode($2);  $$->addNode($3);
												   std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }};
function_name
	: "package_identifier" 						{$$ = new AST("function_name", nullptr);
												   SymbolTableEntry sym("void");
												   if(!driver.addSymTabEntry(std::string($1), sym, driver.functions))
												   {
												   std::string errmsg = "ERROR: Duplicate declaration of function " + std::string($1);
												   driver.error(errmsg);
												   YYABORT;
												   }
												    $$->addNode(new AST("Terminal", std::string($1) , nullptr));
												   std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }}
	| "identifier"								{$$ = new AST("function_name", nullptr);
												   SymbolTableEntry sym("void");
												   if(!driver.addSymTabEntry(std::string($1), sym, driver.functions))
												   {
												   std::string errmsg = "ERROR: Duplicate declaration of function " + std::string($1);
												   driver.error(errmsg);
												   YYABORT;
												   }
												   $$->addNode(new AST("Terminal", std::string($1) , nullptr));
												   std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }};
function 
	: signature_rest function_body 				{$$ = new AST("function", nullptr);
												   $$->addNode($1);  $$->addNode($2);
												   std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }};
signature_rest
	: "(" ")" 									{$$ = new AST("signature_rest", nullptr);
												   $$->addNode(new AST("Terminal", "(" , nullptr));  
												   $$->addNode(new AST("Terminal", ")" , nullptr));
												   std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }};
function_body
	: "{" "}" 									{$$ = new AST("function_body", nullptr);
												   $$->addNode(new AST("Terminal", "{" , nullptr));
												   $$->addNode(new AST("Terminal", "}" , nullptr));
												   std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }} ; 
%%

void
yy::go_parser::error (const location_type& l,
                          const std::string& m)
{
  driver.error (l, m);
}
