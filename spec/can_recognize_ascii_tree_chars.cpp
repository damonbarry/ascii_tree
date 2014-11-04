#include "grammar.hpp"
#include "test_helpers.hpp"
#include <CppUnitTest.h>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace
{
    class terminal_assertions
    {
        const terminal& term_;
    public:
        explicit terminal_assertions(const terminal& term) : term_(term) {}
        void should_be(const terminal& other, const wchar_t* message = nullptr) { Assert::AreEqual(other, term_, message); }
    };

    terminal_assertions _(const terminal& term) { return terminal_assertions(term); }
}

namespace ascii_tree { namespace spec
{
    TEST_CLASS(can_recognize_ascii_tree_chars)
    {
    public:
        TEST_METHOD(should_recognize_an_opening_square_brace)
        {
            terminal term = terminal_traits::to_terminal('[');
            _(term).should_be(open_square_brace);
        }

        TEST_METHOD(should_recognize_an_asterisk)
        {
            terminal term = terminal_traits::to_terminal('*');
            _(term).should_be(asterisk);
        }

        TEST_METHOD(should_recognize_a_closing_square_brace)
        {
            terminal term = terminal_traits::to_terminal(']');
            _(term).should_be(close_square_brace);
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
                _(term).should_be(name_char, wstring(&ch, &ch + 1).c_str());
            }
        }

        TEST_METHOD(should_recognize_a_dash)
        {
            terminal term = terminal_traits::to_terminal('-');
            _(term).should_be(dash);
        }

        TEST_METHOD(should_recognize_a_opening_paren)
        {
            terminal term = terminal_traits::to_terminal('(');
            _(term).should_be(open_paren);
        }

        TEST_METHOD(should_recognize_a_closing_paren)
        {
            terminal term = terminal_traits::to_terminal(')');
            _(term).should_be(close_paren);
        }

        TEST_METHOD(should_recognize_a_backslash)
        {
            terminal term = terminal_traits::to_terminal('\\');
            _(term).should_be(backslash);
        }

        TEST_METHOD(should_recognize_a_pipe)
        {
            terminal term = terminal_traits::to_terminal('|');
            _(term).should_be(pipe);
        }

        TEST_METHOD(should_recognize_a_slash)
        {
            terminal term = terminal_traits::to_terminal('/');
            _(term).should_be(slash);
        }

        TEST_METHOD(should_recognize_a_space)
        {
            terminal term = terminal_traits::to_terminal(' ');
            _(term).should_be(space);
        }

        TEST_METHOD(should_not_recognize_an_invalid_char)
        {
            terminal term = terminal_traits::to_terminal('~');
            _(term).should_be(none);
        }

    };
}}
