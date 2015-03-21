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

    struct edge;
    struct node
    {
        token tok;
        std::vector<edge> edges;
        explicit node(const token& tok) : tok(tok) {}
    };

    struct edge
    {
        token tok;
        node node;
        edge(const token& tok, const ascii_tree::node& n) : tok(tok), node(n) {}
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
        explicit syntax_tree(const std::vector<token>& tokens) : tokens_(tokens) {}

        const node& analyze()
        {
            auto root_it = find_root_node_(tokens_.begin());
            if (root_it == tokens_.end()) { throw analyze_exception("missing root node"); }

            auto dup_it = find_root_node_(root_it + 1);
            if (dup_it != tokens_.end()) { throw analyze_exception("more than one root node"); }

            root_ = std::make_unique<node>(*root_it);

            if (root_it != tokens_.begin())
            {
                auto prev_it = root_it - 1;
                if (prev_it->type == token::horizontal_edge)
                {
                    root_->edges.emplace_back(*prev_it, node(*(prev_it - 1)));
                }
                else if (prev_it->type == token::vertical_edge_part) // vertical_edge_part + edge_name + vertical_edge_part + node
                {
                    root_->edges.emplace_back(*(prev_it - 1), node(*(prev_it - 3)));
                }
                else if (prev_it->type == token::ascending_edge_part) // ascending_edge_part + edge_name + ascending_edge_part + node
                {
                    root_->edges.emplace_back(*(prev_it - 1), node(*(prev_it - 3)));
                }
                else if (prev_it->type == token::descending_edge_part) // descending_edge_part + edge_name + descending_edge_part + node
                {
                    root_->edges.emplace_back(*(prev_it - 1), node(*(prev_it - 3)));
                }
            }

            if (root_it + 1 != tokens_.end())
            {
                auto next_it = root_it + 1;
                if (next_it->type == token::horizontal_edge)
                {
                    root_->edges.emplace_back(*next_it, node(*(next_it + 1)));
                }
                else if (next_it->type == token::vertical_edge_part) // vertical_edge_part + edge_name + vertical_edge_part + node
                {
                    root_->edges.emplace_back(*(next_it + 1), node(*(next_it + 3)));
                }
                else if (next_it->type == token::ascending_edge_part) // ascending_edge_part + edge_name + ascending_edge_part + node
                {
                    root_->edges.emplace_back(*(next_it + 1), node(*(next_it + 3)));
                }
                else if (next_it->type == token::descending_edge_part) // descending_edge_part + edge_name + descending_edge_part + node
                {
                    root_->edges.emplace_back(*(next_it + 1), node(*(next_it + 3)));
                }
            }

            return *root_;
        }
    };
}

#endif // ASCII_TREE_SYNTAX_TREE_H
