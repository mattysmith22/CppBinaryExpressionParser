#include "tokeniser.h"

namespace tokeniser
{
    bad_parse_exception::bad_parse_exception(std::string strin, size_t pos)
    {
        badstring = strin.substr(pos, strin.length() - pos - 1);
    }

    bool tokeniser::empty() {return tokens.empty();}

    simpletokeniser::simpletokeniser(std::string strin)
    {
        size_t cursor = 0; //The point at which we are currently looking at
        while(cursor < strin.length())
            tokens.push_back(parse_token(strin, cursor));
    };

    std::unique_ptr<ast::node> simpletokeniser::parse_token(std::string strin, size_t &cursor)
    {
        std::unique_ptr<ast::node> nodeOut;
        switch (strin[cursor])
        {
            case '!':
                nodeOut = std::make_unique<ast::onot>();
                break;
            case '&':
                nodeOut = std::make_unique<ast::oand>();
                break;
            case '^':
                nodeOut = std::make_unique<ast::oxor>();
                break;
            case '|':
                nodeOut = std::make_unique<ast::oor>();
                break;
            case '>':
                nodeOut = std::make_unique<ast::oimplies>();
                break;
            case '<':
                nodeOut = std::make_unique<ast::orevimplies>();
                break;
            case '=':
                nodeOut = std::make_unique<ast::oequal>();
                break;
            case '(':
                nodeOut = std::make_unique<ast::parenthesis>(ast::parentype::open);
                break;
            case ')':
                nodeOut = std::make_unique<ast::parenthesis>(ast::parentype::close);
                break;
            case '1':
                nodeOut = std::make_unique<ast::operand>(true);
                break;
            case '0':
                nodeOut = std::make_unique<ast::operand>(false);
                break;
            default:
                throw bad_parse_exception{strin, cursor};
        }
        cursor++;
        return nodeOut;
    }
    
}