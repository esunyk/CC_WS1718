#include <iostream>
#include <fstream>
#include "go_driver.hh"
#include "AST.h"


int
main (int argc, char *argv[])
{
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
	delete finalTree;}
    else
      res = 1;
  return res;
}

