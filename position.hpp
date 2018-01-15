#if !defined(ASCII_TREE_POSITION_H)
#define ASCII_TREE_POSITION_H

#include "grid_types.hpp"

#include <stdexcept>
#include <cstddef>
#include <memory>
#include <string>

namespace ascii_tree
{
    namespace details
    {

        template<typename... T>
        inline grid_vector_ptr_ make_grid_vector_with_one_string_(const std::string& s)
        {
            return make_grid_vector_(1, make_grid_row_(s));
        }

    }

    class position
    {
        details::grid_vector_ptr_ rows_;
        grid_row_iterator which_row_;
        grid_col_iterator which_col_;

    public:
        position(size_t width, size_t height) :
            rows_(details::make_grid_vector_(details::make_grid_row_(height, std::string(width, ' ')))),
            which_row_(rows_->cbegin()),
            which_col_((*which_row_)->cbegin())
        {}
        position() :
            position(1, 1)
        {}
        explicit position(details::grid_vector_ptr_ rows) :
            rows_(rows),
            which_row_(rows_->cbegin()),
            which_col_((*which_row_)->cbegin())
        {}
        position(details::grid_vector_ptr_ rows, grid_row_iterator row, grid_col_iterator col) :
            rows_(rows),
            which_row_(rows_->cbegin() + std::distance(rows->cbegin(), row)),
            which_col_((*which_row_)->cbegin() + std::distance((*row)->cbegin(), col))
        {}
        position(details::grid_vector_ptr_ rows, size_t y, size_t x) :
            rows_(rows),
            which_row_(rows_->cbegin() + y),
            which_col_((*which_row_)->cbegin() + x)
        {}
        // position(details::grid_vector_ptr_ rows, std::string::difference_type pos) :
        //     rows_(rows),
        //     which_row_(rows_->cbegin()),
        //     which_col_((*which_row_)->cbegin() + pos)
        // {}
        position(const std::string& s, size_t pos) :
            position(details::make_grid_vector_with_one_string_(s), 0, pos)
        {}
        position(size_t pos) :
            position(details::make_grid_vector_with_one_string_(std::string(pos + 1, ' ')), 0, pos)
        {}
        position(const position& other, std::ptrdiff_t offset_x, std::ptrdiff_t offset_y) :
            rows_(other.rows_),
            which_row_(other.which_row_ + offset_y),
            which_col_(other.which_col_ + offset_x)
        {
            std::ptrdiff_t x = std::distance((*other.which_row_)->cbegin(), other.which_col_) + offset_x;
            std::ptrdiff_t y = std::distance(other.rows_->cbegin(), other.which_row_) + offset_y;

            if (x < 0 || x >= std::distance((*other.which_row_)->cbegin(), (*other.which_row_)->cend()) ||
                y < 0 || y >= std::distance(other.rows_->cbegin(), other.rows_->cend()))
            {
                throw std::out_of_range("position out of range");
            }
        }

        static position from(const position& other, std::ptrdiff_t horizontal_offset)
        {
            return position(other.rows_, other.which_row_, other.which_col_ + horizontal_offset);
        }

        template<class T>
        friend class parser;

        friend bool operator==(const position& lhs, const position& rhs)
        {
            const std::string& lhstr = **lhs.which_row_;
            const std::string& rhstr = **rhs.which_row_;
            return lhstr == rhstr &&
                std::distance(lhstr.cbegin(), lhs.which_col_) == std::distance(rhstr.cbegin(), rhs.which_col_);
        }

        std::string to_string() const
        {
            // '[' {x}
            return
                "'" + (which_col_ == (*which_row_)->end() ? "<end>" : std::string(1, *which_col_)) +
                "' {" + std::to_string(std::distance((*which_row_)->cbegin(), which_col_)) +
                "}";
        }
    };

}

#endif // ASCII_TREE_POSITION_H