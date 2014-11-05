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

    class position
    {
        std::shared_ptr<const std::string> s_;
        std::string::const_iterator it_;

        position(std::shared_ptr<const std::string>& s, std::string::const_iterator it) : s_(s), it_(it) {}

        template<class T>
        friend class parser;

        friend bool operator==(const position& lhs, const position& rhs)
        {
            return lhs.s_ == rhs.s_ &&
                lhs.it_ == rhs.it_;
        }

    public:
        std::wstring to_string() const
        {
            return std::wstring(L"position=") + 
                std::to_wstring(std::distance(s_->begin(), it_)) + L"/" + 
                std::to_wstring(s_->length()) + L" (" +
                (it_ == s_->end() ? L"<end>" : std::wstring(1, *it_)) + L")";
        }
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
            if (at_end()) { return s_->end(); }

            terminal next_term = TerminalTraits::to_terminal(*it_);
            return (term == next_term) ? it_++ : s_->end();
        }

    public:
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
            if (at_end()) { return; }
            while (TerminalTraits::to_terminal(*it_) == TerminalTraits::ignore_me && ++it_ != s_->end()) {}
        }

        void unignore()
        {
            while (!at_begin() &&
                TerminalTraits::to_terminal(*(it_ - 1)) == TerminalTraits::ignore_me)
            {
                --it_;
            }
        }

        position current_position()
        {
            return position(s_, it_);
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

            return position(s_, it);
        }

        std::string substring(position start)
        {
            return std::string(start.it_, it_);
        }

        void error()
        {
            throw parse_exception(*s_, std::distance(s_->begin(), it_));
        }
    };

}

#endif // ASCII_TREE_PARSER_H