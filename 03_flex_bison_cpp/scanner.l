%{
//	#include <iostream>
	#include <cstdlib>
	#include "scanner.h"
	#include "parser.hpp"
    #include <iostream>
    #include <iomanip>


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
                std::cout
                    // [trace]
                    << "\n\033[38;5;45m[trace]\033[0m "

                    // %10s:%-5d
                    << "\033[38;5;220m"
                    << std::right << std::setw(10) << __FILE__ << ":"
                    << std::left  << std::setw(5)  << __LINE__
                    << "\033[0m "

                    // %32s()  ← MUST reset to right
                    << "\033[38;5;82m"
                    << std::right << std::setw(34)
                    << (std::string(__func__) + "()")
                    << "\033[0m "

                    // SRIDHAR
                    << "\033[38;5;202mSRIDHAR\033[0m"
                    << ": 4 - scanner waiting for next token)\n";


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
