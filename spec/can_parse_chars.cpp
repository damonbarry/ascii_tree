#include "parser.hpp"
#include "test_helpers.hpp"
#include <CppUnitTest.h>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace ascii_tree { namespace spec
{
    enum test_terminal { none, one, two, space };
    struct test_traits
    {
        typedef test_terminal type;
        static const type space = test_terminal::space;

        static type to_terminal(char ch)
        {
            if (ch == '1') return one;
            else if (ch == '2') return two;
            else if (ch == ' ') return space;
            return none;
        }
    };

    TEST_CLASS(can_parse_chars)
    {
    public:
        TEST_METHOD(should_accept_a_terminal_when_it_matches_the_expected_value)
        {
            parser<test_traits> p("1");
            bool accepted = p.accept(one);
            Assert::IsTrue(accepted);
        }

        TEST_METHOD(should_not_accept_a_terminal_which_does_not_match_the_expected_value)
        {
            parser<test_traits> p("1");
            bool accepted = p.accept(two);
            Assert::IsFalse(accepted);
        }

        TEST_METHOD(should_accept_a_terminal_with_preceeding_spaces)
        {
            parser<test_traits> p("   1");
            bool accepted = p.accept(one);
            Assert::IsTrue(accepted);
        }

        TEST_METHOD(expect_should_not_throw_when_a_terminal_matches_the_expected_value)
        {
            should_not_throw([&]
            {
                parser<test_traits> p("1");
                p.expect(one);
            });
        }

        TEST_METHOD(expect_should_throw_when_a_terminal_does_not_match_the_expected_value)
        {
            should_throw(ascii_tree_parse_exception("1", 0), [&]
            {
                parser<test_traits> p("1");
                p.expect(two);
            });
        }
    };
}}