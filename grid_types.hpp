#if !defined(ASCII_TREE_GRID_TYPES_H)
#define ASCII_TREE_GRID_TYPES_H

#include "position.hpp"

#include <memory>
#include <string>
#include <vector>

namespace ascii_tree
{
    namespace details
    {
        typedef std::shared_ptr<const std::string> grid_row_type_;
        typedef std::vector<grid_row_type_> grid_vector_type_;

        template<typename T>
        inline grid_row_type_ make_grid_row_(T&& s)
        {
            return std::make_shared<const std::string>(std::forward<T>(s));
        }
    }

    typedef details::grid_vector_type_::const_iterator grid_row_iterator;
    typedef std::string::const_iterator grid_col_iterator;
}

#endif // ASCII_TREE_GRID_TYPES_H