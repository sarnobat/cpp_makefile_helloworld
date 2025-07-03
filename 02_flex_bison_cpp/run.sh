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

EOF

