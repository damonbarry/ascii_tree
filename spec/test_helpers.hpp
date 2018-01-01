#include "grammar.hpp"
#include "syntax_tree.hpp"
#include "catch.hpp"
#include <type_traits>
#include <algorithm>
#include <string>

namespace ascii_tree { namespace spec
{
    namespace details
    {
        inline std::string to_string(const terminal& term)
        {
            switch (term)
            {
            case none:
                return "none";
            case open_square_brace:
                return "open_square_brace";
            case close_square_brace:
                return "close_square_brace";
            case asterisk:
                return "asterisk";
            case dash:
                return "dash";
            case open_paren:
                return "open_paren";
            case close_paren:
                return "close_paren";
            case name_char:
                return "name_char";
            case slash:
                return "slash";
            case backslash:
                return "backslash";
            case pipe:
                return "pipe";
            default:
                return "unknown char";
            }
        }

        inline std::string to_string(const token::toktype& type)
        {
            switch (type)
            {
            case token::root_node:
                return "root_node";
            case token::named_node:
                return "named_node";
            case token::edge_name:
                return "edge_name";
            case token::horizontal_edge:
                return "horizontal_edge";
            case token::ascending_edge_part:
                return "ascending_edge_part";
            case token::descending_edge_part:
                return "descending_edge_part";
            case token::vertical_edge_part:
                return "vertical_edge_part";
            default:
                return "unknown token";
            }
        }

        inline std::string to_string(const token& tok)
        {
            std::string name(tok.name.begin(), tok.name.end());
            return to_string(tok.type) + " " + name;
        }

        inline std::string to_string(const position& pos)
        {
            return pos.to_string();
        }

        inline void exception_contents_should_match_(
            const ascii_tree::parse_exception& expected, const ascii_tree::parse_exception& actual)
        {
            REQUIRE(expected.s == actual.s);
            REQUIRE(expected.pos == actual.pos);
        }

        inline void exception_contents_should_match_(
            const ascii_tree::analyze_exception& expected, const ascii_tree::analyze_exception& actual)
        {
            REQUIRE(expected.reason == actual.reason);
        }

        class matches_tokens : public Catch::MatcherBase<std::vector<token>>
        {
            const std::vector<token>& expected_;
        public:
            explicit matches_tokens(const std::vector<token>& expected) : expected_(expected) {}
            virtual bool match(const std::vector<token>& tokens) const override
            {
                auto mismatch_pair = std::mismatch(expected_.begin(), expected_.end(), tokens.begin());
                return mismatch_pair.first == expected_.end() && mismatch_pair.second == tokens.end();
            }
            virtual std::string describe() const override
            {
                std::ostringstream ss;
                std::string separator = "equals { ";
                for (auto& token : expected_)
                {
                    ss << separator << details::to_string(token.type) << "(\"" << token.name << "\")";
                    separator = ", ";
                }
                ss << " }";
                return ss.str();
            }
        };

        class matches_positions : public Catch::MatcherBase<std::vector<position>>
        {
            const std::vector<position>& expected_;
        public:
            explicit matches_positions(const std::vector<position>& expected) : expected_(expected) {}
            virtual bool match(const std::vector<position>& actual) const override
            {
                auto mismatch_pair = std::mismatch(expected_.begin(), expected_.end(), actual.begin());
                return mismatch_pair.first == expected_.end() && mismatch_pair.second == actual.end();
            }
            virtual std::string describe() const override
            {
                std::ostringstream ss;
                std::string separator = "equals { ";
                for(auto& position : expected_)
                {
                    ss << separator << position.to_string();
                    separator = ", ";
                }
                ss << " }";
                return ss.str();
            }
        };
    }

    inline std::vector<position> positions_from(const std::vector<token>& tokens)
    {
        std::vector<position> positions;
        std::for_each(tokens.begin(), tokens.end(), [&](const token& t){ positions.push_back(t.position); });
        return positions;
    }

    template<typename Ex, typename Fn>
    void should_throw_(const Ex& expected, Fn fn)
    {
        REQUIRE_THROWS_AS([&]{
            try
            {
                fn();
            }
            catch (Ex& actual)
            {
                details::exception_contents_should_match_(expected, actual);
                throw;
            }
        }(), Ex);
    }

    template<typename Fn>
    void should_not_throw_(Fn fn)
    {
        REQUIRE_NOTHROW(fn());
    }

    class tokens_assertions
    {
        const std::vector<token>& tokens_;
    public:
        explicit tokens_assertions(const std::vector<token>& tokens) : tokens_(tokens) {}
        void should_be_empty()
        {
            REQUIRE(tokens_.empty() == true);
        }
        void should_equal(std::initializer_list<token> expected)
        {
            REQUIRE_THAT(tokens_, details::matches_tokens(expected));
        }
    };

    class positions_assertions
    {
        const std::vector<position>& positions_;
    public:
        explicit positions_assertions(const std::vector<position>& positions) : positions_(positions) {}
        void should_equal(std::initializer_list<position> expected)
        {
            REQUIRE_THAT(positions_, details::matches_positions(expected));
        }
    };

    class bool_assertions
    {
        const bool val_;
    public:
        explicit bool_assertions(bool val) : val_(val) {}
        void should_be_true()
        {
            REQUIRE(val_ == true);
        }
        void should_be_false()
        {
            REQUIRE(val_ == false);
        }
    };

    class node_assertions
    {
        const node& node_;
    public:
        node_assertions(const node& n) : node_(n) {}
        void should_be(const token& tok/*, const wchar_t* message = nullptr*/)
        {
            REQUIRE(tok == node_.tok);
        }
        void should_equal(const node& expected)
        {
            REQUIRE(node_ == expected);
        }
        void should_have_node_along_edge(const std::string& node_name, const std::string& edge_name/*, const wchar_t* message = nullptr*/)
        {
            auto edge_it = std::find_if(node_.edges.begin(), node_.edges.end(), [&edge_name](const edge& e){
                return (edge_name == e.tok.name);
            });

            REQUIRE(edge_it != node_.edges.end());
            REQUIRE(node_name == edge_it->node.tok.name);
        }
    };

    template<class T>
    class value_assertions
    {
        const T val_;
    public:
        value_assertions(T val) : val_(val) {}
        void should_be(T other/*, const wchar_t* message = nullptr*/)
        {
            REQUIRE(other == val_);
        }
    };

    inline tokens_assertions _(const std::vector<token>& tokens) { return tokens_assertions(tokens); }
    inline positions_assertions _(const std::vector<position>& positions) { return positions_assertions(positions); }
    inline bool_assertions _(bool val) { return bool_assertions(val); }
    inline node_assertions _(const node& n) { return node_assertions(n); }

    template<class T,
    typename std::enable_if<!std::is_pointer<T>::value>::type* = nullptr>
    value_assertions<T> _(T val) { return value_assertions<T>(val); }
}}
