#include "go_driver.hh"
#include "go_parser.hh"

go_driver::go_driver ()
  : trace_scanning (false), trace_parsing (false)
{
  variables["one"] = 1;
  variables["two"] = 2;
}

go_driver::~go_driver ()
{
}

int
go_driver::parse (const std::string &f)
{
  file = f;
  scan_begin ();
  yy::go_parser parser (*this);
  parser.set_debug_level (trace_parsing);
  int res = parser.parse ();
  scan_end ();
  return res;
}

void
go_driver::error (const yy::location& l, const std::string& m)
{
  std::cerr << l << ": " << m << std::endl;
}

void
go_driver::error (const std::string& m)
{
  std::cerr << m << std::endl;
}

bool
go_driver::addSymTabEntry(std::string name, SymbolTableEntry sym, std::map<std::string, SymbolTableEntry>& symbolTable){
	if (symbolTable.find(name) != symbolTable.end()){
		return false;
	}
	else{
		symbolTable.insert(std::pair<std::string, SymbolTableEntry>(name, sym));
		return true;
	}
}

void 
go_driver::printSymTab(std::ostream& output, std::map<std::string, SymbolTableEntry>& symbolTable){
		for (auto const& entry : symbolTable){
		output << "Name: " << entry.first << std::endl; //key
		SymbolTableEntry e(entry.second);
		//output << "Scope: ";
		//output << "global" << std::endl;
		output << "Type: " << e.getType() << std::endl;
		//TODO with bigger grammar: create scope output for each node type the symbol table is stored at
		//output << "Is a function: ";
		//output << (e.isFunction() ? "Yes" : "No") << std::endl;
		//output << "Name declared on line " << e.getDecLine();
		//output << " , at position " << e.getDecPos() << std::endl;
		output << std::endl;
	}
	
}
