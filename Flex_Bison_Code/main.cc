#include <iostream>
#include "go_parser.tab.hh"

//extern unsigned int codenum;


//int yywrap(void) { return 1; } //done

void yy::parser::error(std::string const&err) { std::cout << "ERROR: " << err << std::endl; } 

int main(int argc, char *argv[]) {
	yy::parser parser;
	return 0;
}
