#include "grammar.hpp"
#include "test_helpers.hpp"
#include <CppUnitTest.h>
#include <algorithm>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace
{
    class tokens_assertions
    {
        const vector<token>& tokens_;
    public:
        explicit tokens_assertions(const vector<token>& tokens) : tokens_(tokens) {}
        void should_be_empty() { Assert::IsTrue(tokens_.empty()); }
        void should_equal(std::initializer_list<token> expected)
        {
            auto mismatch_pair = std::mismatch(expected.begin(), expected.end(), tokens_.begin());

            if (mismatch_pair.first != expected.end() || mismatch_pair.second != tokens_.end())
            {
                wstring expectedName(mismatch_pair.first->name.begin(), mismatch_pair.first->name.end());
                wstring actualName(mismatch_pair.second->name.begin(), mismatch_pair.second->name.end());

                wstring message = L"Expected: " + ToString(mismatch_pair.first->type) + L" \"" + expectedName + L"\" "
                    + L"Actual: " + ToString(mismatch_pair.second->type) + L" \"" + actualName + L"\"";

                Assert::Fail(message.c_str());
            }
        }
    };

    tokens_assertions _(const vector<token>& tokens) { return tokens_assertions(tokens); }
}

namespace ascii_tree { namespace spec
{
    TEST_CLASS(can_recognize_ascii_tree_tokens)
    {
    public:
        
        TEST_METHOD(should_not_recognize_any_tokens_in_an_empty_string)
        {
            auto tokens = tokenize("");
            _(tokens).should_be_empty();
        }

        TEST_METHOD(should_recognize_a_root_node)
        {
            grammar g("[*]");
            token tok = g.root_node();
            Assert::AreEqual(root_node(), tok);
        }

        TEST_METHOD(should_recognize_a_named_node)
        {
            grammar g("[abc]");
            token tok = g.named_node();
            Assert::AreEqual(named_node("abc"), tok);
        }

        TEST_METHOD(should_reject_an_empty_node_when_it_expects_a_named_node)
        {
            grammar g("[]");
            should_throw(ascii_tree_parse_exception("[]", 1), [&]{
                g.named_node();
            });
        }

        TEST_METHOD(should_recognize_an_edge_name)
        {
            grammar g("(abc)");
            token tok = g.edge_name();
            Assert::AreEqual(edge_name("abc"), tok);
        }

        TEST_METHOD(should_reject_an_empty_edge_name)
        {
            grammar g("()");
            should_throw(ascii_tree_parse_exception("()", 1), [&]{
                g.edge_name();
            });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part)
        {
            grammar g("/");
            Assert::AreEqual(ascending_edge_part(), g.ascending_edge_part());
        }

        TEST_METHOD(should_recognize_a_descending_edge_part)
        {
            grammar g("\\");
            Assert::AreEqual(descending_edge_part(), g.descending_edge_part());
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part)
        {
            grammar g("|");
            Assert::AreEqual(vertical_edge_part(), g.vertical_edge_part());
        }

        TEST_METHOD(should_recognize_a_horizontal_edge)
        {
            grammar g("---(xyz)--");
            token tok = g.horizontal_edge();
            Assert::AreEqual(horizontal_edge("xyz"), tok);
        }

        TEST_METHOD(should_reject_a_nameless_horizontal_edge)
        {
            grammar g("--");
            should_throw(ascii_tree_parse_exception("--", 2), [&]{
                g.horizontal_edge();
            });
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_a_named_node)
        {
            auto tokens = tokenize("[*][a]");
            _(tokens).should_equal({ root_node(), named_node("a") });
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_a_horizontal_edge)
        {
            auto tokens = tokenize("[*]-(a)-");
            _(tokens).should_equal({ root_node(), horizontal_edge("a") });
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_a_descending_edge_part)
        {
            auto tokens = tokenize("[*]\\");
            _(tokens).should_equal({ root_node(), descending_edge_part() });
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_an_ascending_edge_part)
        {
            auto tokens = tokenize("[*]/");
            _(tokens).should_equal({ root_node(), ascending_edge_part() });
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_a_vertical_edge_part)
        {
            auto tokens = tokenize("[*]|");
            _(tokens).should_equal({ root_node(), vertical_edge_part() });
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_an_edge_name)
        {
            auto tokens = tokenize("[*](a)");
            _(tokens).should_equal({ root_node(), edge_name("a") });
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_a_root_node)
        {
            auto tokens = tokenize("[a][*]");
            _(tokens).should_equal({ named_node("a"), root_node() });
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_a_named_node)
        {
            auto tokens = tokenize("[a][b]");
            _(tokens).should_equal({ named_node("a"), named_node("b") });
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_a_horizontal_edge)
        {
            auto tokens = tokenize("[a]-(b)-");
            _(tokens).should_equal({ named_node("a"), horizontal_edge("b") });
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_a_descending_edge_part)
        {
            auto tokens = tokenize("[a]\\");
            _(tokens).should_equal({ named_node("a"), descending_edge_part() });
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_an_ascending_edge_part)
        {
            auto tokens = tokenize("[a]/");
            _(tokens).should_equal({ named_node("a"), ascending_edge_part() });
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_a_vertical_edge_part)
        {
            auto tokens = tokenize("[a]|");
            _(tokens).should_equal({ named_node("a"), vertical_edge_part() });
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_an_edge_name)
        {
            auto tokens = tokenize("[a](b)");
            _(tokens).should_equal({ named_node("a"), edge_name("b") });
        }

        TEST_METHOD(should_recognize_a_horizontal_edge_next_to_a_root_node)
        {
            auto tokens = tokenize("-(a)-[*]");
            _(tokens).should_equal({ horizontal_edge("a"), root_node() });
        }

        TEST_METHOD(should_recognize_a_horizontal_edge_next_to_a_named_node)
        {
            auto tokens = tokenize("-(a)-[b]");
            _(tokens).should_equal({ horizontal_edge("a"), named_node("b") });
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_a_root_node)
        {
            auto tokens = tokenize("\\[*]");
            _(tokens).should_equal({ descending_edge_part(), root_node() });
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_a_named_node)
        {
            auto tokens = tokenize("\\[a]");
            _(tokens).should_equal({ descending_edge_part(), named_node("a") });
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_a_descending_edge_part)
        {
            auto tokens = tokenize("\\\\");
            _(tokens).should_equal({ descending_edge_part(), descending_edge_part() });
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_an_ascending_edge_part)
        {
            auto tokens = tokenize("\\/");
            _(tokens).should_equal({ descending_edge_part(), ascending_edge_part() });
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_a_vertical_edge_part)
        {
            auto tokens = tokenize("\\|");
            _(tokens).should_equal({ descending_edge_part(), vertical_edge_part() });
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_an_edge_name)
        {
            auto tokens = tokenize("\\(a)");
            _(tokens).should_equal({ descending_edge_part(), edge_name("a") });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_a_root_node)
        {
            auto tokens = tokenize("/[*]");
            _(tokens).should_equal({ ascending_edge_part(), root_node() });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_a_named_node)
        {
            auto tokens = tokenize("/[a]");
            _(tokens).should_equal({ ascending_edge_part(), named_node("a") });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_a_descending_edge_part)
        {
            auto tokens = tokenize("/\\");
            _(tokens).should_equal({ ascending_edge_part(), descending_edge_part() });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_an_ascending_edge_part)
        {
            auto tokens = tokenize("//");
            _(tokens).should_equal({ ascending_edge_part(), ascending_edge_part() });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_a_vertical_edge_part)
        {
            auto tokens = tokenize("/|");
            _(tokens).should_equal({ ascending_edge_part(), vertical_edge_part() });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_an_edge_name)
        {
            auto tokens = tokenize("/(a)");
            _(tokens).should_equal({ ascending_edge_part(), edge_name("a") });
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_a_root_node)
        {
            auto tokens = tokenize("|[*]");
            _(tokens).should_equal({ vertical_edge_part(), root_node() });
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_a_named_node)
        {
            auto tokens = tokenize("|[a]");
            _(tokens).should_equal({ vertical_edge_part(), named_node("a") });
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_a_descending_edge_part)
        {
            auto tokens = tokenize("|\\");
            _(tokens).should_equal({ vertical_edge_part(), descending_edge_part() });
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_an_ascending_edge_part)
        {
            auto tokens = tokenize("|/");
            _(tokens).should_equal({ vertical_edge_part(), ascending_edge_part() });
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_a_vertical_edge_part)
        {
            auto tokens = tokenize("||");
            _(tokens).should_equal({ vertical_edge_part(), vertical_edge_part() });
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_an_edge_name)
        {
            auto tokens = tokenize("|(a)");
            _(tokens).should_equal({ vertical_edge_part(), edge_name("a") });
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_a_root_node)
        {
            auto tokens = tokenize("(a)[*]");
            _(tokens).should_equal({ edge_name("a"), root_node() });
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_a_named_node)
        {
            auto tokens = tokenize("(a)[b]");
            _(tokens).should_equal({ edge_name("a"), named_node("b") });
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_a_descending_edge_part)
        {
            auto tokens = tokenize("(a)\\");
            _(tokens).should_equal({ edge_name("a"), descending_edge_part() });
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_an_ascending_edge_part)
        {
            auto tokens = tokenize("(a)/");
            _(tokens).should_equal({ edge_name("a"), ascending_edge_part() });
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_a_vertical_edge_part)
        {
            auto tokens = tokenize("(a)|");
            _(tokens).should_equal({ edge_name("a"), vertical_edge_part() });
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_an_edge_name)
        {
            auto tokens = tokenize("(a)(b)");
            _(tokens).should_equal({ edge_name("a"), edge_name("b") });
        }

    };
}}
