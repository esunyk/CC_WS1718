/***********************
 * Lexer for minimal subset of golang, based on the Calc++ Flex example
 *
 ***********************/
%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <string>
# include "go_driver.hh"
# include "go_parser.hh"
# include "AST.h"

// Work around an incompatibility in flex (at least versions
// 2.5.31 through 2.5.33): it generates code that does
// not conform to C89.  See Debian bug 333231
// <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.
# undef yywrap
# define yywrap() 1

// The location of the current token.
static yy::location loc;
%}
%option noyywrap nounput batch debug
digit 	[0-9]
letter 	[a-zA-Z]
ws		[ \t\r]
string	\"[a-zA-Z0-9]+\"

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}

%%

%{
  // Code run each time yylex is called.
  loc.step ();
%}

";"		    {return yy::go_parser::token::TOK_SEMICOLON; }
"("         {return yy::go_parser::token::TOK_LPAREN; }
")"		    {return yy::go_parser::token::TOK_RPAREN; }
"{"		    {return yy::go_parser::token::TOK_LCURLY;}
"}"		    {return yy::go_parser::token::TOK_RCURLY; }
"package"	{return yy::go_parser::token::TOK_PACKAGE; }
"func"		{return yy::go_parser::token::TOK_FUNC; }
"import" 	{return yy::go_parser::token::TOK_IMPORT;}

{letter}								{yylval->sval = strdup(yytext); return yy::go_parser::token::TOK_LETTER;}
({letter})({letter}|{digit}|"_")* 		{yylval->sval = strdup(yytext); return yy::go_parser::token::TOK_PID;}
({letter}|"_")({letter}|{digit}|"_")* 	{yylval->sval = strdup(yytext); return yy::go_parser::token::TOK_ID; }
{digit}+								{long n = (int) strtol (yytext, NULL, 10);
										  errno = 0;
										  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
											{driver.error (loc, "integer is out of range");}
										  else{
											yylval->ival = (int) n;}
										return yy::go_parser::token::TOK_INT;}
{ws}+									{loc.step();}
[\n]+      								{loc.lines(yyleng); loc.step();}
{string}								{yylval->sval = strdup(yytext); return yy::go_parser::token::TOK_STRING;}
.          								{yylval->sval = strdup(yytext); driver.error (loc, "invalid character");}
<<EOF>> 								{yylval->sval = strdup(yytext); return yy::go_parser::token::TOK_END; }
%%

void
go_driver::scan_begin ()
{
  yy_flex_debug = trace_scanning;
  if (file.empty () || file == "-")
    yyin = stdin;
  else if (!(yyin = fopen (file.c_str (), "r")))
    {
      error ("cannot open " + file + ": " + strerror(errno));
      exit (EXIT_FAILURE);
    }
}



void
go_driver::scan_end ()
{
  fclose (yyin);
}

