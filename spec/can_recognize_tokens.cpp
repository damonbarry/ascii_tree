#include "CppUnitTest.h"
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace algo
{
    template<class T, class Tokenize>
    class tokens
    {
        vector<T> tokens_;
        Tokenize tokenize_;
    public:
        explicit tokens(Tokenize fn) : tokenize_(fn) {}
        bool empty() const { return tokens_.size() == 0; }
        void from_string(string s)
        {
            tokens_.push_back(tokenize_(s));
        }
    };

    template<class T, class Tokenize>
    tokens<T, Tokenize> make_tokens(Tokenize&& fn) { return tokens<T, Tokenize>(std::forward<Tokenize>(fn)); }
}

namespace algo { namespace spec
{
    struct s {};
    auto alwaysReturnAToken = [](string&){ return s(); };

    TEST_CLASS(can_recognize_tokens)
	{
	public:

        TEST_METHOD(should_not_have_any_tokens_when_default_constructed)
		{
            auto testTokens = make_tokens<s>(alwaysReturnAToken);
            Assert::IsTrue(testTokens.empty());
		}

        TEST_METHOD(should_recognize_a_token)
        {
            auto testTokens = make_tokens<s>(alwaysReturnAToken);
            testTokens.from_string("s");
            Assert::IsFalse(testTokens.empty());
        }

	};
}}
