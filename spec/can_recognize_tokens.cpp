#include "CppUnitTest.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace algo
{
    template<class Traits>
    class tokens
    {
        vector<typename Traits::type> tokens_;
    public:
        bool empty() const { return tokens_.size() == 0; }
        void from_string(string s)
        {
            tokens_.push_back(Traits::tokenize(s));
        }
    };
}

namespace algo { namespace spec
{
    struct test_token_traits
    {
        struct s {};
        typedef s type;
        static s tokenize(const string&) { return s(); }
    };

    typedef tokens<test_token_traits> test_tokens;

    TEST_CLASS(can_recognize_tokens)
	{
	public:

        TEST_METHOD(should_not_have_any_tokens_when_default_constructed)
		{
            test_tokens testTokens;
            Assert::IsTrue(testTokens.empty());
		}

        TEST_METHOD(should_recognize_a_token_in_a_string)
        {
            test_tokens testTokens;
            testTokens.from_string("s");
            Assert::IsFalse(testTokens.empty());
        }

	};
}}
