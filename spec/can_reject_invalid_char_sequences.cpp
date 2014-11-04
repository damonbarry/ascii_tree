#include "grammar.hpp"
#include "test_helpers.hpp"

using namespace std;

namespace ascii_tree { namespace spec
{
    TEST_CLASS(can_reject_invalid_char_sequences)
    {
    public:

        TEST_METHOD(should_reject_an_empty_node)
        {
            should_throw_(parse_exception("[]", 1), [&]
            {
                grammar("[]").tokens();
            });
        }

        TEST_METHOD(should_reject_a_node_with_an_invalid_name_char)
        {
            should_throw_(parse_exception("[!]", 1), [&]
            {
                grammar("[!]").tokens();
            });
        }

        TEST_METHOD(should_reject_two_open_square_braces_in_a_row)
        {
            should_throw_(parse_exception("[[", 1), [&]
            {
                grammar("[[").tokens();
            });
        }

        TEST_METHOD(should_reject_an_orphaned_close_square_brace)
        {
            should_throw_(parse_exception("]", 0), [&]
            {
                grammar("]").tokens();
            });
        }

        TEST_METHOD(should_reject_a_close_square_brace_following_a_root_node)
        {
            should_throw_(parse_exception("[*]]", 3), [&]
            {
                grammar("[*]]").tokens();
            });
        }

        TEST_METHOD(should_reject_a_nameless_horizontal_edge)
        {
            should_throw_(parse_exception("[*]--[a]", 5), [&]
            {
                grammar("[*]--[a]").tokens();
            });
        }

        TEST_METHOD(should_reject_a_root_horizontal_edge)
        {
            should_throw_(parse_exception("--(*)--", 3), [&]
            {
                grammar("--(*)--").tokens();
            });
        }

        TEST_METHOD(should_reject_a_nearly_valid_edge_name)
        {
            should_throw_(parse_exception("(abAB12')", 7), [&]
            {
                grammar("(abAB12')").tokens();
            });
        }

    };
}}
