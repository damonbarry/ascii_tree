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

        std::vector<token_iterator>::const_iterator find_token_(
            token_iterator from_it,
            std::ptrdiff_t row_offset,
            std::ptrdiff_t column_offset,
            const std::vector<token_iterator>& candidates)
        {
            position candidate_position = position::from(from_it->position, row_offset, column_offset);
            return std::find_if(candidates.begin(), candidates.end(),
                [&](const token_iterator& it){ return it->position == candidate_position; });
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

            // build index of tokens by type
            std::vector<token_iterator> horizontal_edges;
            std::vector<token_iterator> vertical_edge_parts;
            std::vector<token_iterator> ascending_edge_parts;
            std::vector<token_iterator> edge_names;
            std::vector<token_iterator> named_nodes;

            for (auto token_it = tokens_.begin(); token_it != tokens_.end(); ++token_it)
            {
                if (token_it->type == token::horizontal_edge)
                {
                    horizontal_edges.push_back(token_it);
                }
                else if (token_it->type == token::vertical_edge_part)
                {
                    vertical_edge_parts.push_back(token_it);
                }
                else if (token_it->type == token::ascending_edge_part)
                {
                    ascending_edge_parts.push_back(token_it);
                }
                else if (token_it->type == token::edge_name)
                {
                    edge_names.push_back(token_it);
                }
                else if (token_it->type == token::named_node)
                {
                    named_nodes.push_back(token_it);
                }
            }

            // look right for edge + node
            auto edge_it = find_token_(root_it, 0, 1, horizontal_edges);
            if (edge_it != horizontal_edges.end())
            {
                // found an edge, now find the node at the other end of it
                auto node_it = find_token_(*edge_it, 0, 1, named_nodes);
                if (node_it == named_nodes.end())
                {
                    throw analyze_exception("missing named_node at the end of a horizontal edge");
                }

                root_->edges.emplace_back(**edge_it, node(**node_it));
            }

            // look left for edge + node
            edge_it = find_token_(root_it, 0, -1, horizontal_edges);
            if (edge_it != horizontal_edges.end())
            {
                // found an edge, now find the node at the other end of it
                auto node_it = find_token_(*edge_it, 0, -1, named_nodes);
                if (node_it == named_nodes.end())
                {
                    throw analyze_exception("missing named_node at the end of a horizontal edge");
                }

                root_->edges.emplace_back(**edge_it, node(**node_it));
            }

            // look down for edge + node
            edge_it = find_token_(root_it, -1, 0, vertical_edge_parts);
            if (edge_it != vertical_edge_parts.end())
            {
                // found the start of an edge, now look for the rest of it
                auto name_it = find_token_(*edge_it, -1, 0, edge_names);
                if (name_it == edge_names.end())
                {
                    throw analyze_exception("expected edge_name");
                }

                auto edge2_it = find_token_(*name_it, -1, 0, vertical_edge_parts);
                if (edge2_it == vertical_edge_parts.end())
                {
                    throw analyze_exception("expected vertical_edge_part");
                }

                // found an edge, now find the node at the other end of it
                auto node_it = find_token_(*edge2_it, -1, 0, named_nodes);
                if (node_it == named_nodes.end())
                {
                    throw analyze_exception("expected named_node");
                }

                // A vertical edge consists of three pieces: two vertical_edge_parts with an edge_name between them.
                // For now, represent them within an edge by adding the name to the first vertical_edge_part.
                root_->edges.emplace_back(token((*edge_it)->type, std::string((*name_it)->name), (*edge_it)->position), node(**node_it));
            }

            // look up for edge + node
            edge_it = find_token_(root_it, 1, 0, vertical_edge_parts);
            if (edge_it != vertical_edge_parts.end())
            {
                // found the start of an edge, now look for the rest of it
                auto name_it = find_token_(*edge_it, 1, 0, edge_names);
                if (name_it == edge_names.end())
                {
                    throw analyze_exception("expected edge_name");
                }

                auto edge2_it = find_token_(*name_it, 1, 0, vertical_edge_parts);
                if (edge2_it == vertical_edge_parts.end())
                {
                    throw analyze_exception("expected vertical_edge_part");
                }

                // found an edge, now find the node at the other end of it
                auto node_it = find_token_(*edge2_it, 1, 0, named_nodes);
                if (node_it == named_nodes.end())
                {
                    throw analyze_exception("expected named_node");
                }

                // A vertical edge consists of three pieces: two vertical_edge_parts with an edge_name between them.
                // For now, represent them within an edge by adding the name to the first vertical_edge_part.
                root_->edges.emplace_back(token((*edge_it)->type, std::string((*name_it)->name), (*edge_it)->position), node(**node_it));
            }

            // look up & right for edge + node
            edge_it = find_token_(root_it, 1, 1, ascending_edge_parts);
            if (edge_it != ascending_edge_parts.end())
            {
                // found the start of an edge, not look for the rest of it
                auto name_it = find_token_(*edge_it, 1, 1, edge_names);
                if (name_it == edge_names.end())
                {
                    throw analyze_exception("expected edge_name");
                }

                auto edge2_it = find_token_(*name_it, 1, 1, ascending_edge_parts);
                if (edge2_it == vertical_edge_parts.end())
                {
                    throw analyze_exception("expected ascending_edge_parts");
                }

                // found an edge, now find the node at the other end of it
                auto node_it = find_token_(*edge2_it, 1, 1, named_nodes);
                if (node_it == named_nodes.end())
                {
                    throw analyze_exception("expected named_node");
                }

                // An ascending edge consists of three pieces: two ascending_edge_parts with an edge_name between them.
                // For now, represent them within an edge by adding the name to the first vertical_edge_part.
                root_->edges.emplace_back(token((*edge_it)->type, std::string((*name_it)->name), (*edge_it)->position), node(**node_it));
            }

            return *root_;
        }
    };
}

#endif // ASCII_TREE_SYNTAX_TREE_H
