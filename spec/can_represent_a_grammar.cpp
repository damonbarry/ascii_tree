#include "grammar.hpp"
#include "test_helpers.hpp"
#include <CppUnitTest.h>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace Microsoft { namespace VisualStudio { namespace CppUnitTestFramework
{
    using namespace ascii_tree;

    template<>
    wstring ToString<terminal>(const terminal& term)
    {
        switch (term)
        {
        case none:
            return L"none";
        case open_square_brace:
            return L"open_square_brace";
        case close_square_brace:
            return L"close_square_brace";
        case asterisk:
            return L"asterisk";
        case dash:
            return L"dash";
        case open_paren:
            return L"open_paren";
        case close_paren:
            return L"close_paren";
        case name_char:
            return L"name_char";
        case slash:
            return L"slash";
        case backslash:
            return L"backslash";
        case pipe:
            return L"pipe";
        default:
            return L"unknown char";
        }
    }
}}}

namespace ascii_tree { namespace spec
{
    TEST_CLASS(can_represent_a_grammar)
    {
    public:
        TEST_METHOD(should_recognize_an_opening_square_brace)
        {
            terminal term = to_terminal('[');
            Assert::AreEqual(open_square_brace, term);
        }

        TEST_METHOD(should_recognize_an_asterisk)
        {
            terminal term = to_terminal('*');
            Assert::AreEqual(asterisk, term);
        }

        TEST_METHOD(should_recognize_a_closing_square_brace)
        {
            terminal term = to_terminal(']');
            Assert::AreEqual(close_square_brace, term);
        }

        TEST_METHOD(should_recognize_a_name_char)
        {
            string name_chars =
                "_"
                "1234567890"
                "abcdefghijklmnopqrstuvwxyz"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

            for (auto ch : name_chars)
            {
                terminal term = to_terminal(ch);
                Assert::AreEqual(name_char, term, wstring(&ch, &ch + 1).c_str());
            }
        }

        TEST_METHOD(should_recognize_a_dash)
        {
            terminal term = to_terminal('-');
            Assert::AreEqual(dash, term);
        }

        TEST_METHOD(should_recognize_a_opening_paren)
        {
            terminal term = to_terminal('(');
            Assert::AreEqual(open_paren, term);
        }

        TEST_METHOD(should_recognize_a_closing_paren)
        {
            terminal term = to_terminal(')');
            Assert::AreEqual(close_paren, term);
        }

        TEST_METHOD(should_recognize_a_backslash)
        {
            terminal term = to_terminal('\\');
            Assert::AreEqual(backslash, term);
        }

        TEST_METHOD(should_recognize_a_pipe)
        {
            terminal term = to_terminal('|');
            Assert::AreEqual(pipe, term);
        }

        TEST_METHOD(should_recognize_a_slash)
        {
            terminal term = to_terminal('/');
            Assert::AreEqual(slash, term);
        }

        TEST_METHOD(should_recognize_a_space)
        {
            terminal term = to_terminal(' ');
            Assert::AreEqual(space, term);
        }

        TEST_METHOD(should_not_recognize_an_invalid_char)
        {
            terminal term = to_terminal('~');
            Assert::AreEqual(none, term);
        }

        TEST_METHOD(should_accept_a_terminal_when_it_matches_the_expected_value)
        {
            grammar g("*");
            bool accepted = g.accept(asterisk);
            Assert::IsTrue(accepted);
        }

        TEST_METHOD(should_not_accept_a_terminal_which_does_not_match_the_expected_value)
        {
            grammar g("*");
            bool accepted = g.accept(pipe);
            Assert::IsFalse(accepted);
        }

        TEST_METHOD(should_accept_a_terminal_with_preceeding_spaces)
        {
            grammar g("   *");
            bool accepted = g.accept(asterisk);
            Assert::IsTrue(accepted);
        }

        TEST_METHOD(expect_should_not_throw_when_a_terminal_matches_the_expected_value)
        {
            should_not_throw([&]
            {
                grammar g("*");
                g.expect(asterisk);
            });
        }

        TEST_METHOD(expect_should_throw_when_a_terminal_does_not_match_the_expected_value)
        {
            should_throw(ascii_tree_parse_exception("*", 0), [&]
            {
                grammar g("*");
                g.expect(pipe);
            });
        }

        TEST_METHOD(grammar_should_recognize_a_root_node)
        {
            grammar g("[*]");
            token tok = g.root_node();
            Assert::AreEqual(root_node(), tok);
        }

        TEST_METHOD(grammar_should_recognize_a_named_node)
        {
            grammar g("[abc]");
            should_not_throw([&]{ g.named_node(); });
        }

        TEST_METHOD(grammar_should_reject_an_empty_node_when_it_expects_a_named_node)
        {
            grammar g("[]");
            should_throw(ascii_tree_parse_exception("[]", 1), [&]{
                g.named_node();
            });
        }

        TEST_METHOD(grammar_should_recognize_an_edge_name)
        {
            grammar g("(abc)");
            should_not_throw([&]{ g.edge_name(); });
        }

        TEST_METHOD(grammar_should_reject_an_empty_edge_name)
        {
            grammar g("()");
            should_throw(ascii_tree_parse_exception("()", 1), [&]{
                g.edge_name();
            });
        }

        TEST_METHOD(grammar_should_recognize_an_ascending_edge_part)
        {
            grammar g("/");
            should_not_throw([&]{ g.ascending_edge_part(); });
        }

        TEST_METHOD(grammar_should_recognize_a_descending_edge_part)
        {
            grammar g("\\");
            should_not_throw([&]{ g.descending_edge_part(); });
        }

        TEST_METHOD(grammar_should_recognize_a_vertical_edge_part)
        {
            grammar g("|");
            should_not_throw([&]{ g.vertical_edge_part(); });
        }

        TEST_METHOD(gramar_should_recognize_a_horizontal_edge)
        {
            grammar g("---(xyz)--");
            should_not_throw([&]{ g.horizontal_edge(); });
        }

        TEST_METHOD(grammar_should_reject_a_nameless_horizontal_edge)
        {
            grammar g("--");
            should_throw(ascii_tree_parse_exception("--", 2), [&]{
                g.horizontal_edge();
            });
        }

    };
}}
