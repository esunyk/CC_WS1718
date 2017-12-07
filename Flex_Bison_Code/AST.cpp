#include "AST.h" 
#include <iostream>

//TODO (maybe): subclass for leaves, i.e. tokens?
AST::AST(const std::string type, AST* parent) {
	token = static_cast<Type>(strtoint(type));
	value = "";
	this->parent = parent;
}

AST::AST(){}

std::map<std::string, SymbolTableEntry>& AST::getSymbolTable(){
	return symbolTable;
}

void AST::addSymTabEntry(std::string name, SymbolTableEntry sym){
	//std::pair<std::map<char, int>::iterator, bool> inserted;
	//inserted = symbolTable.insert(std::pair<std::string, SymbolTableEntry>(name, sym));
	//if (!inserted.second){
	//	throw ParseException("Error: duplicate declaration for import of " + name);
	//}
	if (symbolTable.find(name) != symbolTable.end()){
		//throw ParseException("ERROR: \nDuplicate declaration for import of " + name + ".");
	}
	else{
		symbolTable.insert(std::pair<std::string, SymbolTableEntry>(name, sym));
	}
}

AST::AST(const std::string type, const std::string val, AST* parent) {
	token = static_cast<Type>(strtoint(type));
	value = val;
	this->parent = parent;
}

AST::~AST() {
	for (AST* ast : nodes){
		//symbol table entries are not created dynamically and don't need to be deleted manually
		delete ast;
	}
}

AST* AST::getParent(){
	return this->parent;
}

std::vector<AST*> AST::getNodes(){
	return this->nodes;
}

void AST::setParent(AST* parent){
	this->parent = parent;
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
	for (AST* ast : nodes){
		if (ast != nullptr)
			(ast)->traverse(output, depth);
	}
}

const void AST::printSymbolTable(std::ostream& output){
	for (auto const& entry : symbolTable){
		output << "Name: " << entry.first << std::endl; //key
		SymbolTableEntry e(entry.second);
		output << "Scope: ";
		output << "global" << std::endl;
		output << "Type: " << e.getType() << std::endl;
		//TODO with bigger grammar: create scope output for each node type the symbol table is stored at
		output << "Is a function: ";
		//output << (e.isFunction() ? "Yes" : "No") << std::endl;
		//output << "Name declared on line " << e.getDecLine();
		//output << " , at position " << e.getDecPos() << std::endl;
		output << std::endl;
	}
	for (AST* ast : nodes){
		if (ast != nullptr)
			(ast)->printSymbolTable(output);
	}
}

Type AST::getType(){
	return this->token;
}

// converts token string value to int value
const int AST::strtoint(const std::string type) {
	//TODO: switch case with string hash instead? -> only if there is time!
	if (type == "s")
		return 0;

	if (type == "source_file")
		return 1;

	if (type == "package_declaration")
		return 2;

	if (type == "import_declaration")
		return 3;

	if (type == "import_path")
		return 4;

	if (type == "toplevel_declaration")
		return 5;

	if (type == "function_name")
		return 6;

	if (type == "function")
		return 7;

	if (type == "signature_rest")
		return 8;

	if (type == "function_body")
		return 9;
	//add more here
	
	if (type == "package_name")
		return 10;

	if (type == "Terminal")
		return 11;
}

// converts token int value to string value
const std::string AST::inttostr(const int r) {
	switch (r) {
	case  0: return "s";
	case  1: return "source_file";
	case  2: return "package_declaration";
	case  3: return "import_declaration";
	case  4: return "import_path";
	case  5: return "toplevel_declaration";
	case  6: return "function_name";
	case  7: return "function";
	case  8: return "signature_rest";
	case  9: return "function_body";
	case 10: return "package_name";
		// more cases to be added
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

