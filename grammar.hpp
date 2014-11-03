#if !defined(ASCII_TREE_GRAMMAR_H)
#define ASCII_TREE_GRAMMAR_H

#include <iterator>
#include <utility>
#include <cctype>
#include <string>
#include <vector>

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

    inline terminal to_terminal(char ch)
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

    struct ascii_tree_parse_exception
    {
        const std::string s;
        const size_t pos;

        ascii_tree_parse_exception(const std::string& s, size_t pos) : s(s), pos(pos) {}
    };

    class grammar
    {
        const std::string s_;
        std::string::const_iterator it_;

        std::string::const_iterator accept_(terminal term)
        {
            if (it_ == s_.end()) { return s_.end(); }

            terminal next_term = to_terminal(*it_);
            while (next_term == space)
            {
                if (++it_ == s_.end()) { return s_.end(); }
                next_term = to_terminal(*it_);
            }

            return (term == next_term) ? it_++ : s_.end();
        }

    public:
        explicit grammar(const std::string& s)
            : s_(s), it_(s_.begin())
        {}

        bool accept(terminal term)
        {
            return accept_(term) != s_.end();
        }

        std::string::const_iterator expect(terminal term)
        {
            auto it = accept_(term);
            if (it == s_.end())
            {
                throw ascii_tree_parse_exception(s_, std::distance(s_.begin(), it_));
            }

            return it;
        }

        token root_node()
        {
            expect(open_square_brace);
            expect(asterisk);
            expect(close_square_brace);
            return ascii_tree::root_node();
        }

        token named_node()
        {
            expect(open_square_brace);
            auto begin = expect(name_char);
            while (accept(name_char)) {}
            auto end = it_;
            expect(close_square_brace);
            return ascii_tree::named_node(std::string(begin, end));
        }

        token edge_name()
        {
            expect(open_paren);
            auto begin = expect(name_char);
            while (accept(name_char)) {}
            auto end = it_;
            expect(close_paren);
            return ascii_tree::edge_name(std::string(begin, end));
        }

        token ascending_edge_part()
        {
            expect(slash);
            return ascii_tree::ascending_edge_part();
        }

        token descending_edge_part()
        {
            expect(backslash);
            return ascii_tree::descending_edge_part();
        }

        token vertical_edge_part()
        {
            expect(pipe);
            return ascii_tree::vertical_edge_part();
        }

        void horizontal_edge()
        {
            expect(dash);
            while (accept(dash)) {}
            expect(open_paren);
            expect(name_char);
            while (accept(name_char)) {}
            expect(close_paren);
            expect(dash);
            while (accept(dash)) {}
        }
    };

    inline std::vector<token> tokenize(const std::string& s)
    {
        std::vector<token> tokens;
        terminal prev_ch = none, prev_prev_ch = none;
        size_t marker = 0, marked_length = 0;

        for (size_t i = 0; i < s.size(); ++i)
        {
            auto term = to_terminal(s[i]);

            if (term == open_square_brace)
            {
                if (prev_ch == open_square_brace)
                {
                    throw ascii_tree_parse_exception(s, i);
                }

                prev_ch = term;
            }
            else if (term == close_square_brace)
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

                prev_ch = term;
            }
            else if (term == asterisk)
            {
                prev_ch = term;
            }
            else if (term == dash)
            {
                if (prev_ch == close_paren)
                {
                    tokens.emplace_back(horizontal_edge(s.substr(marker, marked_length)));
                }

                prev_ch = term;
            }
            else if (term == slash)
            {
                tokens.emplace_back(ascending_edge_part());
                prev_ch = term;
            }
            else if (term == backslash)
            {
                tokens.emplace_back(descending_edge_part());
                prev_ch = term;
            }
            else if (term == pipe)
            {
                tokens.emplace_back(vertical_edge_part());
                prev_ch = term;
            }
            else if (term == open_paren)
            {
                if (prev_ch == dash) { prev_prev_ch = dash; }
                prev_ch = term;
            }
            else if (term == close_paren)
            {
                if (prev_prev_ch == none)
                {
                    tokens.emplace_back(edge_name(s.substr(marker, marked_length)));
                }

                prev_ch = term;
            }
            else if (term == name_char)
            {
                if (prev_ch != name_char)
                {
                    marker = i;
                    marked_length = 0;
                }

                prev_ch = term;
                ++marked_length;
            }
            else if (term != space)
            {
                throw ascii_tree_parse_exception(s, i);
            }
        }

        return tokens;
    }
}

#endif // ASCII_TREE_GRAMMAR_H
