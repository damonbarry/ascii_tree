#include "grammar.hpp"
#include "syntax_tree.hpp"
#include <CppUnitTest.h>
#include <type_traits>
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

    template<>
    inline std::wstring ToString<position>(const position& pos)
    {
        return pos.to_string();
    }
}}}

namespace ascii_tree { namespace spec
{
    inline void exception_contents_should_match_(
        const ascii_tree::parse_exception& expected, const ascii_tree::parse_exception& actual)
    {
        namespace cpput = Microsoft::VisualStudio::CppUnitTestFramework;

        cpput::Assert::AreEqual(expected.s.c_str(), actual.s.c_str(),
            L"value of parse_exception::s is wrong");
        cpput::Assert::AreEqual(expected.pos, actual.pos,
            L"value of parse_exception::pos is wrong");
    }

    inline void exception_contents_should_match_(
        const ascii_tree::analyze_exception& expected, const ascii_tree::analyze_exception& actual)
    {
        namespace cpput = Microsoft::VisualStudio::CppUnitTestFramework;

        cpput::Assert::AreEqual(expected.reason.c_str(), actual.reason.c_str(),
            L"value of analyze_exception::reason is wrong");
    }

    template<typename Ex, typename Fn>
    void should_throw_(const Ex& expected, Fn fn)
    {
        namespace cpput = Microsoft::VisualStudio::CppUnitTestFramework;

        cpput::Assert::ExpectException<Ex>([&]
        {
            try
            {
                fn();
            }
            catch (const Ex& actual)
            {
                exception_contents_should_match_(expected, actual);
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

    class bool_assertions
    {
        const bool val_;
    public:
        explicit bool_assertions(bool val) : val_(val) {}
        void should_be_true()
        {
            Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue(val_);
        }
        void should_be_false()
        {
            Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsFalse(val_);
        }
    };

    template<class T>
    class value_assertions
    {
        const T val_;
    public:
        value_assertions(T val) : val_(val) {}
        void should_be(T other, const wchar_t* message = nullptr)
        {
            Microsoft::VisualStudio::CppUnitTestFramework::Assert::AreEqual(other, val_, message);
        }
    };

    template<class T>
    class pointer_assertions
    {
        const T ptr_;
    public:
        pointer_assertions(T ptr) : ptr_(ptr) {}
        void should_not_be_null()
        {
            Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsNotNull(ptr_);
        }
    };

    inline tokens_assertions _(const std::vector<token>& tokens) { return tokens_assertions(tokens); }
    inline bool_assertions _(bool val) { return bool_assertions(val); }

    template<class T,
        typename std::enable_if<!std::is_pointer<T>::value>::type* = nullptr>
    value_assertions<T> _(T val) { return value_assertions<T>(val); }

    template<class T,
        typename std::enable_if<std::is_pointer<T>::value>::type* = nullptr>
    pointer_assertions<T> _(T ptr) { return pointer_assertions<T>(ptr); }

}}
