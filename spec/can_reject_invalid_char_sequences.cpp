#include "grammar.hpp"
#include <CppUnitTest.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ascii_tree { namespace spec
{
    TEST_CLASS(can_reject_invalid_char_sequences)
    {
    public:

        TEST_METHOD(should_reject_an_empty_node)
        {
            Assert::ExpectException<ascii_tree_parse_exception>([&]
            {
                auto tokens = tokenize("[]");
            });
        }

    };
}}
