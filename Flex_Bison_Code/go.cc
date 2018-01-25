#include <iostream>
#include <fstream>
#include "go_driver.hh"
#include "AST.h"

#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Support/IRBuilder.h"

int
main (int argc, char *argv[])
{

	# taken from https://www.ibm.com/developerworks/library/os-createcompilerllvm1/ via moodle
	# Module(StringRef, LLVMContext&): top level container that contains every llvm object
	# StringRef: name of the module, irrelevant
	# LLVMContext: provides context in which elements are created 
	llvm::LLVMContext& context = llvm::getGlobalContext();
	llvm::Module* module = new llvm::Module("cc-ws1718", LLVMContext& context);

	# creates LLVM instructions and inserts them into basic blocks
	llvm::IRBuilder<> builder(context);

	
  int res = 0;
  go_driver driver;
  AST* finalTree = nullptr;
  for (int i = 1; i < argc; ++i)
	if (argv[i] == std::string ("-m"))
      { //user enters code manually, code is saved to file, parsed from file
	std::string code = "";
	std::string loc = "";
	std::cout << "Enter go code: (# in a new line to quit)" << std::endl;
	int i = 1;
	do {
		std::cout << i++ << " ";
		getline(std::cin, loc);
		if (loc != "#" && loc != ""){ //do not save empty strings
			code += loc;
		}
	} while (loc[0] != '#');
	std::ofstream codeFile;
	codeFile.open("tmpFile.txt");
	codeFile << code;
	codeFile.close();
	if(!driver.parse ("tmpFile.txt")){
    std::cout << driver.result << std::endl;
	finalTree = driver.finalTree;
	finalTree->traverse(std::cout, 0);
	std::cout << "package name: " << driver.package << std::endl;
	driver.printSymTab(std::cout, driver.imports);
	//TODO: output global symbol tables
	delete finalTree;}
	}
    else if (argv[i] == std::string ("-p"))
      driver.trace_parsing = true;
    else if (argv[i] == std::string ("-s"))
      driver.trace_scanning = true;
    else if(!driver.parse (argv[i])){
    std::cout << driver.result << std::endl;
	finalTree = driver.finalTree;
	finalTree->traverse(std::cout, 0);
	std::cout << "package name: " << driver.package << std::endl;
	std::cout << "Imports: " << std::endl;
	driver.printSymTab(std::cout, driver.imports);
	std::cout << "Functions: " << std::endl;
	driver.printSymTab(std::cout, driver.functions);
	std::cout << "Global Variables: " << std::endl;
	driver.printSymTab(std::cout, driver.globalVariables);
	//TODO: output global symbol tables
	
	
	llvm::FunctionType *funcType = llvm::FunctionType::get(builder.getVoidTy(), false);
	llvm::Function *mainFunc = 
		llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module);
	llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "s", mainFunc);
	builder.SetInsertPoint(entry);

	# vom root-AST über die Kinder iterieren, 
	# nach Typ prüfen und davon abhängig neues Element erstellen und anhängen
	# vllt lieber rekursiv über die Elemente iterieren?
	AST* current = finalTree->getType();
	while (?) {	
		switch (current) {
			case x: llvm::Value *helloWorld = builder.CreateGlobalStringPtr(current->inttostr());
				break;
			...
		}
		
	}

	# show contents of the module
	module->dump();
	
	
	delete finalTree;}
    else
      res = 1;
  return res;
}

