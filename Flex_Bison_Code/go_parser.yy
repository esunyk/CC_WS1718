/***********************
* Parser for minimal subset of golang, based on Calc++ Bison example
*
 ***********************/

%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.2"
%defines
%define parser_class_name {go_parser}
%define api.token.constructor //only for value type variant
%define api.value.type variant //does work after all, there was probably a logical error or typo last time
%define parse.assert

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

%code //required in target file
{
# include "go_driver.hh"
# include "AST.h"
# include "SymbolTableEntry.h"
}

/*
%union
{
AST *ast;
char *sval;
int ival;
}
*/

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
  PLUS			"+"
  ASSIGN		":="
  
;
	//rest of tokens: different semantic values, but are syntactically equivalent
%token	<std::string>			ID		 	"identifier"  
%token	<std::string>			PID		 	"package_identifier" 
%token	<int>					INT     	"integer" //not used in grammar yet, only token exists
%token	<std::string>			STRING     	"string" 
%token	<std::string>			LETTER    	"letter"
 
	//associativity with precedence implied by order (bottom first)
%left '+' '-'
%left '*' '/'
%right UMINUS

	//types for nonterminals
%type <AST*> source_file;
%type <AST*> package_declaration;
%type <AST*> import_declaration;
%type <AST*> import_path;
%type <AST*> toplevel_declaration;
%type <AST*> function_name;
%type <AST*> package_name;
%type <AST*> function;
%type <AST*> signature_rest;
%type <AST*> function_body;
%type <AST*> line;
%type <AST*> addition;
%type <AST*> func_call;
%type <AST*> assignment;
%type <AST*> left_hand;
%type <AST*> right_hand;

%%
/* RULES 
   no epsilon rules: lead to shift/reduce conflicts
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
												   //try to add entry to proper table, print error and stop parsing if duplicate
												   if(!driver.addSymTabEntry(std::string($1), sym, driver.imports))
												   {
												   std::string errmsg = "ERROR: Duplicate import: " + std::string($1);
												   driver.error(@1, errmsg);
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
												   driver.error(@1, errmsg);
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
												   driver.error(@1, errmsg);
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
	: "(" ")" 									{$$ = new AST("signature_rest", nullptr); //TO DO: param
												   $$->addNode(new AST("Terminal", "(" , nullptr));  
												   $$->addNode(new AST("Terminal", ")" , nullptr));
												   std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }};
function_body
	: "{" line "}" 									{$$ = new AST("function_body", nullptr);
												   $$->addNode(new AST("Terminal", "{" , nullptr));
												   $$->addNode($2);
												   $$->addNode(new AST("Terminal", "}" , nullptr));
												   std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }} ; 
	| "{" "}" 								{$$ = new AST("function_body", nullptr);
												   $$->addNode(new AST("Terminal", "{" , nullptr));
												   $$->addNode(new AST("Terminal", "}" , nullptr));
												   std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }} ; 
												   
line:												//TO DO: print
	assignment										{$$ = new AST("line", nullptr);	
													$$->addNode($1);	
													std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }} ;
	| func_call										{$$ = new AST("line", nullptr);	
													$$->addNode($1);	
													std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }} ;
assignment:
	left_hand ":=" right_hand						{$$ = new AST("assignment", nullptr);	
													$$->addNode($1);
													$$->addNode(new AST("Terminal", ":=" , nullptr));
													$$->addNode($3);
													std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }} ; 
left_hand:										//TO DO: add to symbol table for this scope
	"package_identifier"						{$$ = new AST("left_hand", nullptr);	
												$$->addNode(new AST("Terminal", std::string($1) , nullptr));
												std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }} ; 
												   
	| "identifier"								{$$ = new AST("left_hand", nullptr);
												 $$->addNode(new AST("Terminal", std::string($1) , nullptr));
													std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }} ; 
	| "letter" 									{$$ = new AST("left_hand", nullptr);
												 $$->addNode(new AST("Terminal", std::string($1) , nullptr));
													std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }} ; 
right_hand
	: addition									{$$ = new AST("right_hand", nullptr);
												$$->addNode($1);
												std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }} ; 
	| func_call									{$$ = new AST("right_hand", nullptr);
												$$->addNode($1);
												std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }} ; 
	| "integer"									{$$ = new AST("right_hand", nullptr);
												$$->addNode(new AST("Terminal", std::to_string($1) , nullptr));
												std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }} ; 
addition: 
//TO DO: change integer to to expr (don't forget to edit AST file), add rules
	"integer" "+" "integer"  ";"              {$$ = new AST("addition", nullptr);
												$$->addNode(new AST("Terminal", std::to_string($1) , nullptr));
												$$->addNode(new AST("Terminal", "+" , nullptr));
												$$->addNode(new AST("Terminal", std::to_string($3) , nullptr));
												$$->addNode(new AST("Terminal", ";" , nullptr));
												std::vector<AST*> nodes = $$->getNodes();
												   //set this as parent for each child node
												   for (AST* node: nodes)
												   {
												   node->setParent($$);
												   }} ; 
												   
func_call:
	"package_identifier" "(" ")" ";"           {$$ = new AST("func_call", nullptr); //TO DO: param
												//TO DO: throw error if not in symbol table
											  $$->addNode(new AST("Terminal", std::string($1) , nullptr));
											  $$->addNode(new AST("Terminal", "(", nullptr));
											  $$->addNode(new AST("Terminal", ")" , nullptr));
											  $$->addNode(new AST("Terminal", ";" , nullptr));
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
