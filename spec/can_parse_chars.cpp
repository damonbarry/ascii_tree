#include "parser.hpp"
#include "test_helpers.hpp"
#include <CppUnitTest.h>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace ascii_tree { namespace spec
{
    enum test_terminal { none, one, two, three };
    struct test_traits
    {
        typedef test_terminal type;
        static const type ignore_me = three;

        static type to_terminal(char ch)
        {
            if (ch == '1') return one;
            else if (ch == '2') return two;
            else if (ch == '3') return three;
            return none;
        }
    };

    TEST_CLASS(can_parse_chars)
    {
    public:
        TEST_METHOD(ignore_should_not_advance_the_parser_on_an_empty_string)
        {
            parser<test_traits> p("");
            p.ignore();
            Assert::IsTrue(p.at_end());
        }

        TEST_METHOD(ignore_should_ignore_chars)
        {
            parser<test_traits> p("33313");
            p.ignore();
            p.expect(one);
            p.ignore();
            Assert::IsTrue(p.at_end());
        }

        TEST_METHOD(unignore_should_not_rewind_the_parser_on_an_empty_string)
        {
            parser<test_traits> p("");
            p.unignore();
            Assert::IsTrue(p.at_end());
        }

        TEST_METHOD(unignore_should_rewind_to_the_beginning_of_an_ignored_char_sequence)
        {
            string test_str = "1333";
            parser<test_traits> p(test_str, test_str.size());
            p.unignore();
            Assert::AreEqual(test_str.front(), *(p.save_position() - 1));
        }

        TEST_METHOD(unignore_should_not_try_to_rewind_past_the_beginning_of_the_string)
        {
            string test_str = "3333";
            parser<test_traits> p(test_str, test_str.size());
            p.unignore();
            Assert::IsTrue(p.at_begin());
        }

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

        TEST_METHOD(should_accept_a_terminal_when_preceeded_by_ignored_chars)
        {
            parser<test_traits> p("3331");
            bool accepted = p.accept(one);
            Assert::IsTrue(accepted);
        }

        TEST_METHOD(expect_should_not_throw_when_a_terminal_matches_the_expected_value)
        {
            should_not_throw_([&]
            {
                parser<test_traits> p("1");
                p.expect(one);
            });
        }

        TEST_METHOD(expect_should_throw_when_a_terminal_does_not_match_the_expected_value)
        {
            should_throw_(parse_exception("1", 0), [&]
            {
                parser<test_traits> p("1");
                p.expect(two);
            });
        }

        TEST_METHOD(substr_should_return_the_string_starting_from_the_given_position)
        {
            parser<test_traits> p("12121", 4);
            string str = p.substr(p.save_position() - 3);
            Assert::AreEqual("212", str.c_str());
        }

        TEST_METHOD(error_should_throw_a_parse_exception)
        {
            parser<test_traits> p("");
            should_throw_(parse_exception("", 0), [&]{ p.error(); });
        }
    };
}}
