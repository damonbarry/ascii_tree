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

    typedef std::vector<std::string> grid_type;
    typedef std::shared_ptr<grid_type> grid_ptr;

    template<typename... Args>
    inline grid_ptr make_grid(Args&&... args)
    {
        return std::make_shared<grid_type>(std::forward<Args>(args)...);
    }

    class position
    {
        grid_ptr rows_;
        grid_type::const_iterator which_row_;
        std::string::const_iterator which_col_;

    public:
        position(grid_ptr grid, size_t row, size_t column) :
            rows_(grid),
            which_row_(rows_->cbegin() + row),
            which_col_(which_row_->cbegin() + column)
        {}

        position(std::string s, size_t pos) :
            position(make_grid(1, s), 0, pos)
        {}

        explicit position(size_t pos) :
            position(std::string(pos, ' '), pos)
        {}

        position() : position("", 0) {}

        position(const position& other) :
            rows_(other.rows_),
            which_row_(rows_->cbegin() + std::distance(other.rows_->cbegin(), other.which_row_)),
            which_col_(which_row_->cbegin() + std::distance(other.which_row_->cbegin(), other.which_col_))
        {}

        static position from(const position& other, std::ptrdiff_t row_offset, std::ptrdiff_t column_offset)
        {
            return position(
                other.rows_,
                std::distance(other.rows_->cbegin(), other.which_row_) + row_offset,
                std::distance(other.which_row_->cbegin(), other.which_col_) + column_offset);
        }

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
        typedef typename TerminalTraits::type terminal;

        grid_ptr rows_;
        position pos_;

        std::string::const_iterator accept_(terminal term)
        {
            ignore();
            if (at_line_end()) { return pos_.which_row_->end(); }

            terminal next_term = TerminalTraits::to_terminal(*pos_.which_col_);
            return (term == next_term) ? pos_.which_col_++ : pos_.which_row_->cend();
        }

    public:
        explicit parser(const std::string& s)
            : parser(s, 0)
        {}

        parser(const std::string& s, size_t pos) :
            rows_(make_grid(1, s)),
            pos_(rows_, 0, pos)
        {}

        parser(const grid_type& grid, size_t row, size_t column) :
            rows_(make_grid(grid)),
            pos_(rows_, row, column)
        {}

        parser(const grid_type& grid) :
            parser(grid, 0, 0)
        {}

        parser(std::initializer_list<std::string> grid) :
            parser(grid, 0, 0)
        {}

        parser(std::initializer_list<std::string> grid, size_t row, size_t column) :
            parser(grid_type(grid), row, column)
        {}

        parser(const parser& other) :
            rows_(other.rows_),
            pos_(other.pos_)
        {}

        void ignore()
        {
            if (at_line_end()) { return; }

            while (TerminalTraits::to_terminal(*pos_.which_col_) == TerminalTraits::ignore_me &&
                ++pos_.which_col_ != pos_.which_row_->cend())
            {}
        }

        void unignore()
        {
            while (!at_line_begin() &&
                TerminalTraits::to_terminal(*(pos_.which_col_ - 1)) == TerminalTraits::ignore_me)
            {
                --pos_.which_col_;
            }
        }

        void maybe_advance_row()
        {
            if (at_line_end() && pos_.which_row_ + 1 != pos_.rows_->cend())
            {
                ++pos_.which_row_;
                pos_.which_col_ = pos_.which_row_->cbegin();
            }
        }

        position current_position()
        {
            return position(pos_.rows_, std::distance(pos_.rows_->cbegin(), pos_.which_row_), std::distance(pos_.which_row_->cbegin(), pos_.which_col_));
        }

        position position_at(size_t row, size_t column)
        {
            return position(pos_.rows_, row, column);
        }

        bool at_line_begin()
        {
            return pos_.which_col_ == pos_.which_row_->cbegin();
        }

        bool at_line_end()
        {
            return pos_.which_col_ == pos_.which_row_->cend();
        }

        bool at_begin()
        {
            return pos_.which_row_ == pos_.rows_->cbegin() && at_line_begin();
        }

        bool at_end()
        {
            return pos_.which_row_ + 1 == pos_.rows_->cend() && at_line_end();
        }

        bool accept(terminal term)
        {
            return accept_(term) != pos_.which_row_->cend();
        }

        position expect(terminal term)
        {
            auto it = accept_(term);
            if (it == pos_.which_row_->cend())
            {
                throw parse_exception(*pos_.which_row_, std::distance(pos_.which_row_->cbegin(), pos_.which_col_));
            }

            return position(*pos_.which_row_, std::distance(pos_.which_row_->cbegin(), it));
        }

        std::string substring(const position& start)
        {
            return std::string(pos_.which_row_->cbegin() + std::distance(start.which_row_->cbegin(), start.which_col_), pos_.which_col_);
        }

        void error()
        {
            throw parse_exception(*pos_.which_row_, std::distance(pos_.which_row_->cbegin(), pos_.which_col_));
        }
    };

}

#endif // ASCII_TREE_PARSER_H