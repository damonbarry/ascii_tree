#if !defined(ASCII_TREE_SYNTAX_TREE_H)
#define ASCII_TREE_SYNTAX_TREE_H

#include "grammar.hpp"
#include <string>
#include <vector>

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

        const token* analyze()
        {
            if (tokens_.empty()) { throw analyze_exception("missing root node"); }
            return &tokens_.front();
        }
    };
}

#endif // ASCII_TREE_SYNTAX_TREE_H
