#include "grammar.hpp"
#include <CppUnitTest.h>
#include <algorithm>
#include <string>

namespace Microsoft { namespace VisualStudio { namespace CppUnitTestFramework
{
    using namespace ascii_tree;

    template<>
    inline std::wstring ToString<terminal>(const terminal& term)
    {
        switch (term)
        {
        case none:
            return L"none";
        case open_square_brace:
            return L"open_square_brace";
        case close_square_brace:
            return L"close_square_brace";
        case asterisk:
            return L"asterisk";
        case dash:
            return L"dash";
        case open_paren:
            return L"open_paren";
        case close_paren:
            return L"close_paren";
        case name_char:
            return L"name_char";
        case slash:
            return L"slash";
        case backslash:
            return L"backslash";
        case pipe:
            return L"pipe";
        default:
            return L"unknown char";
        }
    }

    template<>
    inline std::wstring ToString<token::toktype>(const token::toktype& type)
    {
        switch (type)
        {
        case token::root_node:
            return L"root_node";
        case token::named_node:
            return L"named_node";
        case token::edge_name:
            return L"edge_name";
        case token::horizontal_edge:
            return L"horizontal_edge";
        case token::ascending_edge_part:
            return L"ascending_edge_part";
        case token::descending_edge_part:
            return L"descending_edge_part";
        case token::vertical_edge_part:
            return L"vertical_edge_part";
        default:
            return L"unknown token";
        }
    }

    template<>
    inline std::wstring ToString<token>(const token& tok)
    {
        std::wstring name(tok.name.begin(), tok.name.end());
        return ToString(tok.type) + L" " + name;
    }
}}}

namespace ascii_tree { namespace spec
{

    template<typename Fn>
    void should_throw_(const ascii_tree::parse_exception& expected, Fn fn)
    {
        namespace cpput = Microsoft::VisualStudio::CppUnitTestFramework;

        cpput::Assert::ExpectException<ascii_tree::parse_exception>([&]
        {
            try
            {
                fn();
            }
            catch (ascii_tree::parse_exception& actual)
            {
                cpput::Assert::AreEqual(expected.s.c_str(), actual.s.c_str(),
                    L"value of parse_exception::s is wrong");
                cpput::Assert::AreEqual(expected.pos, actual.pos,
                    L"value of parse_exception::pos is wrong");
                throw;
            }
        });
    }

    template<typename Fn>
    void should_not_throw_(Fn fn)
    {
        namespace cpput = Microsoft::VisualStudio::CppUnitTestFramework;

        try
        {
            fn();
        }
        catch (...)
        {
            cpput::Assert::Fail(L"Shouldn't have thrown an exception, but it did.");
        }
    }

    class tokens_assertions
    {
        const std::vector<token>& tokens_;
    public:
        explicit tokens_assertions(const std::vector<token>& tokens) : tokens_(tokens) {}
        void should_be_empty()
        {
            Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue(tokens_.empty());
        }
        void should_equal(std::initializer_list<token> expected)
        {
            namespace cpput = Microsoft::VisualStudio::CppUnitTestFramework;

            auto mismatch_pair = std::mismatch(expected.begin(), expected.end(), tokens_.begin());

            if (mismatch_pair.first != expected.end() || mismatch_pair.second != tokens_.end())
            {
                std::wstring expectedName(mismatch_pair.first->name.begin(), mismatch_pair.first->name.end());
                std::wstring actualName(mismatch_pair.second->name.begin(), mismatch_pair.second->name.end());

                std::wstring message = L"Expected: " + cpput::ToString(mismatch_pair.first->type) + L" \"" + expectedName + L"\" "
                    + L"Actual: " + cpput::ToString(mismatch_pair.second->type) + L" \"" + actualName + L"\"";

                cpput::Assert::Fail(message.c_str());
            }
        }
    };

    class token_assertions
    {
        const token& token_;
    public:
        explicit token_assertions(const token& token) : token_(token) {}
        void should_be(const token& other)
        {
            Microsoft::VisualStudio::CppUnitTestFramework::Assert::AreEqual(other, token_);
        }
    };

    class terminal_assertions
    {
        const terminal& term_;
    public:
        explicit terminal_assertions(const terminal& term) : term_(term) {}
        void should_be(const terminal& other, const wchar_t* message = nullptr)
        {
            Microsoft::VisualStudio::CppUnitTestFramework::Assert::AreEqual(other, term_, message);
        }
    };

    inline tokens_assertions _(const std::vector<token>& tokens) { return tokens_assertions(tokens); }
    inline token_assertions _(const token& token) { return token_assertions(token); }
    inline terminal_assertions _(const terminal& term) { return terminal_assertions(term); }

}}
