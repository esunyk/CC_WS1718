# Rescursive descent parser

This is a recursive descent parser (with lexer) for a minimal subset of Go, written in C++11.


## Directory Structure

### documentation

Contains a grammar changelog and descriptions of the correct and incorrect test cases.


### go_test_code

Contains Go test files as separate text documents, as well as the output folder for automated tests.
This is the directory you should give the program for automated testing when prompted. 
If you prefer, you can copy the subdirectories into another directory and run the tests there.
If you change the names of either the directories or the files, you will need to adjust the relevant section in the Main.cpp file as well.
You can add your own test cases by adding new test documents for each test and incrementing the number at the end of the file name, the automated testing works with all files that follow the naming convention.

#### Correct_Input

Contains the semantically correct Go test cases.

#### Incorrect_Input

Contains the semantically incorrect Go test cases.

#### Test_Output

The output folder for automated tests. It already contains a test output file for the standard test cases. 
You don't need to  archive the test output files manually to save them from being overwritten, a new output file following the naming convention is put in this folder each time the automated testing is executed.


### grammar

Contains the most recent version of the grammar in .pdf format.

#### tokens

Contains a file in .pdf format containing a table of all tokens for the current grammar.


### source_files

Contains the C++ source files.

__how to run__: 

linux: tested under ubuntu 16.04.05 using g++ version 5.4.0
- open a terminal
- change into directory _cd (directory)_
- enter _g++ -std=c++11 *.cpp -o Main_
- execute using _./Main_

windows: developed and tested in Visual Studio 2013 using MSVC
- create a C++ project of your choice
- add the source files to the project, either the original files themselves by adding them as an existing source, or by copying the source files into the appropriate directory ("Visual Studio 2013\Projects\<project name>\<project name>" for empty projects)