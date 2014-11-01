#include "grammar.hpp"
#include "test_helpers.hpp"
#include <CppUnitTest.h>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace ascii_tree { namespace spec
{
    TEST_CLASS(can_reject_invalid_char_sequences)
    {
    public:

        TEST_METHOD(should_reject_an_empty_node)
        {
            should_throw(ascii_tree_parse_exception("[]", 1), [&]
            {
                tokenize("[]");
            });
        }

        TEST_METHOD(should_reject_a_node_with_an_invalid_name_char)
        {
            should_throw(ascii_tree_parse_exception("[!]", 1), [&]
            {
                tokenize("[!]");
            });
        }

        TEST_METHOD(should_reject_two_open_square_braces_in_a_row)
        {
            should_throw({"[[", 1}, [&]
            {
                tokenize("[[");
            });
        }

    };
}}
