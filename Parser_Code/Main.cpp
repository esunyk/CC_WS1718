#include "AST.h"
#include "Parser.h"
#include <iostream>
#include <cstring>
#include <fstream>

//TODO: call parser, work through input
//TODO: test with test files

static std::vector<std::string> takeInput() {
	const int size = 256;
	std::vector<std::string> code;
	std::string loc = "";
	char input[size] = "";
	std::cout << "Enter go code: (# to quit)" << std::endl;
	int i = 1;

	do {
		std::cout << i++ << " ";
		getline(std::cin, loc);
		if (loc != "#"){
			code.push_back(loc);
		}
	} while (loc[0] != '#');
	return code;
	/* convert vector to char* ?
	code.push_back(new char('\0'));
	char* c = new char[code.size()];
	std::memcpy(c, code.data(), code.size());

	// delete vector
	code.clear();

	// test
	int id = 0;
	while (c[id] != '\0')
	std::cout << *(c+id++);
	*/

	/* test
	for (unsigned int i = 0; i < code.size(); i++)
	std::cout << code.at(i);
	*/
}

static void readFile(std::string){

}

int main(int argc, char** argv) {

	// check for arguments, if not switch to user menu
	if (argc > 1) {
		// vector of inputs?
		std::vector<char*> input;
		std::cout << "hi";
		try {
			// argv[0]: program name
			for (int i = 0; i < argc; i++) {

				// check format etc
				//	std::cout << argv[i] << std::endl; 

				// add to queue  
				input.push_back(argv[i]);
			}

			// second queue to process each element?

			for (unsigned int i = 1; i < input.size(); i++) {
				std::ifstream file;
				file.open(input.at(i));

				// do something 
				char output[512];
				if (file.is_open()) {

					while (!file.eof()) {

						file >> output;
						std::cout << output;
					}

				}

				file.close();
			}

		}
		catch (const std::exception&) {
		}
	}
	else {
		/*
		int input;
		do{
		std::cout << "Options: " << std::endl;
		std::cout << "Enter go code manually: 1" << std::endl;
		std::cout << "Read go code from file: 2" << std::endl;
		std::cout << "Terminate Program: 3" << std::endl;

		std::cin >> input;

		switch (input){
		case 1:
			break;
		case 2: 
			break;
		case 3: 
			break;
		default: std::cout << "Invalid option." << std::endl;

		}
		} while (input != 3);*/
		std::vector<std::string> code = takeInput();
		AST* finalTree = Parser::startParsing(code);
		finalTree->traverse();
	}

	std::cin.get(); //pause for debugging purposes
	return 0;
}