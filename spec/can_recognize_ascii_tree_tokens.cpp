#include "CppUnitTest.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace algo
{
    struct token
    {
        enum toktype { root_node, named_node };
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
            if (s.empty())
            {
                return vector<token>();
            }
            else if (s == "[*]")
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
    };
}

namespace Microsoft { namespace VisualStudio { namespace CppUnitTestFramework
{
    template<>
    wstring ToString<algo::token::toktype>(const algo::token::toktype& type)
    {
        return (type == algo::token::root_node) ? L"root_node" : L"named_node";
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

    };
}}
