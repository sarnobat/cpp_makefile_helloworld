%{
//	#include <iostream>
	#include <cstdlib>
	#include "scanner.h"
	#include "parser.hpp"

	using namespace std;

	// Original yyterminate() macro returns int. Since we're using Bison 3 variants
	// as tokens, we must redefine it to change type from `int` to `Parser::semantic_type`
	#define yyterminate() EzAquarii::Parser::make_END();

	
%}

%option nodefault
%option noyywrap
%option c++
%option yyclass="Scanner"
%option prefix="EzAquarii_"

%%


[a-z]+      { 
                cout << "SRIDHAR Scanner: identifier [" << yytext << "]" << endl;
                //assert(false);
                return EzAquarii::Parser::make_STRING(yytext); 
            }
            
\(          {
                cout << "Scanner: '('" << endl;
                return EzAquarii::Parser::make_LEFTPAR();
            }
            
\)          { 
                cout << "Scanner: ')'" << endl;
                return EzAquarii::Parser::make_RIGHTPAR();
            }
            
;           {
                cout << "Scanner: ';'" << endl;
                return EzAquarii::Parser::make_SEMICOLON();
            }
            
,           {
                cout << "Scanner: ','" << endl;
                return EzAquarii::Parser::make_COMMA();
            }
            
[\n\t ]     {
                //cout << "Scanner: whitechar (ignored)" << endl;
            }

[1-9][0-9]* {
                cout << "Scanner: decimal number: " << yytext << endl;
                uint64_t number = strtoull(yytext, 0, 10);
                return EzAquarii::Parser::make_NUMBER(number);
            }

.           { 
                cout << "Scanner: unknown character [" << yytext << "]" << endl; 
            }
            
<<EOF>>     { return yyterminate(); }


%%
