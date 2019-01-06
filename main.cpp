#include "ast.h"
#include "tokeniser.h"
#include "shuntingyard.h"

#include <iostream>

int main(int argc, char** argv)
{
    if(argc == 1)
    {
        std::cout << "Wrong number of arguments!\n";
        return 1;
    }
    
    std::cout << "Tokenising\n";
    std::unique_ptr<tokeniser::tokeniser> tokens = std::make_unique<tokeniser::simpletokeniser>(std::string{argv[1]});
    std::cout << "Parsing to RPN\n";
    auto rpn = shuntingyard::infixtorpn(tokens);
    std::cout << "RPN to AST\n";
    auto ast = shuntingyard::rpntoast(rpn);
    std::cout << "Evaluating: " << ast->eval() << "\n";

    return 0;
}