#include "tokens.hpp"
#include <CppUnitTest.h>
#include <string>
#include <array>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace ascii_tree { namespace spec
{
    struct s { string val; };

    template<size_t N>
    struct test_token_traits
    {
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
            Assert::AreEqual(1U, testTokens.size());
        }

        TEST_METHOD(should_recognize_more_than_one_token_in_a_string)
        {
            test_tokens<3> testTokens;
            testTokens.from_string("irrelevant");
            Assert::AreEqual(3U, testTokens.size());
        }

        TEST_METHOD(should_accumulate_tokens_from_successive_strings)
        {
            test_tokens<2> testTokens(3);
            testTokens.from_string("irrelevant");
            Assert::AreEqual(5U, testTokens.size());
        }

        TEST_METHOD(should_be_able_to_enumerate_tokens)
        {
            test_tokens<0> testTokens(3);
            size_t i = 0;
            for (s tok : testTokens) { ++i; }
            Assert::AreEqual(3U, i);
        }

        TEST_METHOD(should_be_able_to_enumerate_const_tokens_from_a_const_container)
        {
            const test_tokens<0> testTokens(3);
            size_t i = 0;
            for (const s tok : testTokens) { ++i; }
            Assert::AreEqual(3U, i);
        }

        TEST_METHOD(should_be_able_to_enumerate_const_tokens_from_a_non_const_container)
        {
            typedef test_tokens<0>::const_iterator const_iter;
            test_tokens<0> testTokens(3);
            size_t i = 0;
            for (const_iter it = testTokens.cbegin(); it != testTokens.cend(); ++it) { ++i; }
            Assert::AreEqual(3U, i);
        }

        TEST_METHOD(should_be_able_to_access_tokens_by_their_index)
        {
            test_tokens<0> testTokens = { { "one" }, { "two" }, { "three" } };
            Assert::AreEqual("one", testTokens[0].val.c_str());
            Assert::AreEqual("two", testTokens[1].val.c_str());
            Assert::AreEqual("three", testTokens[2].val.c_str());
        }

        TEST_METHOD(should_be_able_to_access_tokens_from_a_const_container_by_ther_index)
        {
            const test_tokens<0> testTokens = { { "one" } };
            Assert::AreEqual("one", testTokens[0].val.c_str());
        }

    };
}}
