#if !defined(ASCII_TREE_GRAMMAR_H)
#define ASCII_TREE_GRAMMAR_H

#include <iterator>
#include <utility>
#include <cctype>
#include <string>
#include <vector>
#include "parser.hpp"

#if 0

GRAMMAR
========

tokens:                 token
                        tokens token

token:                  root-node
                        named-node
                        horizontal-edge
                        descending-edge-part
                        vertical-edge-part
                        ascending-edge-part
                        edge-name

root-node:              '[' '*' ']'

named-node:             '[' name-chars ']'

name-chars:             name-char
                        name-chars name-char

name-char:              alnum
                        '_'

alnum:                  one of: [A-Za-z0-9]

horizontal-edge:        edge-chars edge-name edge-chars

edge-chars:             '-'
                        edge-chars '-'

descending-edge-part    '\'
vertical-edge-part      '|'
ascending-edge-part     '/'
edge-name:              '(' name-chars ')'


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

    enum terminal
    {
        none, open_square_brace, close_square_brace, asterisk, dash,
        open_paren, close_paren, name_char, slash, backslash, pipe, space
    };

    struct terminal_traits
    {
        typedef terminal type;

        static const type space = terminal::space;

        static terminal to_terminal(char ch)
        {
            if (ch == '[') return open_square_brace;
            else if (ch == '*') return asterisk;
            else if (ch == ']') return close_square_brace;
            else if (isalnum(ch) || ch == '_') return name_char;
            else if (ch == '-') return dash;
            else if (ch == '(') return open_paren;
            else if (ch == ')') return close_paren;
            else if (ch == '\\') return backslash;
            else if (ch == '|') return pipe;
            else if (ch == '/') return slash;
            else if (ch == ' ') return space;
            return none;
        }
    };

    class grammar
    {
        parser<terminal_traits> p_;

        std::string expect_name_chars_()
        {
            auto begin = p_.expect(name_char);
            while (p_.accept(name_char)) {}
            p_.uneat_spaces();
            return p_.substr(begin);
        }

    public:
        explicit grammar(const std::string& s)
            : p_(s)
        {}

        token root_node()
        {
            p_.expect(open_square_brace);
            p_.expect(asterisk);
            p_.expect(close_square_brace);
            return ascii_tree::root_node();
        }

        token named_node()
        {
            p_.expect(open_square_brace);
            auto name = expect_name_chars_();
            p_.expect(close_square_brace);
            return ascii_tree::named_node(std::move(name));
        }

        token edge_name()
        {
            p_.expect(open_paren);
            auto name = expect_name_chars_();
            p_.expect(close_paren);
            return ascii_tree::edge_name(std::move(name));
        }

        token ascending_edge_part()
        {
            p_.expect(slash);
            return ascii_tree::ascending_edge_part();
        }

        token descending_edge_part()
        {
            p_.expect(backslash);
            return ascii_tree::descending_edge_part();
        }

        token vertical_edge_part()
        {
            p_.expect(pipe);
            return ascii_tree::vertical_edge_part();
        }

        token horizontal_edge()
        {
            p_.expect(dash);
            while (p_.accept(dash)) {}
            p_.expect(open_paren);
            auto name = expect_name_chars_();
            p_.expect(close_paren);
            p_.expect(dash);
            while (p_.accept(dash)) {}
            return ascii_tree::horizontal_edge(std::move(name));
        }

        std::vector<token> tokens()
        {
            std::vector<token> tokens;
            while (p_.eat_spaces(), !p_.at_end())
            {
                auto pos = p_.save_position();

                if (p_.accept(open_square_brace))
                {
                    if (p_.accept(asterisk))
                    {
                        p_.restore_position(pos);
                        tokens.emplace_back(root_node());
                    }
                    else
                    {
                        p_.restore_position(pos);
                        tokens.emplace_back(named_node());
                    }
                }
                else if (p_.accept(dash))
                {
                    p_.restore_position(pos);
                    tokens.emplace_back(horizontal_edge());
                }
                else if (p_.accept(backslash))
                {
                    p_.restore_position(pos);
                    tokens.emplace_back(descending_edge_part());
                }
                else if (p_.accept(pipe))
                {
                    p_.restore_position(pos);
                    tokens.emplace_back(vertical_edge_part());
                }
                else if (p_.accept(slash))
                {
                    p_.restore_position(pos);
                    tokens.emplace_back(ascending_edge_part());
                }
                else if (p_.accept(open_paren))
                {
                    p_.restore_position(pos);
                    tokens.emplace_back(edge_name());
                }
                else
                {
                    p_.error();
                }
            }

            return tokens;
        }
    };

    inline std::vector<token> tokenize(const std::string& s)
    {
        return grammar(s).tokens();
    }
}

#endif // ASCII_TREE_GRAMMAR_H
