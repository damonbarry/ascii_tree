#include "CppUnitTest.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace algo
{
    struct token
    {
        enum toktype { root_node, named_node, edge_name };
        toktype type;
        string name;
    };

    bool operator==(const token& lhs, const token& rhs)
    {
        return true;
    }

    struct ascii_tree
    {
        static vector<token> tokenize(const string& s)
        {
            enum { none, open_square_brace, asterisk, edge_name } prev = none;

            for (auto ch : s)
            {
                if (ch == '[')
                {
                    prev = open_square_brace;
                }
                else if (ch == ']')
                {
                    if (prev == asterisk)
                    {
                        token newtok = { token::root_node, "" };
                        return vector<token>(1, newtok);
                    }
                    else
                    {
                        string name = s.substr(1, s.size() - 2);
                        token newtok = { token::named_node, name };
                        return vector<token>(1, newtok);
                    }
                }
                else if (ch == '*')
                {
                    prev = asterisk;
                }
                else
                {
                    if (prev == none)
                    {
                        prev = edge_name;
                    }
                }
            }

            return (prev == edge_name)
                ? vector<token>(1, token { token::edge_name, s })
                : vector<token>();
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

    };
}}
