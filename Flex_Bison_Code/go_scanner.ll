/* flex */
/* DECLARATIONS */
%option c++
%top{
	#include <iostream>
	#include <string>
	#include <stdlib.h>     /* atoi */
	#include "go_parser.tab.hh"
	#define YY_DECL yy::parser::symbol_type yylex()
	
	int line = 1;
	int position = 0;
%}

digit 	[0-9]
letter 	[a-zA-Z]
ws		[ \t\n\r]
string	\"[a-zA-Z0-9]+\"

%%

";"		    { ECHO; return yy::parser::make_SEMICOLON(yytext); }
"("         { ECHO; return yy::parser::make_LPAREN(yytext); }
")"		    { ECHO; return yy::parser::make_RPAREN(yytext); }
"{"		    { ECHO; return yy::parser::make_LCURLY(yytext);}
"}"		    { ECHO; return yy::parser::make_RCURLY(yytext); }
"package"	{ ECHO; return yy::parser::make_PACKAGE(yytext); }
"func"		{ ECHO; return yy::parser::make_FUNC(yytext); }
"import" 	{ ECHO; return yy::parser::make_IMPORT(yytext);}

{letter}						{
								ECO; 
								return yy::parser::make_LETTER(yytext);}
({letter})({letter}|{digit}|"_")* {
									ECHO;
									return yy::parser::make_PID(yytext);}
({letter}|"_")({letter}|{digit}|"_")* 	{ 
									ECHO;
									return yy::parser::make_ID(yytext); }
{digit}+						{
								ECHO;
								  return yy::parser::make_NUMBER(atoi(yytext));;
								} 
{ws}								/* do nothing */
{string}						{
								ECHO;
								return yy::parser::make_STRING(yytext);}
<<EOF>> 						{ return yy::parser::make_END(); }

%%



