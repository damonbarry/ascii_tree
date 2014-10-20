#include "CppUnitTest.h"
#include <string>
#include <vector>
#include <array>

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
        size_t size() const { return tokens_.size(); }
        void from_string(string s)
        {
            auto newTokens = Traits::tokenize(s);
            tokens_.insert(tokens_.end(), newTokens.begin(), newTokens.end());
        }
    };
}

namespace algo { namespace spec
{
    template<size_t N>
    struct test_token_traits
    {
        struct s {};
        typedef s type;
        static array<s, N> tokenize(const string&) { return array<s, N>(); }
    };

    template<size_t N>
    using test_tokens = tokens<test_token_traits<N>>;

    TEST_CLASS(can_recognize_tokens)
	{
	public:

        TEST_METHOD(should_not_have_any_tokens_when_default_constructed)
		{
            test_tokens<0> testTokens;
            Assert::IsTrue(testTokens.empty());
		}

        TEST_METHOD(should_recognize_a_token_in_a_string)
        {
            test_tokens<1> testTokens;
            testTokens.from_string("irrelevant");
            Assert::IsFalse(testTokens.empty());
        }

        TEST_METHOD(should_recognize_more_than_one_token_in_a_string)
        {
            test_tokens<3> testTokens;
            testTokens.from_string("irrelevant");
            Assert::AreEqual(3U, testTokens.size());
        }

	};
}}
