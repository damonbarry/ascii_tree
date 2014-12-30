#if !defined(ASCII_TREE_SYNTAX_TREE_H)
#define ASCII_TREE_SYNTAX_TREE_H

#include "grammar.hpp"
#include <string>
#include <memory>
#include <vector>
#include <algorithm>

namespace ascii_tree
{
    struct analyze_exception
    {
        const std::string reason;
        analyze_exception(const std::string& reason) : reason(reason) {}
    };

    struct node
    {
        token tok;
        explicit node(const token& tok) : tok(tok) {}
    };

    class syntax_tree
    {
        std::unique_ptr<node> root_;
        const std::vector<token> tokens_;
        typedef std::vector<token>::const_iterator token_iterator;

        token_iterator find_root_node_(token_iterator begin)
        {
            return std::find_if(begin, tokens_.end(), [](const token& tok){
                return tok.type == token::root_node;
            });
        }

    public:
        explicit syntax_tree(const std::vector<token>& tokens) : tokens_(tokens) { }

        const node& analyze()
        {
            auto root_it = find_root_node_(tokens_.begin());
            if (root_it == tokens_.end()) { throw analyze_exception("missing root node"); }

            auto dup_it = find_root_node_(root_it + 1);
            if (dup_it != tokens_.end()) { throw analyze_exception("more than one root node"); }

            root_ = std::make_unique<node>(*root_it);
            return *root_;
        }
    };
}

#endif // ASCII_TREE_SYNTAX_TREE_H
