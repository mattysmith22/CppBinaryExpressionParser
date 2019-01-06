#include "shuntingyard.h"
#include <stack>
#include <queue>
#include <iterator>

namespace shuntingyard
{
    //Condition used to work out whether the operator should get removed from the stack
    bool shouldpopoperator(const std::stack<std::unique_ptr<ast::node>> &oprstack , const ast::operatr *oper)
    {
        if(oprstack.empty())
            return false;
        else if(const ast::operatr *stkoper = dynamic_cast<ast::operatr*>(oprstack.top().get()))
            return ((stkoper->prescedence() > oper->prescedence()) ||
                    (stkoper->prescedence() == oper->prescedence() && stkoper->associativity() == ast::assoc::left)) &&
                    (oper->arity() == 1 ? stkoper->arity() == 1 : true);
        else if(dynamic_cast<ast::parenthesis*>(oprstack.top().get()))
            return false;
        else
            throw std::exception{};
    }

    std::queue<std::unique_ptr<ast::node>> infixtorpn(std::unique_ptr<tokeniser::tokeniser> &tokens)
    {
        std::queue<std::unique_ptr<ast::node>> outqueue{};
        std::stack<std::unique_ptr<ast::node>> oprstack{};

        for(auto const &token: tokens->tokens)
        {
            if(dynamic_cast<ast::operand*>(token.get()))
                outqueue.push(token->clone());
            else if (auto oper = dynamic_cast<ast::operatr*>(token.get()))
            {
                while(shouldpopoperator(oprstack, oper))
                {
                    outqueue.push(std::move(oprstack.top()));
                    oprstack.pop();
                }
                oprstack.push(token->clone());
            }
            else if (auto paren = dynamic_cast<ast::parenthesis*>(token.get()))
            {
                if (paren->type() == ast::parentype::open)
                    oprstack.push(token->clone());
                else
                {
                    while(!oprstack.empty() && !dynamic_cast<ast::parenthesis*>(oprstack.top().get()))
                    {
                        outqueue.push(std::move(oprstack.top()));
                        oprstack.pop();
                    }
                    if(oprstack.empty())
                        throw bad_expression_exception{};
                    else
                        oprstack.pop();
                }
            }
        }

        while (!oprstack.empty() && !dynamic_cast<ast::parenthesis*>(oprstack.top().get()))
        {
            outqueue.push(std::move(oprstack.top()));
            oprstack.pop();
        }

        if(!oprstack.empty())
            throw bad_expression_exception{};
        
        return outqueue;
    }

    std::unique_ptr<ast::node> rpntoast(std::queue<std::unique_ptr<ast::node>> &rpn)
    {
        std::stack<std::unique_ptr<ast::node>> aststack{};
        while(!rpn.empty())
        {
            if(dynamic_cast<ast::operand*>(rpn.front().get()))
                aststack.push(std::move(rpn.front()));
            else if(auto oper = dynamic_cast<ast::unaryoperator*>(rpn.front().get()))
            {
                if(aststack.size() < 1)
                    throw bad_expression_exception{};
                auto child = std::move(aststack.top());
                aststack.pop();
                oper->addnode(child);
                aststack.push(std::move(rpn.front()));
            }
            else if(auto oper = dynamic_cast<ast::binaryoperator*>(rpn.front().get()))
            {
                if(aststack.size() < 2)
                    throw bad_expression_exception{};
                auto child = std::move(aststack.top());
                aststack.pop();
                oper->addnode(child);
                child = std::move(aststack.top());
                aststack.pop();
                oper->addnode(child);
                aststack.push(std::move(rpn.front()));
            }
            else
                throw bad_expression_exception{};
            rpn.pop();
        }
        if(aststack.size() != 1)
            throw bad_expression_exception{};
        return std::move(aststack.top());
    }

}
