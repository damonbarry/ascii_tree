#include "parser.hpp"
#include "test_helpers.hpp"
#include <string>

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

    typedef parser<test_traits> test_parser;

    TEST_CLASS(can_parse_chars)
    {
    public:
        TEST_METHOD(a_parser_copy_should_be_independent_from_the_original)
        {
            test_parser original("12");
            original.expect(one);

            auto copy = original;
            copy.expect(two);

            _(original.at_end()).should_be_false();
            _(copy.at_end()).should_be_true();
        }

        TEST_METHOD(ignore_should_not_advance_the_parser_on_an_empty_string)
        {
            test_parser p("");
            p.ignore();
            _(p.at_end()).should_be_true();
        }

        TEST_METHOD(ignore_should_ignore_chars)
        {
            test_parser p("33313");
            p.ignore();
            p.expect(one);
            p.ignore();
            _(p.at_end()).should_be_true();
        }

        TEST_METHOD(ignore_should_ignore_through_multiple_lines)
        {
            test_parser p({ "33", "31" });
            p.ignore();
            p.expect(one);
            _(p.at_end()).should_be_true();
        }

        TEST_METHOD(unignore_should_not_rewind_the_parser_on_an_empty_string)
        {
            test_parser p("");
            p.unignore();
            _(p.at_end()).should_be_true();
        }

        TEST_METHOD(unignore_should_rewind_to_the_beginning_of_an_ignored_char_sequence)
        {
            string test_str = "1332";
            test_parser p(test_str, 1); // position is here: "1>332"
            position expected_pos = p.current_position();
            p.accept(one);              // moves to here:    "133>2"
            p.unignore();               // rewinds to here:  "1>332"
            _(p.current_position()).should_be(expected_pos);
        }

        TEST_METHOD(unignore_should_not_try_to_rewind_past_the_beginning_of_the_string)
        {
            string test_str = "3333";
            test_parser p(test_str, test_str.size());
            p.unignore();
            _(p.at_begin()).should_be_true();
        }

        TEST_METHOD(should_accept_a_terminal_when_it_matches_the_expected_value)
        {
            test_parser p("1");
            _(p.accept(one)).should_be_true();
        }

        TEST_METHOD(should_not_accept_a_terminal_which_does_not_match_the_expected_value)
        {
            test_parser p("1");
            _(p.accept(two)).should_be_false();
        }

        TEST_METHOD(should_accept_a_terminal_when_preceeded_by_ignored_chars)
        {
            test_parser p("3331");
            _(p.accept(one)).should_be_true();
        }

        TEST_METHOD(expect_should_not_throw_when_a_terminal_matches_the_expected_value)
        {
            should_not_throw_([]
            {
                test_parser("1").expect(one);
            });
        }

        TEST_METHOD(expect_should_throw_when_a_terminal_does_not_match_the_expected_value)
        {
            should_throw_(parse_exception("1", 0), []
            {
                test_parser("1").expect(two);
            });
        }

        TEST_METHOD(substring_should_return_the_chars_from_the_given_position_to_the_current_position)
        {
            test_parser p("1212", 1);   // position is here: "1>212"
            position start = p.current_position();
            p.expect(two);
            p.expect(one);              // moves to here:    "121>2"
            string str = p.substring(start);
            _(str).should_be("21");
        }

        TEST_METHOD(error_should_throw_a_parse_exception)
        {
            should_throw_(parse_exception("", 0), []
            {
                test_parser("").error();
            });
        }
    };
}}
