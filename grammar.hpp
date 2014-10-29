#if !defined(ASCII_TREE_GRAMMAR_H)
#define ASCII_TREE_GRAMMAR_H

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
    };

    inline bool operator==(const token& lhs, const token& rhs)
    {
        return lhs.type == rhs.type
            && lhs.name == rhs.name;
    }

    inline std::vector<token> tokenize(const std::string& s)
    {
        std::vector<token> tokens;
        enum {
            none, open_square_brace, close_square_brace, asterisk, dash,
            open_paren, close_paren, name_char, slash, backslash, pipe
        } prev_ch = none, prev_prev_ch = none;
        size_t marker = 0, marked_length = 0;

        for (size_t i = 0; i < s.size(); ++i)
        {
            auto ch = s[i];

            if (ch == '[')
            {
                prev_ch = open_square_brace;
            }
            else if (ch == ']')
            {
                if (prev_ch == asterisk)
                {
                    tokens.emplace_back(token { token::root_node, "" });
                }
                else
                {
                    tokens.emplace_back(token { token::named_node, s.substr(marker, marked_length) });
                }

                prev_ch = close_square_brace;
            }
            else if (ch == '*')
            {
                prev_ch = asterisk;
            }
            else if (ch == '-')
            {
                if (prev_ch == name_char)
                {
                    tokens.emplace_back(token { token::horizontal_edge, s.substr(marker, marked_length) });
                }

                prev_ch = dash;
            }
            else if (ch == '/')
            {
                tokens.emplace_back(token { token::ascending_edge_part, "" });
                prev_ch = slash;
            }
            else if (ch == '\\')
            {
                tokens.emplace_back(token { token::descending_edge_part, "" });
                prev_ch = backslash;
            }
            else if (ch == '|')
            {
                tokens.emplace_back(token { token::vertical_edge_part, "" });
                prev_ch = pipe;
            }
            else if (ch == '(')
            {
                if (prev_ch == dash) { prev_prev_ch = dash; }
                prev_ch = open_paren;
            }
            else if (ch == ')')
            {
                if (prev_prev_ch == dash)
                {
                    tokens.emplace_back(token { token::horizontal_edge, s.substr(marker, marked_length) });
                }
                else
                {
                    tokens.emplace_back(token { token::edge_name, s.substr(marker, marked_length) });
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
        }

        return tokens;
    }
}

#endif // ASCII_TREE_GRAMMAR_H
