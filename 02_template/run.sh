#!/bin/sh

#----------------------------------------------------------------------------
# DESCRIPTION		
# DATE				[:VIM_EVAL:]strftime('%Y-%m-%d')[:END_EVAL:]
# AUTHOR			ss401533@gmail.com                                           
#----------------------------------------------------------------------------

set -o errexit

GPP=/opt/homebrew/Cellar/gcc/15.1.0/bin/g++-15
BOOST_DIR_INCLUDE=/opt/homebrew/Cellar/boost/1.88.0/include
test -d $BOOST_DIR_INCLUDE
# BOOST_DIR_LIB=/Volumes/numerous/2022/usr/local/homebrew/Cellar/boost/1.81.0_1/lib/
BOOST_DIR_LIB=/opt/homebrew/Cellar/boost/1.88.0/lib

test -d $BOOST_DIR_LIB
cat <<EOF > debug.auto.gdb
# b main
b detail::status
# run
run
# list
# next
# next
# step
# step
# print f
bt
up
list
quit
EOF

cat <<EOF | batcat --style=plain --paging=never --language sh --theme TwoDark
Manpage:
	-g                      Generate source-level debug information
	-v                      Show commands to run and use verbose output

gdb
---
https://cets.seas.upenn.edu/answers/gcc.html
lldb --batch --command=test.gdb --args ./test.exe 5
lldb --batch --command=test.gdb --args helloworld
lldb -ex "b main; r" helloworld

lldb --one-line "b main" helloworld



lldb helloworld

Notes:
	-v show full compile flags (useful for troubleshooting)
	-g include debug symbols for gdb
Debug
-----
lldb --source debug.auto.gdb helloworld

Compile
-------
$GPP  -I$BOOST_DIR_INCLUDE -L $BOOST_DIR_LIB -l boost_filesystem helloworld.cpp -o helloworld -g

EOF
