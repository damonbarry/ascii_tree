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

    TEST_CASE("The parser copy ctor should make a copy", "[Can parse chars]")
    {
        test_parser original("12");
        original.expect(one);

        auto copy = original;
        copy.expect(two);

        _(original.at_end()).should_be_false();
        _(copy.at_end()).should_be_true();
    }

    TEST_CASE("parser::ignore should not advance the parser on an empty string", "[Can parse chars]")
    {
        test_parser p("");
        p.ignore();
        _(p.at_end()).should_be_true();
    }

    TEST_CASE("parser::ignore should ignore chars", "[Can parse chars]")
    {
        test_parser p("333");
        p.ignore();
        _(p.at_end()).should_be_true();
    }

    TEST_CASE("ignore should not advance to the next line", "[Can parse chars]")
    {
        test_parser p({ "12", "34" }, 0, 2); // starting position is the end of the first row
        p.ignore();
        _(p.current_position()).should_be(p.position_at(0, 2));
    }

    TEST_CASE("parser::unignore should not rewind the parser on an empty string", "[Can parse chars]")
    {
        test_parser p("");
        p.unignore();
        _(p.at_end()).should_be_true();
    }

    TEST_CASE("parser::unignore should rewind to the beginning of an ignored char sequence", "[Can parse chars]")
    {
        string test_str = "1332";
        test_parser p(test_str, 1); // position is here: "1>332"
        position expected_pos = p.current_position();
        p.accept(one);              // moves to here:    "133>2"
        p.unignore();               // rewinds to here:  "1>332"
        _(p.current_position()).should_be(expected_pos);
    }

    TEST_CASE("parser::unignore should not try to rewind past the beginning of the string", "[Can parse chars]")
    {
        string test_str = "3333";
        test_parser p(test_str, test_str.size());
        p.unignore();
        _(p.at_begin()).should_be_true();
    }

    TEST_CASE("unignore should not try to rewind to a previous string", "[Can parse chars]")
    {
        test_parser p({ "33", "31" }, 1, 1);
        p.unignore();
        _(p.at_begin()).should_be_false();
        _(p.at_line_begin()).should_be_true();
    }

    TEST_CASE("parser::accept should accept a terminal when it matches the expected value", "[Can parse chars]")
    {
        test_parser p("1");
        _(p.accept(one)).should_be_true();
    }

    TEST_CASE("parser::accept should not accept a terminal which does not match the expected value", "[Can parse chars]")
    {
        test_parser p("1");
        _(p.accept(two)).should_be_false();
    }

    TEST_CASE("parser::accept should accept a terminal when preceeded by ignored chars", "[Can parse chars]")
    {
        test_parser p("3331");
        _(p.accept(one)).should_be_true();
    }

    TEST_CASE("should not accept a terminal on the next row", "[Can parse chars]")
    {
        test_parser p({ "1", "2" });
        p.accept(one);
        _(p.accept(two)).should_be_false();
    }

    TEST_CASE("parser::expect should not throw when a terminal matches the expected value", "[Can parse chars]")
    {
        should_not_throw_([]
        {
            test_parser("1").expect(one);
        });
    }

    TEST_CASE("parser::expect should throw when a terminal does not match the expected value", "[Can parse chars]")
    {
        should_throw_(parse_exception("1", 0), []
        {
            test_parser("1").expect(two);
        });
    }

    TEST_CASE("expect should throw when a terminal matches on the next row", "[Can parse chars]")
    {
        should_throw_(parse_exception("1", 1), []
        {
            test_parser p({ "1", "2" });
            p.accept(one);
            p.expect(two);
        });
    }

    TEST_CASE("parser::substring should return the chars from the given position to the current position", "[Can parse chars]")
    {
        test_parser p("1212", 1);   // position is here: "1>212"
        position start = p.current_position();
        p.expect(two);
        p.expect(one);              // moves to here:    "121>2"
        string str = p.substring(start);
        _(str).should_be("21");
    }

    TEST_CASE("parser::error should throw a parse exception", "[Can parse  chars]")
    {
        should_throw_(parse_exception("", 0), []
        {
            test_parser("").error();
        });
    }

    TEST_CASE("should advance to the next row when finished parsing this row", "[Can parse  chars]")
    {
        test_parser p({ "1", "2" }, 0, 1); // starting position is the end of the first row
        p.maybe_advance_row();
        _(p.current_position()).should_be(p.position_at(1, 0));
    }

    TEST_CASE("should not advance to the next row if not finished parsing this row", "[Can parse  chars]")
    {
        test_parser p({ "1", "2" });
        p.maybe_advance_row();
        _(p.at_begin()).should_be_true();
    }
}}
