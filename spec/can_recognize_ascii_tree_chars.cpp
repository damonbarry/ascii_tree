#include "grammar.hpp"
#include "test_helpers.hpp"
#include <CppUnitTest.h>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace Microsoft { namespace VisualStudio { namespace CppUnitTestFramework
{
    using namespace ascii_tree;

    template<>
    wstring ToString<terminal>(const terminal& term)
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
}}}

namespace ascii_tree { namespace spec
{
    TEST_CLASS(can_recognize_ascii_tree_chars)
    {
    public:
        TEST_METHOD(should_recognize_an_opening_square_brace)
        {
            terminal term = terminal_traits::to_terminal('[');
            Assert::AreEqual(open_square_brace, term);
        }

        TEST_METHOD(should_recognize_an_asterisk)
        {
            terminal term = terminal_traits::to_terminal('*');
            Assert::AreEqual(asterisk, term);
        }

        TEST_METHOD(should_recognize_a_closing_square_brace)
        {
            terminal term = terminal_traits::to_terminal(']');
            Assert::AreEqual(close_square_brace, term);
        }

        TEST_METHOD(should_recognize_a_name_char)
        {
            string name_chars =
                "_"
                "1234567890"
                "abcdefghijklmnopqrstuvwxyz"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

            for (auto ch : name_chars)
            {
                terminal term = terminal_traits::to_terminal(ch);
                Assert::AreEqual(name_char, term, wstring(&ch, &ch + 1).c_str());
            }
        }

        TEST_METHOD(should_recognize_a_dash)
        {
            terminal term = terminal_traits::to_terminal('-');
            Assert::AreEqual(dash, term);
        }

        TEST_METHOD(should_recognize_a_opening_paren)
        {
            terminal term = terminal_traits::to_terminal('(');
            Assert::AreEqual(open_paren, term);
        }

        TEST_METHOD(should_recognize_a_closing_paren)
        {
            terminal term = terminal_traits::to_terminal(')');
            Assert::AreEqual(close_paren, term);
        }

        TEST_METHOD(should_recognize_a_backslash)
        {
            terminal term = terminal_traits::to_terminal('\\');
            Assert::AreEqual(backslash, term);
        }

        TEST_METHOD(should_recognize_a_pipe)
        {
            terminal term = terminal_traits::to_terminal('|');
            Assert::AreEqual(pipe, term);
        }

        TEST_METHOD(should_recognize_a_slash)
        {
            terminal term = terminal_traits::to_terminal('/');
            Assert::AreEqual(slash, term);
        }

        TEST_METHOD(should_recognize_a_space)
        {
            terminal term = terminal_traits::to_terminal(' ');
            Assert::AreEqual(space, term);
        }

        TEST_METHOD(should_not_recognize_an_invalid_char)
        {
            terminal term = terminal_traits::to_terminal('~');
            Assert::AreEqual(none, term);
        }

    };
}}