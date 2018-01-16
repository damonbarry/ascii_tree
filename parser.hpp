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
    public:
        typedef std::vector<std::string> vector_type;
        typedef std::shared_ptr<vector_type> vector_ptr;

    private:
        vector_ptr rows_;
        vector_type::const_iterator which_row_;
        std::string::const_iterator which_col_;

    public:
        position(vector_type v, size_t row, size_t column) :
            rows_(std::make_shared<vector_type>(v)),
            which_row_(rows_->cbegin() + row),
            which_col_(which_row_->cbegin() + column)
        {}

        position(std::string s, size_t pos) :
            position(vector_type(1, s), 0, pos)
        {}

        position() : position("", 0) {}

        position(const position& other) :
            rows_(other.rows_),
            which_row_(rows_->cbegin() + std::distance(other.rows_->cbegin(), other.which_row_)),
            which_col_(which_row_->cbegin() + std::distance(other.which_row_->cbegin(), other.which_col_))
        {}

        template<class T>
        friend class parser;

        friend bool operator==(const position& lhs, const position& rhs)
        {
            return *lhs.rows_ == *rhs.rows_ &&
                std::distance(lhs.rows_->cbegin(), lhs.which_row_) == std::distance(rhs.rows_->cbegin(), rhs.which_row_) &&
                std::distance(lhs.which_row_->cbegin(), lhs.which_col_) == std::distance(rhs.which_row_->cbegin(), rhs.which_col_);
        }

        friend bool operator!=(const position& lhs, const position& rhs)
        {
            return !(lhs == rhs);
        }

        std::string to_string() const
        {
            // format: ch (row, column)

            std::string ch = which_col_ == which_row_->cend() ? "<end>" : std::string(1, *which_col_);
            size_t row = std::distance(rows_->cbegin(), which_row_);
            size_t column = std::distance(which_row_->begin(), which_col_);

            return ch + " (" + std::to_string(row) + ", " + std::to_string(column) + ")";
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
        std::string::const_iterator which_col_;

        std::string::const_iterator accept_(terminal term)
        {
            ignore();
            if (at_line_end()) { return which_row_->end(); }

            terminal next_term = TerminalTraits::to_terminal(*which_col_);
            return (term == next_term) ? which_col_++ : which_row_->end();
        }

    public:
        explicit parser(const std::string& s)
            : parser(s, 0)
        {}

        parser(const std::string& s, size_t pos) :
            rows_(vector_type(1, s)),
            which_row_(rows_.begin()),
            which_col_(which_row_->begin() + pos)
        {}

        parser(vector_type& v, size_t row, size_t column) :
            rows_(v),
            which_row_(rows_.begin() + row),
            which_col_(which_row_->begin() + column)
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
            which_col_(which_row_->begin() + column)
        {}

        parser(const parser& other) :
            rows_(other.rows_),
            which_row_(rows_.begin() + std::distance(other.rows_.begin(), other.which_row_)),
            which_col_(which_row_->begin() + std::distance(other.which_row_->begin(), other.which_col_))
        {}

        void ignore()
        {
            if (at_line_end()) { return; }

            while (TerminalTraits::to_terminal(*which_col_) == TerminalTraits::ignore_me &&
                ++which_col_ != which_row_->end())
            {}
        }

        void unignore()
        {
            while (!at_line_begin() &&
                TerminalTraits::to_terminal(*(which_col_ - 1)) == TerminalTraits::ignore_me)
            {
                --which_col_;
            }
        }

        void maybe_advance_row()
        {
            if (at_line_end() && which_row_ + 1 != rows_.end())
            {
                ++which_row_;
                which_col_ = which_row_->begin();
            }
        }

        position current_position()
        {
            return position(rows_, std::distance(rows_.cbegin(), which_row_), std::distance(which_row_->cbegin(), which_col_));
        }

        position position_at(size_t row, size_t column)
        {
            return position(rows_, row, column);
        }

        bool at_line_begin()
        {
            return which_col_ == which_row_->begin();
        }

        bool at_line_end()
        {
            return which_col_ == which_row_->end();
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
                throw parse_exception(*which_row_, std::distance(which_row_->begin(), which_col_));
            }

            return position(*which_row_, std::distance(which_row_->cbegin(), it));
        }

        std::string substring(const position& start)
        {
            return std::string(which_row_->cbegin() + std::distance(start.which_row_->cbegin(), start.which_col_), which_col_);
        }

        void error()
        {
            throw parse_exception(*which_row_, std::distance(which_row_->begin(), which_col_));
        }
    };

}

#endif // ASCII_TREE_PARSER_H