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

    inline bool operator==(const node& lhs, const node& rhs)
    {
        return lhs.tok == rhs.tok && lhs.edges == rhs.edges;
    }

    struct edge
    {
        token tok;
        node node;
        edge(const token& tok, const ascii_tree::node& n) : tok(tok), node(n) {}
    };

    inline bool operator==(const edge& lhs, const edge& rhs)
    {
        return lhs.tok == rhs.tok && lhs.node == rhs.node;
    }

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

            // look left for edge + node
            std::vector<const token*> horizontal_edges;
            std::vector<const token*> named_nodes;
            for (const auto& token : tokens_)
            {
                if (token.type == token::horizontal_edge)
                {
                    horizontal_edges.push_back(&token);
                }

                if (token.type == token::named_node)
                {
                    named_nodes.push_back(&token);
                }
            }

            position candidate_position = position::from(root_it->position, 0, 1);
            auto candidate_horizontal_edge_it = std::find_if(horizontal_edges.begin(), horizontal_edges.end(),
                [&](const token* t){ return t->position == candidate_position; });
            if (candidate_horizontal_edge_it == horizontal_edges.end())
            {
                return *root_;
            }

            // found an edge, now find the node at the other end of it
            candidate_position = position::from(candidate_position, 0, 1);
            auto candidate_named_node_it = std::find_if(named_nodes.begin(), named_nodes.end(),
                [&](const token* t){ return t->position == candidate_position; });
            if (candidate_named_node_it == named_nodes.end())
            {
                // no node at the other end of the edge
                throw analyze_exception("missing named_node at the end of a horizontal edge");
            }

            root_->edges.emplace_back(**candidate_horizontal_edge_it, node(**candidate_named_node_it));

            return *root_;
        }
    };
}

#endif // ASCII_TREE_SYNTAX_TREE_H
