#if !defined(ASCII_TREE_POSITION_H)
#define ASCII_TREE_POSITION_H

#include "grid_types.hpp"

#include <cstddef>
#include <string>
#include <memory>

namespace ascii_tree
{

    class position
    {
        details::grid_row_type_ row_;
        grid_col_iterator col_;

    public:
        position() : row_(details::make_grid_row_("")), col_(row_->cbegin()) {}
        position(const details::grid_row_type_& s, grid_col_iterator it) : row_(s), col_(it) {}
        position(const std::string& s, std::string::difference_type pos) :
            row_(details::make_grid_row_(std::string(s))),
            col_(row_->cbegin() + pos)
        {}
        position(std::string::difference_type pos) :
            row_(details::make_grid_row_("")),
            col_(row_->cbegin() + pos)
        {}

        static position from(const position& other, std::ptrdiff_t horizontal_offset)
        {
            return position(other.row_, other.col_ + horizontal_offset);
        }

        template<class T>
        friend class parser;

        friend bool operator==(const position& lhs, const position& rhs)
        {
            return *lhs.row_ == *rhs.row_ &&
                std::distance(lhs.row_->cbegin(), lhs.col_) == std::distance(rhs.row_->cbegin(), rhs.col_);
        }

        std::string to_string() const
        {
            return std::string("position=") + 
                std::to_string(std::distance(row_->begin(), col_)) + "/" + 
                std::to_string(row_->length()) + " (" +
                (col_ == row_->end() ? "<end>" : std::string(1, *col_)) + ")";
        }
    };

}

#endif // ASCII_TREE_POSITION_H