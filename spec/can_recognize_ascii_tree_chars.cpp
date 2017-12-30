#include "grammar.hpp"
#include "test_helpers.hpp"
#include <string>

using namespace std;

namespace ascii_tree { namespace spec
{
    TEST_CASE("should recognize an opening square brace", "[can recognize ascii tree chars]")
    {
        terminal term = terminal_traits::to_terminal('[');
        _(term).should_be(open_square_brace);
    }

    TEST_CASE("should recognize an asterisk", "[can recognize ascii tree chars]")
    {
        terminal term = terminal_traits::to_terminal('*');
        _(term).should_be(asterisk);
    }

    TEST_CASE("should recognize a closing square brace", "[can recognize ascii tree chars]")
    {
        terminal term = terminal_traits::to_terminal(']');
        _(term).should_be(close_square_brace);
    }

    TEST_CASE("should recognize a name char", "[can recognize ascii tree chars]")
    {
        string name_chars =
            "_"
            "1234567890"
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        for (auto ch : name_chars)
        {
            terminal term = terminal_traits::to_terminal(ch);
            _(term).should_be(name_char/*, wstring(&ch, &ch + 1).c_str()*/);
        }
    }

    TEST_CASE("should recognize a dash", "[can recognize ascii tree chars]")
    {
        terminal term = terminal_traits::to_terminal('-');
        _(term).should_be(dash);
    }

    TEST_CASE("should recognize a opening paren", "[can recognize ascii tree chars]")
    {
        terminal term = terminal_traits::to_terminal('(');
        _(term).should_be(open_paren);
    }

    TEST_CASE("should recognize a closing paren", "[can recognize ascii tree chars]")
    {
        terminal term = terminal_traits::to_terminal(')');
        _(term).should_be(close_paren);
    }

    TEST_CASE("should recognize a backslash", "[can recognize ascii tree chars]")
    {
        terminal term = terminal_traits::to_terminal('\\');
        _(term).should_be(backslash);
    }

    TEST_CASE("should recognize a pipe", "[can recognize ascii tree chars]")
    {
        terminal term = terminal_traits::to_terminal('|');
        _(term).should_be(pipe);
    }

    TEST_CASE("should recognize a slash", "[can recognize ascii tree chars]")
    {
        terminal term = terminal_traits::to_terminal('/');
        _(term).should_be(slash);
    }

    TEST_CASE("should recognize a space", "[can recognize ascii tree chars]")
    {
        terminal term = terminal_traits::to_terminal(' ');
        _(term).should_be(space);
    }

    TEST_CASE("should not recognize an invalid char", "[can recognize ascii tree chars]")
    {
        terminal term = terminal_traits::to_terminal('~');
        _(term).should_be(none);
    }
}}
