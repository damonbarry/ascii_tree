#if !defined(ASCII_TREE_SYNTAX_TREE_H)
#define ASCII_TREE_SYNTAX_TREE_H

#include "grammar.hpp"
#include <string>
#include <vector>
#include <algorithm>

namespace ascii_tree
{
    struct analyze_exception
    {
        const std::string reason;
        analyze_exception(const std::string& reason) : reason(reason) {}
    };

    class syntax_tree
    {
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

        const token& analyze()
        {
            auto root_it = find_root_node_(tokens_.begin());
            if (root_it == tokens_.end()) { throw analyze_exception("missing root node"); }

            auto dup_it = find_root_node_(root_it + 1);
            if (dup_it != tokens_.end()) { throw analyze_exception("more than one root node"); }

            return *root_it;
        }
    };
}

#endif // ASCII_TREE_SYNTAX_TREE_H
