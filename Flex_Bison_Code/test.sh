#! /bin/sh

#This shell script is based on the shell script in the official example and serves as a test framework for our golang lexer/parser

#test -z "$VERBOSE" && {
#   exec > /dev/null 2>&1
#   set -x
#}

#
#CORRECT INPUT
#

echo && echo -e"\nTEST: Running a test program with two imports"
cat >input <<EOF
package main;

import "abc";
import "def";

func main() {
}
EOF

./go input


echo && echo -e"\nTEST: Running the program with scanner (lexer) tracing turned on..."
./go -s input

echo && echo -e"\nTEST: Running the same program with parse tracing turned on..."
./go -p input

echo && echo -e"\nTEST: Running a test program without imports"
cat >input <<EOF
package main;

func main() {
}
EOF
./go input

echo && echo -e"\nTEST: Running a test program with a function that performs addition"
cat >input <<EOF
package main;

func main() {
a := 3 + 5;
}
EOF
./go input

echo && echo -e"\nTEST: Running a test program with a function that performs addition and a function that calls that function"
cat >input <<EOF
package main;

func addTwoNumbers() {
a := 3 + 5;
}

func main() {
addTwoNumbers();
}

EOF
./go input

#
#INCORRECT INPUT
#

echo && echo -e"\nTEST: Running a test program where func and function name were swapped"
cat >input <<EOF
package main;

main func() {
}
EOF
./go input

echo && echo -e"\nTEST: Running a test program where the left paranthesis was left out"
cat >input <<EOF
package main;

func main) {
}
EOF
./go input

echo && echo -e"\nTEST: Running a test program where the right paranthesis was left out"
cat >input <<EOF
package main;

func main( {
}
EOF
./go input

echo && echo -e"\nTEST: Running a test program where the both parantheses were left out"
cat >input <<EOF
package main;

func main {
}
EOF
./go input

echo && echo -e"\nTEST: Running a test program where the semicolon after the package name was left out"
cat >input <<EOF
package main

func main() {
}
EOF
./go input

echo && echo -e"\nTEST: Running a test program with only the package declaration"
cat >input <<EOF
package main;
EOF
./go input

echo && echo -e"\nTEST: Running a test program where the left curly bracket of the function body was left out"
cat >input <<EOF
package main;

func main()
}
EOF
./go input

echo && echo -e"\nTEST: Running a test program where the right curly bracket of the function body was left out"
cat >input <<EOF
package main;

func main(){
EOF
./go input

echo && echo -e"\nTEST: Running a test program where the input is just keymash, i.e. complete nonsense"
cat >input <<EOF
gfgfddfgfdgdgf
EOF
./go input

echo && echo -e"\nTEST: Running a test program with a missing second string delimiter in the import path"
cat >input <<EOF
package main;

import "abc;

func main(){}

EOF
./go input

echo && echo -e"\nTEST: Running a test program with everything after the import keyword missing, i.e. a missing import path"
cat >input <<EOF
package main;

import
EOF
./go input

echo && echo -e"\nTEST: Running a test program with an invalid package name"
cat >input <<EOF
package-

EOF
./go input

echo && echo -e"\nTEST: Running a test program with a duplicate import."
cat >input <<EOF
package main;

import "abc";
import "abc";

func main() {
}
EOF
./go input


rm input
