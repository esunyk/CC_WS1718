#include "AST.h"
#include "Lexer.h"
#include "Parser.h"
#include "Main.h"
#include "ParseException.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <sys/stat.h> //to chack for available file names

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
		if (loc != "#" && loc != ""){ //do not save empty strings
			code.push_back(loc);
		}
	} while (loc[0] != '#');
	return code;
}

bool Main::readFile(std::string filename, std::vector<std::string>* code){
	std::ifstream file;
	file.open(filename);
	// do something 
	std::string loc;
	if (file.is_open()) {

		while (!file.eof()) {

			getline(file, loc);
			if (loc != ""){
				code->push_back(loc);
			}
		}
		file.close();
		return true;
	}
	return false;
}

void Main::handleCode(std::vector<std::string> code, std::ostream &output){
	AST* finalTree = nullptr;
	try{
		finalTree = Parser::startParsing(code);
		finalTree->traverse(output, 0);
		output << "Symbol tables: " << std::endl;
		finalTree->printSymbolTable(output);
		delete finalTree;
	}
	catch (ParseException &ex){
		output << ex.what() << std::endl;
		delete finalTree;
		Lexer::resetPosition();
		Lexer::resetLinecount();
	}
}

void Main::userMenu(){
	std::vector<std::string> code;
	std::string filename;
	std::string tmpInput;
	std::string directory;
	std::string tmpFilename;
	std::string outFileName;
	bool validInput;
	bool fileAvailable;
	bool fileNameAvailable;
	std::ofstream outFile;
	std::ifstream testFileName;
	int input;
	int i = 0;
	int j = 0;
	do{
		validInput = false;
		std::cout << "Options: " << std::endl;
		std::cout << "Enter go code manually: 1" << std::endl;
		std::cout << "Read go code from file: 2" << std::endl;
		std::cout << "Run all test files and output results to a text document: 3" << std::endl;
		std::cout << "Terminate Program: 4" << std::endl;

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
			Main::handleCode(code, std::cout);
			break;
		case 2:
			std::cout << "Enter an absolute or relative filepath: " << std::endl;
			getline(std::cin, filename);
			Main::readFile(filename, &code);
			Main::handleCode(code, std::cout);
			break;
		case 3:
			fileAvailable = false;
			std::cout << "Enter an absolute or relative path to the directory";
			std::cout << " where you have stored the \"Correct_Input\" and \"Incorrect_Input\" folders." << std::endl;
			std::cout << "There should also be a \"Test_Output\" folder in the same directory." << std::endl;
			getline(std::cin, directory);
			filename = directory;

			//the commented code fails after once successful iteration, for undetermined reasons, good() returns false even if file exists
			//use the sys/stat.h code below that instead or manage old test log files manually
			/*
			do{
				outFileName = directory + "/Test_Output/Test_Log_";
				outFileName = outFileName + std::to_string(j) + ".txt";
				j++;
				testFileName.open(outFileName);
				fileNameAvailable = !testFileName.good(); //ifstream.good checks if a file exists
				if (fileNameAvailable){
					outFile.open(outFileName);
				}
			} while (!fileNameAvailable);*/

			do{
				outFileName = directory + "/Test_Output/Test_Log_";
				outFileName = outFileName + std::to_string(j) + ".txt";
				j++;
				struct stat buf;
				fileNameAvailable = ((stat(outFileName.c_str(), &buf) != -1) ? false : true);
				if (fileNameAvailable){
					outFile.open(outFileName);
				}
			} while (!fileNameAvailable);

			//correct tests
			filename += "/Correct_Input/Correct_Test_";
			tmpFilename = filename;
			outFile << "Testing correct input:" << std::endl << std::endl;
			do{
				filename = filename + std::to_string(i) + ".txt";
				fileAvailable = Main::readFile(filename, &code);
				if (fileAvailable){
					outFile << "Correct test number " << std::to_string(i) << ":" << std::endl;
					Main::handleCode(code, outFile);
					i++;
					filename = tmpFilename;
					outFile << std::endl;
					code.clear(); //make room for new code
				}
			} while (fileAvailable);

			//incorrect tests
			i = 0;
			filename = directory;
			filename += "/Incorrect_Input/Incorrect_Test_";
			tmpFilename = filename;
			outFile << "Testing incorrect input:" << std::endl << std::endl;
			do{
				filename = filename + std::to_string(i) + ".txt";
				fileAvailable = Main::readFile(filename, &code);
				if (fileAvailable){
					outFile << "Incorrect test number " << std::to_string(i) << ":" << std::endl;
					Main::handleCode(code, outFile);
					i++;
					filename = tmpFilename;
					outFile << std::endl;
					code.clear(); //make room for new code
				}
			} while (fileAvailable);
			outFile.close();
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
	Main::userMenu();
	return 0;
}