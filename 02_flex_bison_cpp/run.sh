#!/bin/sh

#----------------------------------------------------------------------------
# DESCRIPTION			
# DATE				2025
# AUTHOR			ss401533@gmail.com
#----------------------------------------------------------------------------
# template found at ~/.vim/sh_header.temp

set -o errexit
echo `date +%s::`"$0" >> ~/db.git/command_history.txt >> ~/db.git/command_history_timestamped.txt

cat <<EOF | batcat --style=plain --paging=never --language sh --theme TwoDark
Thanks to https://github.com/ezaquarii/bison-flex-cpp-example

call graph
----------
	libsystem_kernel.dylib!__pthread_kill (Unknown Source:0)
	libsystem_pthread.dylib!pthread_kill (Unknown Source:0)
	libsystem_c.dylib!abort (Unknown Source:0)
	libsystem_c.dylib!__assert_rtn (Unknown Source:0)
	mycompiler!yylex(EzAquarii::Scanner&) (/Volumes/git/github/bison-flex-cpp-example/parser.y:38)
	mycompiler!EzAquarii::Parser::parse() (/Volumes/git/github/bison-flex-cpp-example/parser.cpp:493)
	mycompiler!main (/Volumes/git/github/bison-flex-cpp-example/main.cpp:16)
	start (Unknown Source:0)
	
	libsystem_kernel.dylib!__read_nocancel (Unknown Source:0)
	libsystem_c.dylib!__sread (Unknown Source:0)
	libsystem_c.dylib!_sread (Unknown Source:0)
	libsystem_c.dylib!__srefill1 (Unknown Source:0)
	libsystem_c.dylib!__srget (Unknown Source:0)
	libsystem_c.dylib!getc (Unknown Source:0)
	libc++.1.dylib!std::__1::__stdinbuf<char>::__getchar(bool) (Unknown Source:0)
	libc++.1.dylib!std::__1::basic_istream<char, std::__1::char_traits<char>>::get() (Unknown Source:0)
	mycompiler!std::__1::basic_istream<char, std::__1::char_traits<char>>::get[abi:ue170006](char&) (/Library/Developer/CommandLineTools/SDKs/MacOSX14.5.sdk/usr/include/c++/v1/istream:258)
	mycompiler!EzAquarii_FlexLexer::LexerInput(char*, unsigned long) (/Volumes/git/github/bison-flex-cpp-example/scanner.cpp:936)
	mycompiler!EzAquarii_FlexLexer::yy_get_next_buffer() (/Volumes/git/github/bison-flex-cpp-example/scanner.cpp:1059)
	mycompiler!EzAquarii::Scanner::get_next_token() (/Volumes/git/github/bison-flex-cpp-example/scanner.cpp:779)
	mycompiler!yylex(EzAquarii::Scanner&) (/Volumes/git/github/bison-flex-cpp-example/parser.y:40)
	mycompiler!EzAquarii::Parser::parse() (/Volumes/git/github/bison-flex-cpp-example/parser.cpp:494)
	mycompiler!main (/Volumes/git/github/bison-flex-cpp-example/main.cpp:15)
	start (Unknown Source:0)

make
./mycompiler

	Examples:
	 * function()
	 * function(1,2,3)
	Terminate listing with ; to see parsed AST
	Terminate parser with Ctrl-D
	
	prompt> f(1)
	SRIDHAR Scanner: identifier [f]
	Scanner: '('
	Scanner: decimal number: 1
	first argument: 1
	Scanner: ')'
	function: f, 1
	SRIDHAR: command parsed, updating AST
	
	Assertion failed: (false), function parse, file parser.y, line 85.
	prompt> zsh: abort      ./mycompiler

Quick start
-----------
make
echo "f();" | ./mycompiler

EOF

