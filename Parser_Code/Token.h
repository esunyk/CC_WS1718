#ifndef TOKEN_H
#define TOKEN_H

enum Token {
	tok_eof = -1,

	// primary
	tok_id = -2,
	tok_number = -3,		// not needed yet

	// commands
	tok_pid = -4,			// difference to tok_id: cannot be _
	tok_lparen = -5,		// (
	tok_rparen = -6,		// )
	tok_lcurly = -7,		// {
	tok_rcurly = -8,		// }
	tok_semicolon = -9,	// ;
	tok_main = -10,		// main
	tok_package = -11,	// package
	tok_func = -12,		// func
	tok_imp = -13,		// import
	tok_var = -14,		// var
	tok_lcomment = -15,  // linecomment
	tok_string = -16     //proto-string (only alnum)
};

#endif
