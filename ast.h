#ifndef AST_H
#define AST_H

#include <memory>

namespace ast
{
    enum assoc {
        left,
        right
    };

    enum parentype {
        open,
        close
    };

    class child_error_exception : public std::exception {};
    class cannot_eval_exception : public std::exception {};

    class node
    {
        public:
            virtual ~node() = default;
            virtual bool eval() const = 0;
            virtual std::unique_ptr<node> clone() const = 0;
    };

    class operand : public node
    {
        bool val;

        public:
            operand(bool v);
            bool eval() const;
            std::unique_ptr<node> clone() const;
    };

    class operatr: public node
    {
        public:
            virtual void addnode(std::unique_ptr<node> &toadd) = 0;
            virtual int arity() const = 0;
            virtual int prescedence() const = 0;
            virtual assoc associativity() const = 0;
        protected:
            virtual void checkevaluateable() const = 0;
    };

    class parenthesis : public node
    {
        parentype typ;
        public:
            parenthesis(parentype typein) : typ{typein} {}
            std::unique_ptr<node> clone() const;
            parentype type() const;
            bool eval() const;
    };

    class unaryoperator : public operatr
    {

        public:
            unaryoperator();
            unaryoperator(std::unique_ptr<node> &n);

            std::unique_ptr<node> clone() const;
            void addnode(std::unique_ptr<node> &toadd);
            int arity() const;
        protected:
            std::unique_ptr<node> child;
            void checkevaluateable() const;
            virtual std::unique_ptr<operatr> create(std::unique_ptr<node> child) const = 0;
    };

    class binaryoperator : public operatr
    {
        public:
            binaryoperator();
            binaryoperator(std::unique_ptr<node> &n1, std::unique_ptr<node> &n2);

            std::unique_ptr<node> clone() const;
            void addnode(std::unique_ptr<node> &toadd);
            int arity() const;
        protected:
            std::unique_ptr<node> child[2];
            void checkevaluateable() const;
            virtual std::unique_ptr<operatr> create(std::unique_ptr<node> child1, std::unique_ptr<node> child2) const = 0;
    };

    class onot : public unaryoperator {
        public:
            using unaryoperator::unaryoperator;
            int prescedence() const;
            assoc associativity() const;
            bool eval() const;
        protected:
            std::unique_ptr<operatr> create(std::unique_ptr<node> child) const;
    };
    class oand : public binaryoperator {
        public:
            using binaryoperator::binaryoperator;
            int prescedence() const;
            assoc associativity() const;
            bool eval() const;
        protected:
            std::unique_ptr<operatr> create(std::unique_ptr<node> child1, std::unique_ptr<node> child2) const;
    };
    class oor : public binaryoperator {
        public:
            using binaryoperator::binaryoperator;
            int prescedence() const;
            assoc associativity() const;
            bool eval() const;
        protected:
            std::unique_ptr<operatr> create(std::unique_ptr<node> child1, std::unique_ptr<node> child2) const;
    };
    class oimplies : public binaryoperator {
        public:
            using binaryoperator::binaryoperator;
            int prescedence() const;
            assoc associativity() const;
            bool eval() const;
        protected:
            std::unique_ptr<operatr> create(std::unique_ptr<node> child1, std::unique_ptr<node> child2) const;
    };
    class orevimplies : public binaryoperator {
        public:
            using binaryoperator::binaryoperator;
            int prescedence() const;
            assoc associativity() const;
            bool eval() const;
        protected:
            std::unique_ptr<operatr> create(std::unique_ptr<node> child1, std::unique_ptr<node> child2) const;
    };
    class oequal : public binaryoperator {
        public:
            using binaryoperator::binaryoperator;
            int prescedence() const;
            assoc associativity() const;
            bool eval() const;
        protected:
            std::unique_ptr<operatr> create(std::unique_ptr<node> child1, std::unique_ptr<node> child2) const;
    };
    class oxor : public binaryoperator {
        public:
            using binaryoperator::binaryoperator;
            int prescedence() const;
            assoc associativity() const;
            bool eval() const;
        protected:
            std::unique_ptr<operatr> create(std::unique_ptr<node> child1, std::unique_ptr<node> child2) const;
    };
}

#endif
