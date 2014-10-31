#include "grammar.hpp"
#include <CppUnitTest.h>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace ascii_tree { namespace spec
{
    TEST_CLASS(can_reject_invalid_char_sequences)
    {

        template<typename Fn>
        static void expect_parse_exception_(const ascii_tree_parse_exception& expected, Fn fn)
        {
            Assert::ExpectException<ascii_tree_parse_exception>([&]
            {
                try
                {
                    fn();
                }
                catch (ascii_tree_parse_exception& actual)
                {
                    Assert::AreEqual(expected.s.c_str(), actual.s.c_str(),
                        L"value of ascii_tree_parse_exception::s is wrong");
                    Assert::AreEqual(expected.pos, actual.pos,
                        L"value of ascii_tree_parse_exception::pos is wrong");
                    throw;
                }
                catch (...)
                {
                    throw;
                }
            });
        }

    public:

        TEST_METHOD(should_reject_an_empty_node)
        {
            expect_parse_exception_(ascii_tree_parse_exception("[]", 1), [&]
            {
                auto tokens = tokenize("[]");
            });
        }

        TEST_METHOD(should_reject_a_node_with_an_invalid_name_char)
        {
            expect_parse_exception_(ascii_tree_parse_exception("[!]", 1), [&]
            {
                auto tokens = tokenize("[!]");
            });
        }

    };
}}
