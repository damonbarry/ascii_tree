#if !defined(ASCII_TREE_PARSER_H)
#define ASCII_TREE_PARSER_H

#include <string>
#include <iterator>

namespace ascii_tree
{

    struct parse_exception
    {
        const std::string s;
        const size_t pos;

        parse_exception(const std::string& s, size_t pos) : s(s), pos(pos) {}
    };

    template<class TerminalTraits>
    class parser
    {
        typedef typename TerminalTraits::type terminal;

        const std::string s_;
        std::string::const_iterator it_;

        std::string::const_iterator accept_(terminal term)
        {
            ignore();
            if (it_ == s_.end()) { return s_.end(); }

            terminal next_term = TerminalTraits::to_terminal(*it_);
            return (term == next_term) ? it_++ : s_.end();
        }

    public:
        typedef std::string::const_iterator position;

        explicit parser(const std::string& s)
            : s_(s), it_(s_.begin())
        {}

        void ignore()
        {
            if (it_ == s_.end()) { return; }
            while (TerminalTraits::to_terminal(*it_) == TerminalTraits::ignore_me && ++it_ != s_.end()) {}
        }

        void unignore()
        {
            if (it_ == s_.begin()) { return; }
            while (TerminalTraits::to_terminal(*(it_ - 1)) == TerminalTraits::ignore_me) { --it_; }
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

        position expect(terminal term)
        {
            auto it = accept_(term);
            if (it == s_.end())
            {
                throw parse_exception(s_, std::distance(s_.begin(), it_));
            }

            return it;
        }

        std::string substr(position start)
        {
            return std::string(start, it_);
        }

        void error()
        {
            throw parse_exception(s_, std::distance(s_.begin(), it_));
        }
    };

}

#endif // ASCII_TREE_PARSER_H