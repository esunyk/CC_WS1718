####################################################################
# This Makefile started out as a copy of the one in the flex manual.
# Cf. http://flex.sourceforge.net/manual/Makefiles-and-Flex.html#Makefiles-and-Flex
#
# It replaces the amazingly complex Makefile that comes with the C++calc example
# found in the Bison manual.
#
# This is Verison 0.2 of the Makefile (as of 17 April 2013, 22:00
# The previous (unnumbered) version only worked with MAKEFLAGS=-j3
# (and it's strange that it worked then).
#
####################################################################
#      "Variables", die hier benutzt werden:
#      Vgl. http://www.makelinux.net/make3/make3-CHP-2-SECT-2.html
# $@ = The filename representing the target.
# $< = The filename of the first prerequisite.
# $* = The stem of the filename of the target (i.e. without .o, .cpp...)
# $^ = The names of all the prerequisites, with spaces between them.
####################################################################
# Uncomment only one of the next two lines (choose your c++ compiler)
# CC=g++
CC=clang++
CFLAGS=-std=c++1y
#c++11 with extensions

LEX=flex
YACC=bison
YFLAGS=-v -d
# Wo   -d wichtig ist, weil damit Header-Dateien erzeugt werden
#         (*.hh - und nicht nur Quellcode in *.cc)
# aber -v nicht so wichtig ist, weil damit "nur" die  Datei
#         calc++-parser.output erzeugt wird, die zwar informativ aber nicht
#         unbedingt notwendig (sie wird nicht weiterverwendet).

HEADERS=go_parser.hh go_scanner.hh AST.h SymbolTableEntry.h

go: go.o go_scanner.o go_parser.o go_driver.o AST.o SymbolTableEntry.o

go.o : go.cc go_driver.hh go_parser.hh AST.h

%.o: %.cc
	$(CC) $(CFLAGS) -o $@ -c $<

go_scanner.cc: go_scanner.ll
	$(LEX) $(LFLAGS) -o go_scanner.cc go_scanner.ll

go_parser.cc go_parser.hh: go_parser.yy
	$(YACC) $(YFLAGS) -o go_parser.cc go_parser.yy

clean:
	$(RM) *~ *.o  go go_scanner.cc go_parser.cc  go_scanner.hh go_parser.hh go_parser.output location.hh stack.hh position.hh

tests: test.sh go
	./test.sh
