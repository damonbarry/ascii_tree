#if !defined(ASCII_TREE_POSITION_H)
#define ASCII_TREE_POSITION_H

#include "grid_types.hpp"

#include <cstddef>
#include <string>
#include <memory>

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
        position() :
            position(details::make_grid_vector_with_one_string_(""))
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
        position(details::grid_vector_ptr_ rows, std::string::difference_type pos) :
            rows_(rows),
            which_row_(rows_->cbegin()),
            which_col_((*which_row_)->cbegin() + pos)
        {}
        position(const std::string& s, std::string::difference_type pos) :
            position(details::make_grid_vector_with_one_string_(s), pos)
        {}
        position(std::string::difference_type pos) :
            position(details::make_grid_vector_with_one_string_(std::string(pos + 1, ' ')), pos)
        {}

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