#if !defined(ASCII_TREE_GRAMMAR_H)
#define ASCII_TREE_GRAMMAR_H

#include <utility>
#include <cctype>
#include <string>
#include <vector>

#if 0

GRAMMAR
========

tokens:           token
                  tokens token

token:            node
                  horizontal-edge
                  edge-part

node:             root-node
                  named-node

root-node:        '[' '*' ']'

named-node:       '[' node-name ']'

node-name:        name-chars

name-chars:       name-char
                  name-chars name-char

name-char:        alnum
                  '_'

alnum:            one of: [A-Za-z0-9]

horizontal-edge:  edge-chars edge-name edge-chars

edge-chars:       '-'
                  edge-chars '-'

edge-name:        '(' name-chars ')'

edge-part:        '\'
                  '|'
                  '/'
                  edge-name

#endif

namespace ascii_tree
{
    struct token
    {
        enum toktype { root_node, named_node, edge_name, horizontal_edge, ascending_edge_part, descending_edge_part, vertical_edge_part };
        toktype type;
        std::string name;

        token(toktype type, std::string&& name) : type(type), name(std::move(name)) {}
    };

    inline bool operator==(const token& lhs, const token& rhs)
    {
        return lhs.type == rhs.type
            && lhs.name == rhs.name;
    }

    inline token root_node() { return token(token::root_node, ""); }
    inline token named_node(std::string&& name) { return token(token::named_node, std::move(name)); }
    inline token edge_name(std::string&& name) { return token(token::edge_name, std::move(name)); }
    inline token horizontal_edge(std::string&& name) { return token(token::horizontal_edge, std::move(name)); }
    inline token ascending_edge_part() { return token(token::ascending_edge_part, ""); }
    inline token descending_edge_part() { return token(token::descending_edge_part, ""); }
    inline token vertical_edge_part() { return token(token::vertical_edge_part, ""); }

    enum Terminals
    {
        none, open_square_brace, close_square_brace, asterisk, dash,
        open_paren, close_paren, name_char, slash, backslash, pipe
    };

    struct ascii_tree_parse_exception
    {
        const std::string s;
        const size_t pos;

        ascii_tree_parse_exception(const std::string& s, size_t pos) : s(s), pos(pos) {}
    };

    inline std::vector<token> tokenize(const std::string& s)
    {
        std::vector<token> tokens;
        Terminals prev_ch = none, prev_prev_ch = none;
        size_t marker = 0, marked_length = 0;

        for (size_t i = 0; i < s.size(); ++i)
        {
            auto ch = s[i];

            if (ch == '[')
            {
                if (prev_ch == open_square_brace)
                {
                    throw ascii_tree_parse_exception(s, i);
                }

                prev_ch = open_square_brace;
            }
            else if (ch == ']')
            {
                if (prev_ch == asterisk)
                {
                    tokens.emplace_back(root_node());
                }
                else if (prev_ch == open_square_brace)
                {
                    throw ascii_tree_parse_exception(s, i);
                }
                else
                {
                    tokens.emplace_back(named_node(s.substr(marker, marked_length)));
                }

                prev_ch = close_square_brace;
            }
            else if (ch == '*')
            {
                prev_ch = asterisk;
            }
            else if (ch == '-')
            {
                if (prev_ch == close_paren)
                {
                    tokens.emplace_back(horizontal_edge(s.substr(marker, marked_length)));
                }

                prev_ch = dash;
            }
            else if (ch == '/')
            {
                tokens.emplace_back(ascending_edge_part());
                prev_ch = slash;
            }
            else if (ch == '\\')
            {
                tokens.emplace_back(descending_edge_part());
                prev_ch = backslash;
            }
            else if (ch == '|')
            {
                tokens.emplace_back(vertical_edge_part());
                prev_ch = pipe;
            }
            else if (ch == '(')
            {
                if (prev_ch == dash) { prev_prev_ch = dash; }
                prev_ch = open_paren;
            }
            else if (ch == ')')
            {
                if (prev_prev_ch == none)
                {
                    tokens.emplace_back(edge_name(s.substr(marker, marked_length)));
                }

                prev_ch = close_paren;
            }
            else if (isalnum(ch) || ch == '_')
            {
                if (prev_ch != name_char)
                {
                    marker = i;
                    marked_length = 0;
                }

                prev_ch = name_char;
                ++marked_length;
            }
            else if (ch != ' ')
            {
                throw ascii_tree_parse_exception(s, i);
            }
        }

        return tokens;
    }
}

#endif // ASCII_TREE_GRAMMAR_H
