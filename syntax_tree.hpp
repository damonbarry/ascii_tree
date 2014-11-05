#if !defined(ASCII_TREE_SYNTAX_TREE_H)
#define ASCII_TREE_SYNTAX_TREE_H

#include <string>

namespace ascii_tree
{
    struct analyze_exception
    {
        const std::string reason;
        analyze_exception(const std::string& reason) : reason(reason) {}
    };

    class syntax_tree
    {
    public:
        void analyze(const std::string&)
        {
            throw analyze_exception("missing root node");
        }
    };
}

#endif // ASCII_TREE_SYNTAX_TREE_H
