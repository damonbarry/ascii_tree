#if !defined(ASCII_TREE_GRAMMAR_H)
#define ASCII_TREE_GRAMMAR_H

#include <initializer_list>
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

horizontal-edge:        edge-chars edge-name edge-chars

descending-edge-part    '\'

vertical-edge-part      '|'

ascending-edge-part     '/'

edge-name:              '(' name-chars ')'

name-chars:             name-char
                        name-chars name-char

name-char:              alnum
                        '_'

alnum:                  one of: [A-Za-z0-9]

edge-chars:             '-'
                        edge-chars '-'

#endif

namespace ascii_tree
{
    struct token
    {
        enum toktype { root_node, named_node, edge_name, horizontal_edge, ascending_edge_part, descending_edge_part, vertical_edge_part };
        toktype type;
        std::string name;
        position position;

        token(toktype type, std::string&& name) : type(type), name(std::move(name)), position() {}
        token(toktype type, std::string&& name, const class position& pos) : type(type), name(std::move(name)), position(pos) {}
    };

    inline bool operator==(const token& lhs, const token& rhs)
    {
        return lhs.type == rhs.type
            && lhs.name == rhs.name;
    }

    inline token root_node(position p = position()) { return token(token::root_node, "", p); }
    inline token named_node(std::string&& name, position p = position()) { return token(token::named_node, std::move(name), p); }
    inline token edge_name(std::string&& name, position p = position()) { return token(token::edge_name, std::move(name), p); }
    inline token horizontal_edge(std::string&& name, position p = position()) { return token(token::horizontal_edge, std::move(name), p); }
    inline token ascending_edge_part(position p = position()) { return token(token::ascending_edge_part, "", p); }
    inline token descending_edge_part(position p = position()) { return token(token::descending_edge_part, "", p); }
    inline token vertical_edge_part(position p = position()) { return token(token::vertical_edge_part, "", p); }

    enum terminal
    {
        none, open_square_brace, close_square_brace, asterisk, dash,
        open_paren, close_paren, name_char, slash, backslash, pipe, space
    };

    struct terminal_traits
    {
        typedef terminal type;

        static const type ignore_me = space;

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
            p_.unignore(); // strip trailing spaces
            return p_.substring(begin);
        }

    public:
        explicit grammar(const std::string& s) : p_(s) {}
        grammar(std::vector<std::string>& v) : p_(v) {}
        grammar(std::initializer_list<std::string> l) : p_(l) {}

        token root_node()
        {
            auto begin = p_.expect(open_square_brace);
            p_.expect(asterisk);
            p_.expect(close_square_brace);
            return ascii_tree::root_node(begin);
        }

        token named_node()
        {
            auto begin = p_.expect(open_square_brace);
            auto name = expect_name_chars_();
            p_.expect(close_square_brace);
            return ascii_tree::named_node(std::move(name), begin);
        }

        token edge_name()
        {
            auto begin = p_.expect(open_paren);
            auto name = expect_name_chars_();
            p_.expect(close_paren);
            return ascii_tree::edge_name(std::move(name), begin);
        }

        token ascending_edge_part()
        {
            auto begin = p_.expect(slash);
            return ascii_tree::ascending_edge_part(begin);
        }

        token descending_edge_part()
        {
            auto begin = p_.expect(backslash);
            return ascii_tree::descending_edge_part(begin);
        }

        token vertical_edge_part()
        {
            auto begin = p_.expect(pipe);
            return ascii_tree::vertical_edge_part(begin);
        }

        token horizontal_edge()
        {
            auto begin = p_.expect(dash);
            while (p_.accept(dash)) {}
            p_.expect(open_paren);
            auto name = expect_name_chars_();
            p_.expect(close_paren);
            p_.expect(dash);
            while (p_.accept(dash)) {}
            return ascii_tree::horizontal_edge(std::move(name), begin);
        }

        std::vector<token> tokens()
        {
            std::vector<token> tokens;
            while (p_.maybe_advance_row(), !p_.at_end())
            {
                auto peek = p_;

                if (peek.accept(open_square_brace))
                {
                    if (peek.accept(asterisk))
                    {
                        tokens.emplace_back(root_node());
                    }
                    else
                    {
                        tokens.emplace_back(named_node());
                    }
                }
                else if (peek.accept(dash))
                {
                    tokens.emplace_back(horizontal_edge());
                }
                else if (peek.accept(backslash))
                {
                    tokens.emplace_back(descending_edge_part());
                }
                else if (peek.accept(pipe))
                {
                    tokens.emplace_back(vertical_edge_part());
                }
                else if (peek.accept(slash))
                {
                    tokens.emplace_back(ascending_edge_part());
                }
                else if (peek.accept(open_paren))
                {
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
}

#endif // ASCII_TREE_GRAMMAR_H
