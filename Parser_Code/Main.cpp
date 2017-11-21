#include "AST.h"
#include "Parser.h"
#include "Main.h"
#include <iostream>
#include <cstring>
#include <fstream>

//TODO: test with test files -> works as intended, now:
//TODO: automated testing
std::vector<std::string> Main::takeInput() {
	std::vector<std::string> code;
	std::string loc = "";
	std::cout << "Enter go code: (# in a new line to quit)" << std::endl;
	int i = 1;
	do {
		std::cout << i++ << " ";
		getline(std::cin, loc);
		if (loc != "#"){
			code.push_back(loc);
		}
	} while (loc[0] != '#');
	return code;
}

std::vector<std::string> Main::readFile(std::string filename){
	std::ifstream file;
	file.open(filename);
	// do something 
	std::string loc;
	std::vector<std::string> code;
	if (file.is_open()) {

		while (!file.eof()) {

			getline(file, loc);
			code.push_back(loc);
		}

	}
	file.close();
	return code;
}

void Main::handleCode(std::vector<std::string> code){
	AST* finalTree = Parser::startParsing(code);
	finalTree->traverse();
	delete finalTree;
}

void Main::userMenu(){
	std::vector<std::string> code;
	std::string filename;
	int input;
	std::string tmpInput;
	bool validInput;
	do{
		validInput = false;
		std::cout << "Options: " << std::endl;
		std::cout << "Enter go code manually: 1" << std::endl;
		std::cout << "Read go code from file: 2" << std::endl;
		std::cout << "Run all test files and output results to a text document: 3" << std::endl;
		std::cout << "Terminate Program: 3" << std::endl;

		while (!validInput){
		getline(std::cin, tmpInput);
		try{
			input = std::stoi(tmpInput);
			validInput = true;
		}
		catch (std::invalid_argument){ //conversion failed
			std::cout << "Please enter a number." << std::endl;
		}
		catch (std::out_of_range){ //number to big, error from underlying function
			std::cout << "The number you entered is too big, please enter another number." << std::endl;
		}
		}

		switch (input){
		case 1:
			code = Main::takeInput();
			Main::handleCode(code);
			break;
		case 2:
			std::cout << "Enter file name: " << std::endl;
			getline(std::cin, filename);
			code = Main::readFile(filename);
			Main::handleCode(code);
			break;
		case 3:
			//TODO: automated testing:
			//go to folders, increment file ending number, run while file is openable
			//create output file -> date, output
			//manually archive old output files
			break;
		case 4:
			break;
		default:
			std::cout << "Invalid option." << std::endl;
			break;
		}
	} while (input != 4);
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
			for (unsigned int i = 1; i < input.size(); i++) {
				std::string filename(input.at(i));
				std::vector<std::string> code = Main::readFile(filename);
				Main::handleCode(code);

			}

		}
		catch (const std::exception&) {
		}
	}
	else {
		Main::userMenu();
	}

	std::cin.get(); //pause for debugging purposes
	return 0;
}