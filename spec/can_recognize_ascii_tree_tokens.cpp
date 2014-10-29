#include "grammar.hpp"
#include <CppUnitTest.h>
#include <algorithm>
#include <sstream>
#include <cctype>
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

namespace ascii_tree { namespace spec
{
    TEST_CLASS(can_recognize_ascii_tree_tokens)
    {
        void tokens_should_match_(std::initializer_list<token> expected, vector<token>& actual)
        {
            auto mismatch_pair = std::mismatch(expected.begin(), expected.end(), actual.begin());

            if (mismatch_pair.first != expected.end() || mismatch_pair.second != actual.end())
            {
                wstring expectedName(mismatch_pair.first->name.begin(), mismatch_pair.first->name.end());
                wstring actualName(mismatch_pair.second->name.begin(), mismatch_pair.second->name.end());

                wstring message = L"Expected: " + ToString(mismatch_pair.first->type) + L" \"" + expectedName + L"\" "
                    + L"Actual: " + ToString(mismatch_pair.second->type) + L" \"" + actualName + L"\"";

                Assert::Fail(message.c_str());
            }
        }

    public:
        
        TEST_METHOD(should_not_recognize_any_tokens_in_an_empty_string)
        {
            auto tokens = tokenize("");
            Assert::IsTrue(tokens.empty());
        }

        TEST_METHOD(should_recognize_a_root_node)
        {
            auto tokens = tokenize("[*]");
            Assert::AreEqual(token::root_node, tokens.front().type);
        }

        TEST_METHOD(should_recognize_a_root_node_with_spaces)
        {
            auto tokens = tokenize(" [ * ]");
            Assert::AreEqual(token::root_node, tokens.front().type);
        }

        TEST_METHOD(should_recognize_a_named_node)
        {
            const string all_chars = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
            auto tokens = tokenize("[" + all_chars + "]");
            Assert::AreEqual(token::named_node, tokens.front().type);
            Assert::AreEqual(all_chars.c_str(), tokens.front().name.c_str());
        }

        TEST_METHOD(should_recognize_a_named_node_with_spaces)
        {
            auto tokens = tokenize(" [ a ]");
            Assert::AreEqual(token::named_node, tokens.front().type);
            Assert::AreEqual("a", tokens.front().name.c_str());
        }

        TEST_METHOD(should_recognize_an_edge_name)
        {
            auto tokens = tokenize("(a)");
            Assert::AreEqual(token::edge_name, tokens.front().type);
            Assert::AreEqual("a", tokens.front().name.c_str());
        }

        TEST_METHOD(should_recognize_an_edge_name_with_spaces)
        {
            auto tokens = tokenize("( a )");
            Assert::AreEqual(token::edge_name, tokens.front().type);
            Assert::AreEqual("a", tokens.front().name.c_str());
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part)
        {
            auto tokens = tokenize("/");
            Assert::AreEqual(token::ascending_edge_part, tokens.front().type);
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_with_spaces)
        {
            auto tokens = tokenize(" / ");
            Assert::AreEqual(token::ascending_edge_part, tokens.front().type);
        }

        TEST_METHOD(should_recognize_a_descending_edge_part)
        {
            auto tokens = tokenize("\\");
            Assert::AreEqual(token::descending_edge_part, tokens.front().type);
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_with_spaces)
        {
            auto tokens = tokenize(" \\ ");
            Assert::AreEqual(token::descending_edge_part, tokens.front().type);
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part)
        {
            auto tokens = tokenize("|");
            Assert::AreEqual(token::vertical_edge_part, tokens.front().type);
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_with_spaces)
        {
            auto tokens = tokenize(" | ");
            Assert::AreEqual(token::vertical_edge_part, tokens.front().type);
        }

        TEST_METHOD(should_recognize_a_horizontal_edge)
        {
            auto tokens = tokenize("-a-");
            Assert::AreEqual(token::horizontal_edge, tokens.front().type);
            Assert::AreEqual("a", tokens.front().name.c_str());
        }

        TEST_METHOD(should_recognize_a_horizontal_edge_with_spaces)
        {
            auto tokens = tokenize(" - a - ");
            Assert::AreEqual(token::horizontal_edge, tokens.front().type);
            Assert::AreEqual("a", tokens.front().name.c_str());
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_a_named_node)
        {
            auto tokens = tokenize("[*][a]");
            tokens_should_match_({ { token::root_node, "" }, { token::named_node, "a" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_a_horizontal_edge)
        {
            auto tokens = tokenize("[*]-a-");
            tokens_should_match_({ { token::root_node, "" }, { token::horizontal_edge, "a" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_a_descending_edge_part)
        {
            auto tokens = tokenize("[*]\\");
            tokens_should_match_({ { token::root_node, "" }, { token::descending_edge_part, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_an_ascending_edge_part)
        {
            auto tokens = tokenize("[*]/");
            tokens_should_match_({ { token::root_node, "" }, { token::ascending_edge_part, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_a_vertical_edge_part)
        {
            auto tokens = tokenize("[*]|");
            tokens_should_match_({ { token::root_node, "" }, { token::vertical_edge_part, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_an_edge_name)
        {
            auto tokens = tokenize("[*](a)");
            tokens_should_match_({ { token::root_node, "" }, { token::edge_name, "a" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_a_root_node)
        {
            auto tokens = tokenize("[a][*]");
            tokens_should_match_({ { token::named_node, "a" }, { token::root_node, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_a_named_node)
        {
            auto tokens = tokenize("[a][b]");
            tokens_should_match_({ { token::named_node, "a" }, { token::named_node, "b" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_a_horizontal_edge)
        {
            auto tokens = tokenize("[a]-b-");
            tokens_should_match_({ { token::named_node, "a" }, { token::horizontal_edge, "b" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_a_descending_edge_part)
        {
            auto tokens = tokenize("[a]\\");
            tokens_should_match_({ { token::named_node, "a" }, { token::descending_edge_part, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_an_ascending_edge_part)
        {
            auto tokens = tokenize("[a]/");
            tokens_should_match_({ { token::named_node, "a" }, { token::ascending_edge_part, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_a_vertical_edge_part)
        {
            auto tokens = tokenize("[a]|");
            tokens_should_match_({ { token::named_node, "a" }, { token::vertical_edge_part, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_an_edge_name)
        {
            auto tokens = tokenize("[a](b)");
            tokens_should_match_({ { token::named_node, "a" }, { token::edge_name, "b" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_horizontal_edge_next_to_a_root_node)
        {
            auto tokens = tokenize("-a-[*]");
            tokens_should_match_({ { token::horizontal_edge, "a" }, { token::root_node, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_horizontal_edge_next_to_a_named_node)
        {
            auto tokens = tokenize("-a-[b]");
            tokens_should_match_({ { token::horizontal_edge, "a" }, { token::named_node, "b" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_a_root_node)
        {
            auto tokens = tokenize("\\[*]");
            tokens_should_match_({ { token::descending_edge_part, "" }, { token::root_node, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_a_named_node)
        {
            auto tokens = tokenize("\\[a]");
            tokens_should_match_({ { token::descending_edge_part, "" }, { token::named_node, "a" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_a_descending_edge_part)
        {
            auto tokens = tokenize("\\\\");
            tokens_should_match_({ { token::descending_edge_part, "" }, { token::descending_edge_part, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_an_ascending_edge_part)
        {
            auto tokens = tokenize("\\/");
            tokens_should_match_({ { token::descending_edge_part, "" }, { token::ascending_edge_part, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_a_vertical_edge_part)
        {
            auto tokens = tokenize("\\|");
            tokens_should_match_({ { token::descending_edge_part, "" }, { token::vertical_edge_part, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_an_edge_name)
        {
            auto tokens = tokenize("\\(a)");
            tokens_should_match_({ { token::descending_edge_part, "" }, { token::edge_name, "a" } }, tokens);
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_a_root_node)
        {
            auto tokens = tokenize("/[*]");
            tokens_should_match_({ { token::ascending_edge_part, "" }, { token::root_node, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_a_named_node)
        {
            auto tokens = tokenize("/[a]");
            tokens_should_match_({ { token::ascending_edge_part, "" }, { token::named_node, "a" } }, tokens);
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_a_descending_edge_part)
        {
            auto tokens = tokenize("/\\");
            tokens_should_match_({ { token::ascending_edge_part, "" }, { token::descending_edge_part, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_an_ascending_edge_part)
        {
            auto tokens = tokenize("//");
            tokens_should_match_({ { token::ascending_edge_part, "" }, { token::ascending_edge_part, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_a_vertical_edge_part)
        {
            auto tokens = tokenize("/|");
            tokens_should_match_({ { token::ascending_edge_part, "" }, { token::vertical_edge_part, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_an_edge_name)
        {
            auto tokens = tokenize("/(a)");
            tokens_should_match_({ { token::ascending_edge_part, "" }, { token::edge_name, "a" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_a_root_node)
        {
            auto tokens = tokenize("|[*]");
            tokens_should_match_({ { token::vertical_edge_part, "" }, { token::root_node, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_a_named_node)
        {
            auto tokens = tokenize("|[a]");
            tokens_should_match_({ { token::vertical_edge_part, "" }, { token::named_node, "a" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_a_descending_edge_part)
        {
            auto tokens = tokenize("|\\");
            tokens_should_match_({ { token::vertical_edge_part, "" }, { token::descending_edge_part, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_an_ascending_edge_part)
        {
            auto tokens = tokenize("|/");
            tokens_should_match_({ { token::vertical_edge_part, "" }, { token::ascending_edge_part, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_a_vertical_edge_part)
        {
            auto tokens = tokenize("||");
            tokens_should_match_({ { token::vertical_edge_part, "" }, { token::vertical_edge_part, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_an_edge_name)
        {
            auto tokens = tokenize("|(a)");
            tokens_should_match_({ { token::vertical_edge_part, "" }, { token::edge_name, "a" } }, tokens);
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_a_root_node)
        {
            auto tokens = tokenize("(a)[*]");
            tokens_should_match_({ { token::edge_name, "a" }, { token::root_node, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_a_named_node)
        {
            auto tokens = tokenize("(a)[b]");
            tokens_should_match_({ { token::edge_name, "a" }, { token::named_node, "b" } }, tokens);
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_a_descending_edge_part)
        {
            auto tokens = tokenize("(a)\\");
            tokens_should_match_({ { token::edge_name, "a" }, { token::descending_edge_part, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_an_ascending_edge_part)
        {
            auto tokens = tokenize("(a)/");
            tokens_should_match_({ { token::edge_name, "a" }, { token::ascending_edge_part, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_a_vertical_edge_part)
        {
            auto tokens = tokenize("(a)|");
            tokens_should_match_({ { token::edge_name, "a" }, { token::vertical_edge_part, "" } }, tokens);
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_an_edge_name)
        {
            auto tokens = tokenize("(a)(b)");
            tokens_should_match_({ { token::edge_name, "a" }, { token::edge_name, "b" } }, tokens);
        }

    };
}}
