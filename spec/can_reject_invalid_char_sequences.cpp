#include "grammar.hpp"
#include "test_helpers.hpp"

using namespace std;

namespace ascii_tree { namespace spec
{
    TEST_CASE("should reject an empty node", "[can reject invalid char sequences]")
    {
        should_throw_(parse_exception("[]", 1), [&]
        {
            grammar("[]").tokens();
        });
    }

    TEST_CASE("should reject a node with an invalid name char", "[can reject invalid char sequences]")
    {
        should_throw_(parse_exception("[!]", 1), [&]
        {
            grammar("[!]").tokens();
        });
    }

    TEST_CASE("should reject two open square braces in a row", "[can reject invalid char sequences]")
    {
        should_throw_(parse_exception("[[", 1), [&]
        {
            grammar("[[").tokens();
        });
    }

    TEST_CASE("should reject an orphaned close square brace", "[can reject invalid char sequences]")
    {
        should_throw_(parse_exception("]", 0), [&]
        {
            grammar("]").tokens();
        });
    }

    TEST_CASE("should reject a close square brace following a root node", "[can reject invalid char sequences]")
    {
        should_throw_(parse_exception("[*]]", 3), [&]
        {
            grammar("[*]]").tokens();
        });
    }

    TEST_CASE("should reject a nameless horizontal edge between nodes", "[can reject invalid char sequences]")
    {
        should_throw_(parse_exception("[*]--[a]", 5), [&]
        {
            grammar("[*]--[a]").tokens();
        });
    }

    TEST_CASE("should reject a root horizontal edge", "[can reject invalid char sequences]")
    {
        should_throw_(parse_exception("--(*)--", 3), [&]
        {
            grammar("--(*)--").tokens();
        });
    }

    TEST_CASE("should reject a nearly valid edge name", "[can reject invalid char sequences]")
    {
        should_throw_(parse_exception("(abAB12')", 7), [&]
        {
            grammar("(abAB12')").tokens();
        });
    }
}}
