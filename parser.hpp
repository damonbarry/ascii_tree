#if !defined(ASCII_TREE_PARSER_H)
#define ASCII_TREE_PARSER_H

#include <memory>
#include <string>
#include <vector>
#include <iterator>
#include <initializer_list>

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
        typedef std::vector<std::shared_ptr<const std::string>> vector_type;

        vector_type rows_;
        vector_type::iterator which_row_;
        std::string::const_iterator which_char_;

        vector_type make_vector_of_shared_ptrs_(std::initializer_list<std::string> l)
        {
            vector_type vec;
            for (const auto& elem : l)
            {
                vec.emplace_back(make_shared<const std::string>(elem));
            }

            return vec;
        }

        std::string::const_iterator accept_(terminal term)
        {
            ignore();
            if (at_end()) { return (*which_row_)->end(); }

            terminal next_term = TerminalTraits::to_terminal(*which_char_);
            return (term == next_term) ? which_char_++ : (*which_row_)->end();
        }

        bool at_line_begin_()
        {
            return which_char_ == (*which_row_)->begin();
        }

        bool at_line_end_()
        {
            return which_char_ == (*which_row_)->end();
        }

    public:
        explicit parser(const std::string& s)
            : parser(s, 0)
        {}

        parser(const std::string& s, size_t init_pos) :
            rows_(vector_type(1, std::make_shared<const std::string>(s))),
            which_row_(rows_.begin()),
            which_char_((*which_row_)->begin() + init_pos)
        {}

        parser(std::initializer_list<std::string> l) :
            rows_(make_vector_of_shared_ptrs_(l)),
            which_row_(rows_.begin()),
            which_char_((*which_row_)->begin())
        {}

        parser(const parser& other) :
            rows_(other.rows_),
            which_row_(rows_.begin() + std::distance<vector_type::const_iterator>(other.rows_.begin(), other.which_row_)),
            which_char_((*which_row_)->begin() + std::distance((*other.which_row_)->begin(), other.which_char_))
        {}

        void ignore()
        {
            while (which_row_ != rows_.end())
            {
                if (at_line_end_())
                {
                    if (which_row_ + 1 == rows_.end()) { return; }
                    ++which_row_;
                    which_char_ = (*which_row_)->begin();
                    if (at_line_end_()) { return; }
                }
                while (TerminalTraits::to_terminal(*which_char_) == TerminalTraits::ignore_me &&
                    ++which_char_ != (*which_row_)->end())
                {}
                if (!at_line_end_()) { return; }
            }
        }

        void unignore()
        {
            while (!at_begin() &&
                TerminalTraits::to_terminal(*(which_char_ - 1)) == TerminalTraits::ignore_me)
            {
                --which_char_;
            }
        }

        position current_position()
        {
            return position(*which_row_, which_char_);
        }

        bool at_begin()
        {
            return which_row_ == rows_.begin() && at_line_begin_();
        }

        bool at_end()
        {
            return which_row_ + 1 == rows_.end() && at_line_end_();
        }

        bool accept(terminal term)
        {
            return accept_(term) != (*which_row_)->end();
        }

        position expect(terminal term)
        {
            auto it = accept_(term);
            if (it == (*which_row_)->end())
            {
                throw parse_exception(**which_row_, std::distance((*which_row_)->begin(), which_char_));
            }

            return position(*which_row_, it);
        }

        std::string substring(position start)
        {
            return std::string(start.it_, which_char_);
        }

        void error()
        {
            throw parse_exception(**which_row_, std::distance((*which_row_)->begin(), which_char_));
        }
    };

}

#endif // ASCII_TREE_PARSER_H