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

";"		    {return yy::go_parser::make_SEMICOLON(loc); }
"("         {return yy::go_parser::make_LPAREN(loc); }
")"		    {return yy::go_parser::make_RPAREN(loc); }
"{"		    {return yy::go_parser::make_LCURLY(loc);}
"}"		    {return yy::go_parser::make_RCURLY(loc); }
"package"	{return yy::go_parser::make_PACKAGE(loc); }
"func"		{return yy::go_parser::make_FUNC(loc); }
"import" 	{return yy::go_parser::make_IMPORT(loc);}

{letter}								{return yy::go_parser::make_LETTER(yytext, loc);}
({letter})({letter}|{digit}|"_")* 		{return yy::go_parser::make_PID(yytext, loc);}
({letter}|"_")({letter}|{digit}|"_")* 	{return yy::go_parser::make_ID(yytext, loc); }
{digit}+								{long n = (int) strtol (yytext, NULL, 10);
										  errno = 0;
										  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
											{driver.error (loc, "integer is out of range");}
										return yy::go_parser::make_INT(n, loc);}
{ws}+									{loc.step();}
[\n]+      								{loc.lines(yyleng); loc.step();}
{string}								{return yy::go_parser::make_STRING(yytext, loc);}
.          								{driver.error (loc, "invalid character");}
<<EOF>> 								{return yy::go_parser::make_END(loc); }
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

