#include "CppUnitTest.h"
#include <cctype>
#include <string>
#include <vector>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace algo
{
    struct token
    {
        enum toktype { root_node, named_node, edge_name, horizontal_edge, ascending_edge_part, descending_edge_part, vertical_edge_part };
        toktype type;
        string name;
    };

    bool operator==(const token& lhs, const token& rhs)
    {
        return lhs.type == rhs.type
            && lhs.name == rhs.name;
    }

    struct ascii_tree
    {
        static vector<token> tokenize(const string& s)
        {
            vector<token> tokens;
            enum { none, open_square_brace, close_square_brace, asterisk, dash, name_char } prev = none;
            size_t marker = 0, marked_length = 0;

            for (size_t i = 0; i < s.size(); ++i)
            {
                auto ch = s[i];

                if (ch == '[')
                {
                    prev = open_square_brace;
                }
                else if (ch == ']')
                {
                    if (prev == asterisk)
                    {
                        tokens.emplace_back(token { token::root_node, "" });
                    }
                    else
                    {
                        tokens.emplace_back(token { token::named_node, s.substr(marker, marked_length) });
                    }

                    prev = close_square_brace;
                }
                else if (ch == '*')
                {
                    prev = asterisk;
                }
                else if (ch == '-')
                {
                    if (prev == name_char)
                    {
                        tokens.emplace_back(token { token::horizontal_edge, s.substr(marker, marked_length) });
                    }

                    prev = dash;
                }
                else if (ch == '/')
                {
                    tokens.emplace_back(token { token::ascending_edge_part, "" });
                }
                else if (ch == '\\')
                {
                    tokens.emplace_back(token { token::descending_edge_part, "" });
                }
                else if (ch == '|')
                {
                    tokens.emplace_back(token { token::vertical_edge_part, "" });
                }
                else if (isalnum(ch) || ch == '_')
                {
                    if (prev != name_char)
                    {
                        marker = i;
                        marked_length = 0;
                    }

                    prev = name_char;
                    ++marked_length;
                }
            }

            if (prev == name_char)
            {
                tokens.emplace_back(token { token::edge_name, s });
            }
            else if (prev == dash)
            {
                tokens.emplace_back(token { token::horizontal_edge, s.substr(marker, marked_length) });
            }

            return tokens;
        }
    };
}

namespace Microsoft { namespace VisualStudio { namespace CppUnitTestFramework
{
    template<>
    wstring ToString<algo::token::toktype>(const algo::token::toktype& type)
    {
        switch (type)
        {
        case algo::token::root_node:
            return L"root_node";
        case algo::token::named_node:
            return L"named_node";
        case algo::token::edge_name:
            return L"edge_name";
        case algo::token::horizontal_edge:
            return L"horizontal_edge";
        case algo::token::ascending_edge_part:
            return L"ascending_edge_part";
        case algo::token::descending_edge_part:
            return L"descending_edge_part";
        case algo::token::vertical_edge_part:
            return L"vertical_edge_part";
        default:
            return L"unknown token";
        }
    }
}}}

namespace algo { namespace spec
{
	TEST_CLASS(can_recognize_ascii_tree_tokens)
	{
	public:
		
        TEST_METHOD(should_not_recognize_any_tokens_in_an_empty_string)
        {
            auto tokens = ascii_tree::tokenize("");
            Assert::IsTrue(tokens.empty());
        }

		TEST_METHOD(should_recognize_a_root_node)
		{
            auto tokens = ascii_tree::tokenize("[*]");
            Assert::AreEqual(token::root_node, tokens.front().type);
        }

        TEST_METHOD(should_recognize_a_named_node)
        {
            auto tokens = ascii_tree::tokenize("[a]");
            Assert::AreEqual(token::named_node, tokens.front().type);
            Assert::AreEqual("a", tokens.front().name.c_str());
        }

        TEST_METHOD(should_recognize_an_edge_name)
        {
            auto tokens = ascii_tree::tokenize("a");
            Assert::AreEqual(token::edge_name, tokens.front().type);
            Assert::AreEqual("a", tokens.front().name.c_str());
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part)
        {
            auto tokens = ascii_tree::tokenize("/");
            Assert::AreEqual(token::ascending_edge_part, tokens.front().type);
        }

        TEST_METHOD(should_recognize_a_descending_edge_part)
        {
            auto tokens = ascii_tree::tokenize("\\");
            Assert::AreEqual(token::descending_edge_part, tokens.front().type);
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part)
        {
            auto tokens = ascii_tree::tokenize("|");
            Assert::AreEqual(token::vertical_edge_part, tokens.front().type);
        }

        TEST_METHOD(should_recognize_a_horizontal_edge)
        {
            auto tokens = ascii_tree::tokenize("-a-");
            Assert::AreEqual(token::horizontal_edge, tokens.front().type);
            Assert::AreEqual("a", tokens.front().name.c_str());
        }

        TEST_METHOD(should_recognize_more_than_one_ascii_tree_token)
        {
            auto expected_tokens =
            {
                token { token::root_node, "" }, 
                token { token::horizontal_edge, "a" }, 
                token { token::named_node, "b" }
            };

            auto tokens = ascii_tree::tokenize("[*]-a-[b]");

            auto mismatch_pair = std::mismatch(expected_tokens.begin(), expected_tokens.end(), tokens.begin());
            Assert::IsTrue(expected_tokens.end() == mismatch_pair.first && tokens.end() == mismatch_pair.second);
        }

    };
}}
