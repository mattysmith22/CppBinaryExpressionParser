#include "ast.h"

namespace ast
{
    operand::operand(bool v)
        : val{v}
    {}

    bool operand::eval() const {
        return val;
    }

    std::unique_ptr<node> operand::clone() const {
        return std::make_unique<operand>(val);
    }

    parentype parenthesis::type() const { return typ;}
    bool parenthesis::eval() const {throw cannot_eval_exception{};}

    std::unique_ptr<node> parenthesis::clone() const {
        return std::make_unique<parenthesis>(typ);
    }

    unaryoperator::unaryoperator()
        : child{nullptr} 
    {}

    unaryoperator::unaryoperator(std::unique_ptr<node> &n)
        : child{std::move(n)}
    {}

    void unaryoperator::addnode(std::unique_ptr<node> &toadd) {
        if(child.get() != nullptr)
            throw child_error_exception{};
        child = std::move(toadd);
    }

    int unaryoperator::arity() const {return 1;}

    std::unique_ptr<node> unaryoperator::clone() const {
        auto child1 = child ? child->clone() : nullptr;
        return create(std::move(child1));
    }

    void unaryoperator::checkevaluateable() const {
        if(!child.get())
            throw child_error_exception();
    }

    binaryoperator::binaryoperator()
        : child{nullptr} 
    {}

    binaryoperator::binaryoperator(std::unique_ptr<node> &n1, std::unique_ptr<node> &n2)
        : child{std::move(n1), std::move(n2)}
    {}

    void binaryoperator::addnode(std::unique_ptr<node> &toadd) {
        if(child[0].get() != nullptr)
            if(child[1].get() != nullptr)
                throw child_error_exception{};
            else
                child[1] = std::move(toadd);
        else
            child[0] = std::move(toadd);
    }

    int binaryoperator::arity() const {return 2;}

    std::unique_ptr<node> binaryoperator::clone() const {
        auto child1 = child[0] ? child[0]->clone() : nullptr;
        auto child2 = child[1] ? child[1]->clone() : nullptr;
        return create(std::move(child1), std::move(child2));
    }

    void binaryoperator::checkevaluateable() const {
        if(!(child[0].get() && child[1].get()))
            throw child_error_exception();
    }

    bool onot::eval() const {
        checkevaluateable();
        return !child->eval();
    }

    int onot::prescedence() const {return 6;}
    assoc onot::associativity() const {return assoc::right;}
    std::unique_ptr<operatr> onot::create(std::unique_ptr<node> child) const {
        return std::make_unique<onot>(child);
    }

    bool oand::eval() const {
        checkevaluateable();
        return child[0]->eval() && child[1]->eval();
    }

    int oand::prescedence() const {return 5;}
    assoc oand::associativity() const {return assoc::left;}
    std::unique_ptr<operatr> oand::create(std::unique_ptr<node> child1, std::unique_ptr<node> child2) const {
        return std::make_unique<oand>(child1, child2);
    }

    bool oor::eval() const {
        checkevaluateable();
        return child[0]->eval() || child[1]->eval();
    }

    int oor::prescedence() const {return 3;}
    assoc oor::associativity() const {return assoc::left;}
    std::unique_ptr<operatr> oor::create(std::unique_ptr<node> child1, std::unique_ptr<node> child2) const {
        return std::make_unique<oor>(child1, child2);
    }

    bool oimplies::eval() const {
        checkevaluateable();
        return child[1]->eval() ? child[0]->eval() : true;
    }

    int oimplies::prescedence() const {return 2;}
    assoc oimplies::associativity() const {return assoc::right;}
    std::unique_ptr<operatr> oimplies::create(std::unique_ptr<node> child1, std::unique_ptr<node> child2) const {
        return std::make_unique<oimplies>(child1, child2);
    }

    bool orevimplies::eval() const {
        checkevaluateable();
        return child[0]->eval() ? child[1]->eval() : true;
    }

    int orevimplies::prescedence() const {return 2;}
    assoc orevimplies::associativity() const {return assoc::right;}
    std::unique_ptr<operatr> orevimplies::create(std::unique_ptr<node> child1, std::unique_ptr<node> child2) const {
        return std::make_unique<orevimplies>(child1, child2);
    }

    bool oequal::eval() const {
        checkevaluateable();
        return child[0]->eval() == child[1]->eval();
    }

    int oequal::prescedence() const {return 1;}
    assoc oequal::associativity() const {return assoc::right;}
    std::unique_ptr<operatr> oequal::create(std::unique_ptr<node> child1, std::unique_ptr<node> child2) const {
        return std::make_unique<oequal>(child1, child2);
    }

    bool oxor::eval() const {
        checkevaluateable();
        return !child[0]->eval() != child[0]->eval();
    }

    int oxor::prescedence() const {return 3;}
    assoc oxor::associativity() const {return assoc::left;}
    std::unique_ptr<operatr> oxor::create(std::unique_ptr<node> child1, std::unique_ptr<node> child2) const {
        return std::make_unique<oxor>(child1, child2);
    }
}