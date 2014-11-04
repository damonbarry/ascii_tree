#if !defined(ASCII_TREE_PARSER_H)
#define ASCII_TREE_PARSER_H

#include <memory>
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

        std::shared_ptr<const std::string> s_;
        std::string::const_iterator it_;

        std::string::const_iterator accept_(terminal term)
        {
            ignore();
            if (it_ == s_->end()) { return s_->end(); }

            terminal next_term = TerminalTraits::to_terminal(*it_);
            return (term == next_term) ? it_++ : s_->end();
        }

    public:
        typedef std::string::const_iterator position;

        explicit parser(const std::string& s)
            : parser(s, 0)
        {}

        parser(const std::string& s, size_t init_pos)
            : s_(std::make_shared<const std::string>(s)), it_(s_->begin() + init_pos)
        {}

        parser(const parser& other)
            : s_(other.s_), it_(other.it_)
        {}

        void ignore()
        {
            if (it_ == s_->end()) { return; }
            while (TerminalTraits::to_terminal(*it_) == TerminalTraits::ignore_me && ++it_ != s_->end()) {}
        }

        void unignore()
        {
            while (it_ != s_->begin() &&
                TerminalTraits::to_terminal(*(it_ - 1)) == TerminalTraits::ignore_me)
            {
                --it_;
            }
        }

        position current_position()
        {
            return it_;
        }

        bool at_begin()
        {
            return it_ == s_->begin();
        }

        bool at_end()
        {
            return it_ == s_->end();
        }

        bool accept(terminal term)
        {
            return accept_(term) != s_->end();
        }

        position expect(terminal term)
        {
            auto it = accept_(term);
            if (it == s_->end())
            {
                throw parse_exception(*s_, std::distance(s_->begin(), it_));
            }

            return it;
        }

        std::string substring(position start)
        {
            return std::string(start, it_);
        }

        void error()
        {
            throw parse_exception(*s_, std::distance(s_->begin(), it_));
        }
    };

}

#endif // ASCII_TREE_PARSER_H