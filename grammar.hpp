#if !defined(ASCII_TREE_GRAMMAR_H)
#define ASCII_TREE_GRAMMAR_H

#include <cctype>
#include <string>
#include <vector>

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
        } prev = none;
        size_t marker = 0, marked_length = 0;

        for (size_t i = 0; i < s.size(); ++i)
        {
            auto ch = s[i];

            if (ch == '[')
            {
                prev = open_square_brace;
            }
            else if (ch == ']')
            {
                if (prev == asterisk)
                {
                    tokens.emplace_back(token { token::root_node, "" });
                }
                else
                {
                    tokens.emplace_back(token { token::named_node, s.substr(marker, marked_length) });
                }

                prev = close_square_brace;
            }
            else if (ch == '*')
            {
                prev = asterisk;
            }
            else if (ch == '-')
            {
                if (prev == name_char)
                {
                    tokens.emplace_back(token { token::horizontal_edge, s.substr(marker, marked_length) });
                }

                prev = dash;
            }
            else if (ch == '/')
            {
                tokens.emplace_back(token { token::ascending_edge_part, "" });
                prev = slash;
            }
            else if (ch == '\\')
            {
                tokens.emplace_back(token { token::descending_edge_part, "" });
                prev = backslash;
            }
            else if (ch == '|')
            {
                tokens.emplace_back(token { token::vertical_edge_part, "" });
                prev = pipe;
            }
            else if (ch == '(')
            {
                prev = open_paren;
            }
            else if (ch == ')')
            {
                tokens.emplace_back(token { token::edge_name, s.substr(marker, marked_length) });
                prev = close_paren;
            }
            else if (isalnum(ch) || ch == '_')
            {
                if (prev != name_char)
                {
                    marker = i;
                    marked_length = 0;
                }

                prev = name_char;
                ++marked_length;
            }
        }

        return tokens;
    }
}

#endif // ASCII_TREE_GRAMMAR_H
