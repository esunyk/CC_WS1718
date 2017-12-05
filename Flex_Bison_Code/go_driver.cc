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