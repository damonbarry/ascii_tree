#include "CppUnitTest.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace algo
{
    struct root_node {};

    bool operator==(const root_node& lhs, const root_node& rhs)
    {
        return true;
    }

    struct ascii_tree
    {
        typedef root_node type;
        static vector<type> tokenize(const string& s)
        {
            return vector<type>(s.empty() ? 0 : 1);
        }
    };
}

namespace Microsoft { namespace VisualStudio { namespace CppUnitTestFramework
{
    template<>
    wstring ToString<algo::root_node>(const algo::root_node&) { return L"root_node"; }
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
            Assert::AreEqual(root_node(), tokens.front());
        }

	};
}}
