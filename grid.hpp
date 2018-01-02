#if !defined(ASCII_TREE_GRID_H)
#define ASCII_TREE_GRID_H

#include "grid_types.hpp"
#include "position.hpp"

#include <memory>
#include <string>
#include <vector>

namespace ascii_tree
{
    namespace details
    {
        template<typename T>
        grid_vector_type_ make_vector_of_shared_ptrs_(T range)
        {
            grid_vector_type_ vec;
            for (const auto& elem : range)
            {
                vec.emplace_back(details::make_grid_row_(elem));
            }

            return vec;
        }
    }

    class grid
    {
    public:
        details::grid_vector_type_ rows_;
        grid_row_iterator which_row_;
        grid_col_iterator which_char_;

        explicit grid(const std::string& s)
            : grid(s, 0)
        {}

        grid(const std::string& s, size_t init_pos) :
            rows_(details::grid_vector_type_(1, details::make_grid_row_(s))),
            which_row_(rows_.begin()),
            which_char_((*which_row_)->begin() + init_pos)
        {}

        grid(const std::vector<std::string>& v) :
            grid(v, 0, 0)
        {}

        grid(const std::vector<std::string>& v, size_t init_row, size_t init_pos) :
            rows_(details::make_vector_of_shared_ptrs_(v)),
            which_row_(rows_.begin() + init_row),
            which_char_((*which_row_)->begin() + init_pos)
        {}

        grid(std::initializer_list<std::string> l) :
            grid(l, 0, 0)
        {}

        grid(std::initializer_list<std::string> l, size_t init_row, size_t init_pos) :
            rows_(details::make_vector_of_shared_ptrs_(l)),
            which_row_(rows_.begin() + init_row),
            which_char_((*which_row_)->begin() + init_pos)
        {}

        grid(const grid& other) :
            rows_(other.rows_),
            which_row_(rows_.begin() + std::distance<grid_row_iterator>(other.rows_.begin(), other.which_row_)),
            which_char_((*which_row_)->begin() + std::distance((*other.which_row_)->begin(), other.which_char_))
        {}

        position current_position()
        {
            return position(*which_row_, which_char_);
        }

        position position_at(size_t row, size_t column)
        {
            auto row_ptr = *(rows_.begin() + row);
            return position(row_ptr, row_ptr->begin() + column);
        }
    };

}

#endif // ASCII_TREE_GRID_H