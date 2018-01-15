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
        std::string s_;
        std::string::const_iterator it_;

    public:
        position() : s_(""), it_(s_.cbegin()) {}
        position(std::string s, size_t pos) : s_(s), it_(s_.cbegin() + pos) {}
        position(const position& other) : s_(other.s_), it_(s_.cbegin() + std::distance(other.s_.cbegin(), other.it_)) {}

        template<class T>
        friend class parser;

        friend bool operator==(const position& lhs, const position& rhs)
        {
            return lhs.s_ == rhs.s_ &&
                std::distance(lhs.s_.cbegin(), lhs.it_) == std::distance(rhs.s_.cbegin(), rhs.it_);
        }

        std::string to_string() const
        {
            return (it_ == s_.cend() ? "<end>" : std::string(1, *it_)) + " [" +
                std::to_string(std::distance(s_.begin(), it_)) + "]";
        }
    };

    template<class TerminalTraits>
    class parser
    {
    public:
        typedef std::vector<std::string> vector_type;

    private:
        typedef typename TerminalTraits::type terminal;

        vector_type rows_;
        vector_type::const_iterator which_row_;
        std::string::const_iterator which_char_;

        std::string::const_iterator accept_(terminal term)
        {
            ignore();
            if (at_line_end()) { return which_row_->end(); }

            terminal next_term = TerminalTraits::to_terminal(*which_char_);
            return (term == next_term) ? which_char_++ : which_row_->end();
        }

    public:
        explicit parser(const std::string& s)
            : parser(s, 0)
        {}

        parser(const std::string& s, size_t init_pos) :
            rows_(vector_type(1, s)),
            which_row_(rows_.begin()),
            which_char_(which_row_->begin() + init_pos)
        {}

        parser(vector_type& v, size_t row, size_t column) :
            rows_(v),
            which_row_(rows_.begin() + row),
            which_char_(which_row_->begin() + column)
        {}

        parser(vector_type& v) :
            parser(v, 0, 0)
        {}

        parser(std::initializer_list<std::string> init) :
            parser(init, 0, 0)
        {}

        parser(std::initializer_list<std::string> init, size_t row, size_t column) :
            rows_(init),
            which_row_(rows_.begin() + row),
            which_char_(which_row_->begin() + column)
        {}

        parser(const parser& other) :
            rows_(other.rows_),
            which_row_(rows_.begin() + std::distance(other.rows_.begin(), other.which_row_)),
            which_char_(which_row_->begin() + std::distance(other.which_row_->begin(), other.which_char_))
        {}

        void ignore()
        {
            if (at_line_end()) { return; }

            while (TerminalTraits::to_terminal(*which_char_) == TerminalTraits::ignore_me &&
                ++which_char_ != which_row_->end())
            {}
        }

        void unignore()
        {
            while (!at_line_begin() &&
                TerminalTraits::to_terminal(*(which_char_ - 1)) == TerminalTraits::ignore_me)
            {
                --which_char_;
            }
        }

        void maybe_advance_row()
        {
            if (at_line_end() && which_row_ + 1 != rows_.end())
            {
                ++which_row_;
                which_char_ = which_row_->begin();
            }
        }

        position current_position()
        {
            return position(*which_row_, std::distance(which_row_->cbegin(), which_char_));
        }

        position position_at(size_t row, size_t column)
        {
            std::string& str = *(rows_.begin() + row);
            return position(str, column);
        }

        bool at_line_begin()
        {
            return which_char_ == which_row_->begin();
        }

        bool at_line_end()
        {
            return which_char_ == which_row_->end();
        }

        bool at_begin()
        {
            return which_row_ == rows_.begin() && at_line_begin();
        }

        bool at_end()
        {
            return which_row_ + 1 == rows_.end() && at_line_end();
        }

        bool accept(terminal term)
        {
            return accept_(term) != which_row_->end();
        }

        position expect(terminal term)
        {
            auto it = accept_(term);
            if (it == which_row_->end())
            {
                throw parse_exception(*which_row_, std::distance(which_row_->begin(), which_char_));
            }

            return position(*which_row_, std::distance(which_row_->cbegin(), it));
        }

        std::string substring(const position& start)
        {
            return std::string(which_row_->cbegin() + std::distance(start.s_.cbegin(), start.it_), which_char_);
        }

        void error()
        {
            throw parse_exception(*which_row_, std::distance(which_row_->begin(), which_char_));
        }
    };

}

#endif // ASCII_TREE_PARSER_H