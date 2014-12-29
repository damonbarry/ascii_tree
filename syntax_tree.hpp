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
    public:
        explicit syntax_tree(const std::vector<token>& tokens) : tokens_(tokens) { }

        const token& analyze()
        {
            if (tokens_.empty()) { throw analyze_exception("missing root node"); }
            auto root_it = std::find_if(tokens_.begin(), tokens_.end(), [](const token& tok){
                return tok.type == token::root_node;
            });
            return *root_it;
        }
    };
}

#endif // ASCII_TREE_SYNTAX_TREE_H
