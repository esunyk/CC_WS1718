#include "lexer.cc"
#include "AST.cc"


static int CurTok, CurBck;
static void saveCursor() {
	CurBck = CurTok;
}
static void RestoreCursor() {
	CurTok = CurBck;
}
static int getNextToken() { 
	return CurTok = gettok(); 
}

/// TODO: LogError-functions
static void LogError(const char *str) {
  fprintf(stderr, "LogError: %s\n", str);
}

/// expression
 

// SourceFile := package [PackageDeclarationRest]
static AST* ParseSourceFile() { 	 
	AST* ast = new AST("SourceFile");
	ast->addNode(new AST("Terminal", "package"));
	
	/*
	if (AST* tmp = ParsePackageDeclarationRest() != NULL) {
		ast->addNode(tmp);
	} else {
		std::cout << "error" << std::endl;
	}
	*/
	
	ast->traverse();
	
	return ast;
}

// PackageDeclarationRest := main ; [ImportDeclaration] [MainBody]
//						   | [PackageIdentifier] ; [ImportDeclaration] [Body]

static AST* ParsePackageDeclarationRest() { 	 
	AST* ast = new AST("PackageDeclarationRest");
	
	saveCursor();
	getNextToken();
	
	if (CurTok == tok_main) {
		ast->addNode(new AST("Terminal", "main"));
		
		getNextToken(); 
		if (CurTok == tok_semicolon) { 
			ast->addNode(new AST("Terminal", ";"));
			/*
			if (AST* astImp = parseImportDeclaration()) {
				ast->addNode(new AST("ImportDeclaration"));
				if (AST* astMainB = parseMainBody()) {
					ast->addNode(new AST("MainBody"));
				}
			}
			*/
		} else {
			// 
		}
		
	} else {
		// error
	}
	/*
	if (AST* tmp = ParsePackageDeclarationRest()) {
		ast->addNode(tmp);
	} else {
		std::cout << "error" << std::endl;
	}
	*/
	ast->traverse();
	
	return ast;
}

// Body := [TopLevelDeclaration]

// TopLevelDeclaration := NULL

// MainFunc := func main ( ) { [VoidFuncBody] }

// PackageIdentifier := [a-zA-Z] | [_a-zA-Z][_a-zA-Z0-9]+

// VoidFuncBody := NULL

// Top-Level parsing

static void HandleSourceFile() {
	/*
	if (ParseSourceFile()) {
		fprintf(stderr, "Parsed a sourcefile\n"); 
	} else {
		// Skip token for error recovery.
		getNextToken();
	}
	*/
}

static void takeInput() {
	unsigned int size = 256;
	std::vector<char*> code;
	char input[size] = "";
	std::cout << "Enter go code: (q to quit)" << std::endl;
	int i = 1;
	
	do {
		std::cout << i++ << " ";
		code.push_back(input);
  		std::cin.getline(input, size);
  	} while (input[0] != 'q');
} 

/// S := [SourceFile]
static void MainLoop() {	
	while (true) {    // Abbruchbedingung?
		  
		switch (CurTok) {
			case tok_eof:
			  return;
			case ';': 
			  getNextToken();
			  break;
			case tok_package:
			  HandleSourceFile();
			  break;
			case tok_main:
		   //   HandleMain();
			  break;
			case tok_func:
			//  HandleFunc();
			  break;
			case tok_var: 
			  // HandleVar();
			  break;
			default: 
			  break;
		}
	}
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
					/*
					while (!file.eof()) { 
					
						file >> output;
						std::cout << output; 
					}
					*/
				}
				
				file.close(); 
			}
			
		} catch (const std::exception&) {
		}
	} else {  
		takeInput();
	} 
 
	return 0;
}
