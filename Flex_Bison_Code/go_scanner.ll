/* flex */
%{
	#include <iostream>
	#include "Token.h"
	int yylval = 0;  
%}

digit 	[0-9]
letter 	[a-zA-Z]

%%

";"		    { return SEMICOLON; }
"("         { return LPAREN; }
")"		    { return RPAREN; }
"{"		    { return LCURLY; }
"}"		    { return RCURLY; }
"package"	{ return PACKAGE; }
"func"		{ return FUNC; }
"import" 	{ return IMPORT; }
"var"		{ return VAR; }

{letter}({letter}|{digit})* 	{ return ID; }
{digit}+						{ yylval = atoi(yytext); 
								  return NUMBER;
								} 
[ \t\n\r]			
<<EOF>> 						{ return EOF; }

%%

int yywrap(void) { return 1; }

int main (int argc, char *argv[]) {

	int tok = -1;
	// falls datei existiert, laden | von user lesen
	yyin = ((argc == 2)? fopen(argv[1], "r")
						: stdin);
    yylex();
	while (tok != EOF) {
		tok = yylex();
		std::cout << "token: " << tok << " value: " << yylval;
	};
    fclose(yyin);
	return 0;
}


