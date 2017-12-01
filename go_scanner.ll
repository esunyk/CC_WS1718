/* flex */
%{
	#include <iostream>
	#include "Token.h"
	int yylval = 0;  
%}

digit 	[0-9]
letter 	[a-zA-Z]

%%

";"		    { return tok_semicolon; }
"("         { return tok_lparen; }
")"		    { return tok_rparen; }
"{"		    { return tok_lcurly; }
"}"		    { return tok_rcurly; }
"main"      { return tok_main; }
"package"	{ return tok_package; }
"func"		{ return tok_func; }
"import" 	{ return tok_imp; }
"var"		{ return tok_var; }

{letter}({letter}|{digit})* 	{ return tok_id; }
{digit}+						{ yylval = atoi(yytext); 
								  return tok_number;
								} 
[ \t\n\r]			
<<EOF>> 						{ return tok_eof; }

%%

int yywrap(void) { return 1; }

int main (int argc, char *argv[]) {

	int tok = -1;
	// falls datei existiert, laden | von user lesen
	yyin = ((argc == 2)? fopen(argv[1], "r")
						: stdin);
    yylex();
	while (tok != tok_eof) {
		tok = yylex();
		std::cout << "token: " << tok << " value: " << yylval;
	};
    fclose(yyin);
	return 0;
}


