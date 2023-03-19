#!/bin/sh

#----------------------------------------------------------------------------
# DESCRIPTION		
# DATE				[:VIM_EVAL:]strftime('%Y-%m-%d')[:END_EVAL:]
# AUTHOR			ss401533@gmail.com                                           
#----------------------------------------------------------------------------

set -o errexit

GPP=/Volumes/numerous/usr/local/homebrew/Cellar/gcc/11.3.0_2/bin/g++-11
BOOST_DIR_INCLUDE=/Volumes/numerous/2022/usr/local/homebrew/Cellar/boost/1.81.0_1/include/
BOOST_DIR_LIB=/Volumes/numerous/2022/usr/local/homebrew/Cellar/boost/1.81.0_1/lib/

cat <<EOF > debug.auto.gdb
b main
run
list
next
next
step
quit
EOF

cat <<EOF | batcat --plain --paging=never --language sh --theme TwoDark
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
