#include "AST.h" 
#include <iostream>

//TODO (maybe): subclass for leaves?
//TODO: deal with nullptr in vector -> remove?
AST::AST(const std::string type) {
	token = static_cast<Type>(strtoint(type));
	value = "";
}

AST::AST(const std::string type, const std::string val) {
	token = static_cast<Type>(strtoint(type));
	value = val;
}

AST::~AST() {
	for (std::vector<AST*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
		delete *it;
}

void AST::addNode(AST* node) {
	nodes.push_back(node);
}

const void AST::traverse(std::ostream& output, int depth) {
	for (int i = 0; i <= depth; i++){
		output << "    ";
	}
	depth++;
	output << inttostr(token);
	if (value != ""){
		output << " : " << value;

	}
	else{ output << "->"; }
	output << std::endl;
	for (std::vector<AST*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		// TODO: structure output format
		//output << "\t";
		if (*it != nullptr)
			(*it)->traverse(output, depth);
	}
}


// converts token string value to int value
const int AST::strtoint(const std::string type) {
	//TODO switch case with string hash instead
	if (type == "SourceFile")
		return 0;

	if (type == "PackageDeclarationRest")
		return 1;

	if (type == "ImportDeclaration")
		return 2;

	if (type == "MainBody")
		return 3;

	if (type == "Body")
		return 4;

	if (type == "TopLevelDeclaration")
		return 5;

	if (type == "MainFunc")
		return 6;

	if (type == "PackageIdentifier")
		return 7;

	if (type == "VoidFuncBody")
		return 8;

	if (type == "ImportPath")
		return 9;

	if (type == "S")
		return 10;

	if (type == "ERROR")
		return 11;

	//add more here

	if (type == "Terminal")
		return 12;
}

// converts token int value to string value
const std::string AST::inttostr(const int r) {
	switch (r) {
	case  0: return "SourceFile";
	case  1: return "PackageDeclarationRest";
	case  2: return "ImportDeclaration";
	case  3: return "MainBody";
	case  4: return "Body";
	case  5: return "TopLevelDeclaration";
	case  6: return "MainFunc";
	case  7: return "PackageIdentifier";
	case  8: return "VoidFuncBody";
	case  9: return "ImportPath";
		// more cases to be added
	case 10: return "S";
	case 11: return "ERROR";
	default: return "Terminal";
	}
}

/*int main() {

	// sample tests
	AST* ast1 = new AST("SourceFile");
	ast1->addNode(new AST("PackageDeclarationRest"));
	ast1->addNode(new AST("TopLevelDeclaration"));
	AST* ast2 = new AST("VoidFuncBody");
	ast1->addNode(ast2);
	ast2->addNode(new AST("Terminal", "hello"));
	ast1->traverse();

	delete ast1;

	std::cout << std::endl;
	std::cin.get(); //pause for debugging purposes
	return 0;
	}*/

