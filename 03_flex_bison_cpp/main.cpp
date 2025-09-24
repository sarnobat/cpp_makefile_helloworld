// #include <iostream>
#include "scanner.h"
#include "parser.hpp"
#include "command.h"

#include <sstream>

using namespace EzAquarii;
using namespace std;

int main(int argc, char **argv) {

    Scanner s{};

    int res = Parser(s).parse();
    // cout << "Parse complete. Result = " << res << endl;
    // return res;
    return -1;
}
