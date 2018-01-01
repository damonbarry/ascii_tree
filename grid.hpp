#if !defined(ASCII_TREE_GRID_H)
#define ASCII_TREE_GRID_H

#include <memory>
#include <string>
#include <vector>

namespace ascii_tree
{
    namespace details
    {
        typedef std::vector<std::shared_ptr<const std::string>> vector_type;

        template<typename T>
        vector_type make_vector_of_shared_ptrs_(T range)
        {
            vector_type vec;
            for (const auto& elem : range)
            {
                vec.emplace_back(std::make_shared<const std::string>(elem));
            }

            return vec;
        }
    }

    class grid
    {
    public:
        details::vector_type rows_;
        details::vector_type::iterator which_row_;
        std::string::const_iterator which_char_;

        explicit grid(const std::string& s)
            : grid(s, 0)
        {}

        grid(const std::string& s, size_t init_pos) :
            rows_(details::vector_type(1, std::make_shared<const std::string>(s))),
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
            which_row_(rows_.begin() + std::distance<details::vector_type::const_iterator>(other.rows_.begin(), other.which_row_)),
            which_char_((*which_row_)->begin() + std::distance((*other.which_row_)->begin(), other.which_char_))
        {}
    };

}

#endif // ASCII_TREE_GRID_H