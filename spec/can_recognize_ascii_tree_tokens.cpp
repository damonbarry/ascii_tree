#include "grammar.hpp"
#include <CppUnitTest.h>
#include <algorithm>
#include <utility>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace Microsoft { namespace VisualStudio { namespace CppUnitTestFramework
{
    using namespace ascii_tree;

    template<>
    wstring ToString<token::toktype>(const token::toktype& type)
    {
        switch (type)
        {
        case token::root_node:
            return L"root_node";
        case token::named_node:
            return L"named_node";
        case token::edge_name:
            return L"edge_name";
        case token::horizontal_edge:
            return L"horizontal_edge";
        case token::ascending_edge_part:
            return L"ascending_edge_part";
        case token::descending_edge_part:
            return L"descending_edge_part";
        case token::vertical_edge_part:
            return L"vertical_edge_part";
        default:
            return L"unknown token";
        }
    }
}}}

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

    struct root_node : public token
    {
        root_node() : token(token::root_node, "") {}
    };

    struct named_node : public token
    {
        explicit named_node(string&& name) : token(token::named_node, std::move(name)) {}
    };
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
            auto tokens = tokenize("[*]");
            _(tokens).should_equal({ root_node() });
        }

        TEST_METHOD(should_recognize_a_root_node_with_spaces)
        {
            auto tokens = tokenize(" [ * ] ");
            _(tokens).should_equal({ root_node() });
        }

        TEST_METHOD(should_recognize_a_named_node)
        {
            const string all_chars = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
            auto tokens = tokenize("[" + all_chars + "]");
            _(tokens).should_equal({ named_node(all_chars.c_str()) });
        }

        TEST_METHOD(should_recognize_a_named_node_with_spaces)
        {
            auto tokens = tokenize(" [ a ] ");
            _(tokens).should_equal({ named_node("a") });
        }

        TEST_METHOD(should_recognize_an_edge_name)
        {
            auto tokens = tokenize("(a)");
            _(tokens).should_equal({ { token::edge_name, "a" } });
        }

        TEST_METHOD(should_recognize_an_edge_name_with_spaces)
        {
            auto tokens = tokenize(" ( a ) ");
            _(tokens).should_equal({ { token::edge_name, "a" } });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part)
        {
            auto tokens = tokenize("/");
            _(tokens).should_equal({ { token::ascending_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_with_spaces)
        {
            auto tokens = tokenize(" / ");
            _(tokens).should_equal({ { token::ascending_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_a_descending_edge_part)
        {
            auto tokens = tokenize("\\");
            _(tokens).should_equal({ { token::descending_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_with_spaces)
        {
            auto tokens = tokenize(" \\ ");
            _(tokens).should_equal({ { token::descending_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part)
        {
            auto tokens = tokenize("|");
            _(tokens).should_equal({ { token::vertical_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_with_spaces)
        {
            auto tokens = tokenize(" | ");
            _(tokens).should_equal({ { token::vertical_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_a_horizontal_edge)
        {
            auto tokens = tokenize("-(a)-");
            _(tokens).should_equal({ { token::horizontal_edge, "a" } });
        }

        TEST_METHOD(should_recognize_a_horizontal_edge_with_spaces)
        {
            auto tokens = tokenize(" - ( a ) - ");
            _(tokens).should_equal({ { token::horizontal_edge, "a" } });
        }

        TEST_METHOD(should_recognize_a_horizontal_edge_with_uninterrupted_sequences_of_dashes)
        {
            auto tokens = tokenize("---(a)--");
            _(tokens).should_equal({ { token::horizontal_edge, "a" } });
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_a_named_node)
        {
            auto tokens = tokenize("[*][a]");
            _(tokens).should_equal({ root_node(), named_node("a") });
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_a_horizontal_edge)
        {
            auto tokens = tokenize("[*]-(a)-");
            _(tokens).should_equal({ root_node(), { token::horizontal_edge, "a" } });
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_a_descending_edge_part)
        {
            auto tokens = tokenize("[*]\\");
            _(tokens).should_equal({ root_node(), { token::descending_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_an_ascending_edge_part)
        {
            auto tokens = tokenize("[*]/");
            _(tokens).should_equal({ root_node(), { token::ascending_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_a_vertical_edge_part)
        {
            auto tokens = tokenize("[*]|");
            _(tokens).should_equal({ root_node(), { token::vertical_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_an_edge_name)
        {
            auto tokens = tokenize("[*](a)");
            _(tokens).should_equal({ root_node(), { token::edge_name, "a" } });
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
            _(tokens).should_equal({ named_node("a"), { token::horizontal_edge, "b" } });
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_a_descending_edge_part)
        {
            auto tokens = tokenize("[a]\\");
            _(tokens).should_equal({ named_node("a"), { token::descending_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_an_ascending_edge_part)
        {
            auto tokens = tokenize("[a]/");
            _(tokens).should_equal({ named_node("a"), { token::ascending_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_a_vertical_edge_part)
        {
            auto tokens = tokenize("[a]|");
            _(tokens).should_equal({ named_node("a"), { token::vertical_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_an_edge_name)
        {
            auto tokens = tokenize("[a](b)");
            _(tokens).should_equal({ named_node("a"), { token::edge_name, "b" } });
        }

        TEST_METHOD(should_recognize_a_horizontal_edge_next_to_a_root_node)
        {
            auto tokens = tokenize("-(a)-[*]");
            _(tokens).should_equal({ { token::horizontal_edge, "a" }, root_node() });
        }

        TEST_METHOD(should_recognize_a_horizontal_edge_next_to_a_named_node)
        {
            auto tokens = tokenize("-(a)-[b]");
            _(tokens).should_equal({ { token::horizontal_edge, "a" }, named_node("b") });
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_a_root_node)
        {
            auto tokens = tokenize("\\[*]");
            _(tokens).should_equal({ { token::descending_edge_part, "" }, root_node() });
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_a_named_node)
        {
            auto tokens = tokenize("\\[a]");
            _(tokens).should_equal({ { token::descending_edge_part, "" }, named_node("a") });
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_a_descending_edge_part)
        {
            auto tokens = tokenize("\\\\");
            _(tokens).should_equal({ { token::descending_edge_part, "" }, { token::descending_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_an_ascending_edge_part)
        {
            auto tokens = tokenize("\\/");
            _(tokens).should_equal({ { token::descending_edge_part, "" }, { token::ascending_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_a_vertical_edge_part)
        {
            auto tokens = tokenize("\\|");
            _(tokens).should_equal({ { token::descending_edge_part, "" }, { token::vertical_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_an_edge_name)
        {
            auto tokens = tokenize("\\(a)");
            _(tokens).should_equal({ { token::descending_edge_part, "" }, { token::edge_name, "a" } });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_a_root_node)
        {
            auto tokens = tokenize("/[*]");
            _(tokens).should_equal({ { token::ascending_edge_part, "" }, root_node() });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_a_named_node)
        {
            auto tokens = tokenize("/[a]");
            _(tokens).should_equal({ { token::ascending_edge_part, "" }, named_node("a") });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_a_descending_edge_part)
        {
            auto tokens = tokenize("/\\");
            _(tokens).should_equal({ { token::ascending_edge_part, "" }, { token::descending_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_an_ascending_edge_part)
        {
            auto tokens = tokenize("//");
            _(tokens).should_equal({ { token::ascending_edge_part, "" }, { token::ascending_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_a_vertical_edge_part)
        {
            auto tokens = tokenize("/|");
            _(tokens).should_equal({ { token::ascending_edge_part, "" }, { token::vertical_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_an_edge_name)
        {
            auto tokens = tokenize("/(a)");
            _(tokens).should_equal({ { token::ascending_edge_part, "" }, { token::edge_name, "a" } });
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_a_root_node)
        {
            auto tokens = tokenize("|[*]");
            _(tokens).should_equal({ { token::vertical_edge_part, "" }, root_node() });
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_a_named_node)
        {
            auto tokens = tokenize("|[a]");
            _(tokens).should_equal({ { token::vertical_edge_part, "" }, named_node("a") });
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_a_descending_edge_part)
        {
            auto tokens = tokenize("|\\");
            _(tokens).should_equal({ { token::vertical_edge_part, "" }, { token::descending_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_an_ascending_edge_part)
        {
            auto tokens = tokenize("|/");
            _(tokens).should_equal({ { token::vertical_edge_part, "" }, { token::ascending_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_a_vertical_edge_part)
        {
            auto tokens = tokenize("||");
            _(tokens).should_equal({ { token::vertical_edge_part, "" }, { token::vertical_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_an_edge_name)
        {
            auto tokens = tokenize("|(a)");
            _(tokens).should_equal({ { token::vertical_edge_part, "" }, { token::edge_name, "a" } });
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_a_root_node)
        {
            auto tokens = tokenize("(a)[*]");
            _(tokens).should_equal({ { token::edge_name, "a" }, root_node() });
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_a_named_node)
        {
            auto tokens = tokenize("(a)[b]");
            _(tokens).should_equal({ { token::edge_name, "a" }, named_node("b") });
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_a_descending_edge_part)
        {
            auto tokens = tokenize("(a)\\");
            _(tokens).should_equal({ { token::edge_name, "a" }, { token::descending_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_an_ascending_edge_part)
        {
            auto tokens = tokenize("(a)/");
            _(tokens).should_equal({ { token::edge_name, "a" }, { token::ascending_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_a_vertical_edge_part)
        {
            auto tokens = tokenize("(a)|");
            _(tokens).should_equal({ { token::edge_name, "a" }, { token::vertical_edge_part, "" } });
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_an_edge_name)
        {
            auto tokens = tokenize("(a)(b)");
            _(tokens).should_equal({ { token::edge_name, "a" }, { token::edge_name, "b" } });
        }

    };
}}
