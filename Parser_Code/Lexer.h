#ifndef LEXER_H
#define LEXER_H

class Lexer{
public:
	static void savePosition();		 //for backtracking purposes
	static void backtrack();
	static int gettok();
	static std::string getIdentifierStr();
	static void setCode(std::string);

private:

};

#endif // !LEXER_H
