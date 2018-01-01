#if !defined(ASCII_TREE_PARSER_H)
#define ASCII_TREE_PARSER_H

#include "position.hpp"
#include "grid.hpp"

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

    template<class TerminalTraits>
    class parser
    {
        grid grid_;

        std::string::const_iterator accept_(typename TerminalTraits::type term)
        {
            ignore();
            if (at_line_end()) { return (*grid_.which_row_)->end(); }

            typename TerminalTraits::type next_term = TerminalTraits::to_terminal(*grid_.which_char_);
            return (term == next_term) ? grid_.which_char_++ : (*grid_.which_row_)->end();
        }

    public:
        explicit parser(const std::string& s)
            : parser(s, 0)
        {}

        parser(const std::string& s, size_t init_pos) : grid_(s, init_pos) {}
        parser(const std::vector<std::string>& v) : parser(v, 0, 0) {}
        parser(const std::vector<std::string>& v, size_t init_row, size_t init_pos) : grid_(v, init_row, init_pos) {}
        parser(std::initializer_list<std::string> l) : parser(l, 0, 0) {}
        parser(std::initializer_list<std::string> l, size_t init_row, size_t init_pos) : grid_(l, init_row, init_pos) {}
        parser(const parser& other) : grid_(other.grid_) {}

        void ignore()
        {
            if (at_line_end()) { return; }

            while (TerminalTraits::to_terminal(*grid_.which_char_) == TerminalTraits::ignore_me &&
                ++grid_.which_char_ != (*grid_.which_row_)->end())
            {}
        }

        void unignore()
        {
            while (!at_line_begin() &&
                TerminalTraits::to_terminal(*(grid_.which_char_ - 1)) == TerminalTraits::ignore_me)
            {
                --grid_.which_char_;
            }
        }

        void maybe_advance_row()
        {
            if (at_line_end() && grid_.which_row_ + 1 != grid_.rows_.end())
            {
                ++grid_.which_row_;
                grid_.which_char_ = (*grid_.which_row_)->begin();
            }
        }

        position current_position()
        {
            return grid_.current_position();
        }
        
        position position_at(size_t row, size_t column)
        {
            return grid_.position_at(row, column);
        }

        bool at_line_begin()
        {
            return grid_.which_char_ == (*grid_.which_row_)->begin();
        }

        bool at_line_end()
        {
            return grid_.which_char_ == (*grid_.which_row_)->end();
        }

        bool at_begin()
        {
            return grid_.which_row_ == grid_.rows_.begin() && at_line_begin();
        }

        bool at_end()
        {
            return grid_.which_row_ + 1 == grid_.rows_.end() && at_line_end();
        }

        bool accept(typename TerminalTraits::type term)
        {
            return accept_(term) != (*grid_.which_row_)->end();
        }

        position expect(typename TerminalTraits::type term)
        {
            auto it = accept_(term);
            if (it == (*grid_.which_row_)->end())
            {
                throw parse_exception(**grid_.which_row_, std::distance((*grid_.which_row_)->begin(), grid_.which_char_));
            }

            return position(*grid_.which_row_, it);
        }

        std::string substring(position start)
        {
            return std::string(start.it_, grid_.which_char_);
        }

        void error()
        {
            throw parse_exception(**grid_.which_row_, std::distance((*grid_.which_row_)->begin(), grid_.which_char_));
        }
    };

}

#endif // ASCII_TREE_PARSER_H