#ifndef TOKENISER_H
#define TOKENISER_H

#include "ast.h"

#include <vector>
#include <memory>
#include <string>

namespace tokeniser
{
    class bad_parse_exception : public std::exception
    {
        public:
            bad_parse_exception(std::string strin, size_t pos);
            std::string badstring;
    };

    class tokeniser
    {
        public:
            std::vector<std::unique_ptr<ast::node>> tokens;
            bool empty();
            std::unique_ptr<ast::node> &peek();
            std::unique_ptr<ast::node> &pop();
        protected:
            virtual std::unique_ptr<ast::node> parse_token(std::string strin, size_t &cursor) = 0;
    };

    class simpletokeniser : public tokeniser
    {
        public:
            simpletokeniser(std::string strin);
        protected:
            std::unique_ptr<ast::node> parse_token(std::string strin, size_t &cursor);
    };
}

#endif