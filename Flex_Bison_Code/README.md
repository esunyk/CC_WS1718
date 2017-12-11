# Flex Lexer And Bison Parser

This is a C++ Bison Parser which uses a C Flex lexer and generates an Abstract Syntax Tree.

## How To Run

#### Linux
To use the Makefile as-is you will need Clang, simply switch to the directory you saved the files in and compile with the make command.
You can then run the program with __./go__. For other compilers, you will need to edit the Makefile accordingly, i.e. change the compiler command
and the C++11 flag to the one accepted by the command.

__Parameters__
-p (optional): enable parse tracing for input
-s (optional): enable scanner tracing for input
The last parameter (required) should be either a file path or __-m__. In the first case the file is used as input,
in the second case user input is taken interactively and written to a temporary file, which is then used as input.

## Directory Contents

Contains all required Source Files, as well as a shell script with test cases and a Makefile.

#### test .sh

The shell script, which includes all tests from the previous exercise. The first correct test is run three times - once without any options, then with 
scanner tracing and then with parse tracing.

You can run this script with __make tests__.