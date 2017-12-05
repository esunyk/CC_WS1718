#! /bin/sh

# This shell script is part of the C++ Bison Example
# (tests the calc++ program).
# Some changes made by Prof. R. C. Moore (fbi.h-da.de)

# See https://www.gnu.org/software/bison/manual/html_node/A-Complete-C_002b_002b-Example.html

# Copyright (C) 2005-2012 Free Software Foundation, Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

#test -z "$VERBOSE" && {
#   exec > /dev/null 2>&1
#   set -x
#}

#
#CORRECT INPUT
#

echo && echo -e "\nTEST: Running a test program with two imports"
cat >input <<EOF
package main;

import "abc";
import "def";

func main() {
}
EOF

./go input

echo && echo -e "\nTEST: Running the same four line cac++ program with input from a pipe (stdin)..."
cat input | ./go -

echo && echo -e "\nTEST: Running the same four line cac++ program with parse tracing turned on..."
./go -p input

echo && echo -e "\nTEST: Running a test program without imports"
cat >input <<EOF
package main;

func main() {
}
EOF

echo && echo -e "\nTEST: Running the same four line cac++ program with parse tracing turned on..."
./go -p input



#
#INCORRECT INPUT
#

echo && echo -e "\nTEST: Running a test program where func and function name were swapped"
cat >input <<EOF
package main;

main func() {
}
EOF
./go input

echo && echo -e "\nTEST: Running a test program where the left paranthesis was left out"
cat >input <<EOF
package main;

func main) {
}
EOF
./go input

echo && echo -e "\nTEST: Running a test program where the right paranthesis was left out"
cat >input <<EOF
package main;

func main( {
}
EOF
./go input

echo && echo -e "\nTEST: Running a test program where the both parantheses were left out"
cat >input <<EOF
package main;

func main {
}
EOF
./go input

echo && echo -e "\nTEST: Running a test program where the semicolon after the package name was left out"
cat >input <<EOF
package main

func main() {
}
EOF
./go input

echo && echo -e "\nTEST: Running a test program with only the package declaration"
cat >input <<EOF
package main;
EOF
./go input

echo && echo -e "\nTEST: Running a test program where the left curly bracket of the function body was left out"
cat >input <<EOF
package main;

func main()
}
EOF
./go input

echo && echo -e "\nTEST: Running a test program where the right curly bracket of the function body was left out"
cat >input <<EOF
package main;

func main(){
EOF
./go input

echo && echo -e "\nTEST: Running a test program where the input is just keymash, i.e. complete nonsense"
cat >input <<EOF
gfgfddfgfdgdgf
EOF
./go input

echo && echo -e "\nTEST: Running a test program with a missing second string delimiter in the import path"
cat >input <<EOF
package main;

import "abc;

func main(){}

EOF
./go input

echo && echo -e "\nTEST: Running a test program with a missing second string delimiter in the import path"
cat >input <<EOF
package main;

import "abc;

func main(){}

EOF
./go input

echo && echo -e "\nTEST: Running a test program with everything after the import keyword missing, i.e. a missing import path"
cat >input <<EOF
package main;

import
EOF
./go input

echo && echo -e "\nTEST: Running a test program with an invalid package name"
cat >input <<EOF
package-

EOF
./go input

echo && echo -e "\nTEST: Running a test program with an invalid package name"
cat >input <<EOF
package-

EOF
./go input


echo && echo -e "\nTEST: Running a cac++ program with scanner (lexer) tracing turned on..."
cat >input <<EOF
EOF
./go -s input


rm input
