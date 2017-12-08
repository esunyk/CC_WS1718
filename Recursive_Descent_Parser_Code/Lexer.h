#ifndef LEXER_H
#define LEXER_H

class Lexer{
public:
	static void savePosition();		 //for backtracking purposes
	static void backtrack();
	static int gettok();
	static std::string getIdentifierStr();
	static void setCode(std::vector<std::string>);
	static int getLinecount();
	static int getPosition();
	static void resetLinecount();
	static void resetPosition();
	static void resetSavedPosition();
};

#endif // !LEXER_H
