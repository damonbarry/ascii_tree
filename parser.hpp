#if !defined(ASCII_TREE_PARSER_H)
#define ASCII_TREE_PARSER_H

#include <string>
#include <iterator>

namespace ascii_tree
{

    struct ascii_tree_parse_exception
    {
        const std::string s;
        const size_t pos;

        ascii_tree_parse_exception(const std::string& s, size_t pos) : s(s), pos(pos) {}
    };

    template<class TerminalTraits>
    class parser
    {
        typedef typename TerminalTraits::type terminal;

        const std::string s_;
        std::string::const_iterator it_;

        std::string::const_iterator accept_(terminal term)
        {
            eat_spaces();
            if (it_ == s_.end()) { return s_.end(); }

            terminal next_term = TerminalTraits::to_terminal(*it_);
            return (term == next_term) ? it_++ : s_.end();
        }

    public:
        typedef std::string::const_iterator position;

        explicit parser(const std::string& s)
            : s_(s), it_(s_.begin())
        {}

        std::string::const_iterator eat_spaces()
        {
            if (it_ == s_.end()) { return s_.end(); }
            while (TerminalTraits::to_terminal(*it_) == terminal::space && ++it_ != s_.end()) {}
            return it_;
        }

        void uneat_spaces()
        {
            while (TerminalTraits::to_terminal(*(it_ - 1)) == terminal::space) { --it_; }
        }

        position save_position()
        {
            return it_;
        }

        void restore_position(const position& pos)
        {
            it_ = pos;
        }

        bool at_end()
        {
            return it_ == s_.end();
        }

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

        std::string substr(std::string::const_iterator start)
        {
            return std::string(start, it_);
        }

        void error()
        {
            throw ascii_tree_parse_exception(s_, std::distance(s_.begin(), it_));
        }
    };

}

#endif // ASCII_TREE_PARSER_H