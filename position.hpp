#if !defined(ASCII_TREE_POSITION_H)
#define ASCII_TREE_POSITION_H

#include <cstddef>
#include <string>
#include <memory>

namespace ascii_tree
{

    class position
    {
        std::shared_ptr<const std::string> s_;
        std::string::const_iterator it_;

    public:
        position() : s_(std::make_shared<const std::string>("")), it_(s_->cbegin()) {}
        position(const std::shared_ptr<const std::string>& s, std::string::const_iterator it) : s_(s), it_(it) {}
        position(const std::string& s, std::string::difference_type pos) :
            s_(std::make_shared<const std::string>(std::string(s))),
            it_(s_->cbegin() + pos)
        {}
        position(std::string::difference_type pos) :
            s_(std::make_shared<const std::string>("")),
            it_(s_->cbegin() + pos)
        {}

        static position from(const position& other, std::ptrdiff_t horizontal_offset)
        {
            return position(other.s_, other.it_ + horizontal_offset);
        }

        template<class T>
        friend class parser;

        friend bool operator==(const position& lhs, const position& rhs)
        {
            return *lhs.s_ == *rhs.s_ &&
                std::distance(lhs.s_->cbegin(), lhs.it_) == std::distance(rhs.s_->cbegin(), rhs.it_);
        }

        std::string to_string() const
        {
            return std::string("position=") + 
                std::to_string(std::distance(s_->begin(), it_)) + "/" + 
                std::to_string(s_->length()) + " (" +
                (it_ == s_->end() ? "<end>" : std::string(1, *it_)) + ")";
        }
    };

}

#endif // ASCII_TREE_POSITION_H