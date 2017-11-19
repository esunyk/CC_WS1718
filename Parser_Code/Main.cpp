#include "AST.h"
#include <iostream>
#include <cstring>
#include <fstream>

//TODO: call parser, work through input
//TODO: test with test files

static void takeInput() {
	const int size = 256;
	std::vector<std::string> code;
	char input[size] = "";
	std::cout << "Enter go code: (# to quit)" << std::endl;
	int i = 1;

	do {
		std::cout << i++ << " ";
		code.push_back(input);
		std::cin.getline(input, size);
	} while (input[0] != '#');

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

int main(int argc, char** argv) {

	// check for arguments, if not switch to stdin
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
				char output[100];
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
		takeInput();
	}

	std::cin.get(); //pause for debugging purposes
	return 0;
}