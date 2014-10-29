#if !defined(ASCII_TREE_TOKENS_H)
#define ASCII_TREE_TOKENS_H

#include <string>
#include <vector>

namespace ascii_tree
{
    template<class Traits>
    class tokens
    {
        typedef std::vector<typename Traits::type> token_vector;
        token_vector tokens_;

    public:
        typedef typename token_vector::iterator iterator;
        typedef typename token_vector::const_iterator const_iterator;
        typedef typename Traits::type token_type;

        tokens() {}
        explicit tokens(size_t count) : tokens_(count) {}
        tokens(std::initializer_list<token_type> init) : tokens_(init) {}
        bool empty() const { return tokens_.size() == 0; }
        size_t size() const { return tokens_.size(); }
        token_type& operator[](size_t i) { return tokens_[i]; };
        const token_type& operator[](size_t i) const { return const_cast<tokens&>(*this)[i]; };

        iterator begin() { return tokens_.begin(); }
        const_iterator begin() const { return tokens_.begin(); }
        const_iterator cbegin() const { return tokens_.cbegin(); }
        iterator end() { return tokens_.end(); }
        const_iterator end() const { return tokens_.end(); }
        const_iterator cend() const { return tokens_.cend(); }

        void from_string(std::string s)
        {
            auto newTokens = Traits::tokenize(s);
            tokens_.insert(tokens_.end(), newTokens.begin(), newTokens.end());
        }
    };
}

#endif // ASCII_TREE_TOKENS_H
