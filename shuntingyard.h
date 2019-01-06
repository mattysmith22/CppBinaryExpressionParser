#ifndef SHUNTINGYARD_H
#define SHUNTINGYARD_H

#include "ast.h"
#include "tokeniser.h"
#include <queue>

namespace shuntingyard
{
    class bad_expression_exception : public std::exception {};

    std::unique_ptr<ast::node> rpntoast(std::queue<std::unique_ptr<ast::node>> &rpn);

    std::queue<std::unique_ptr<ast::node>> infixtorpn(std::unique_ptr<tokeniser::tokeniser> &tokens);
}

#endif