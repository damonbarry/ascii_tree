#include "tokens.hpp"
#include "test_helpers.hpp"
#include <string>
#include <array>

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
            _(testTokens.empty()).should_be_true();
        }

        TEST_METHOD(should_recognize_a_token_in_a_string)
        {
            test_tokens<1> testTokens;
            testTokens.from_string("irrelevant");
            _(testTokens.size()).should_be(1U);
        }

        TEST_METHOD(should_recognize_more_than_one_token_in_a_string)
        {
            test_tokens<3> testTokens;
            testTokens.from_string("irrelevant");
            _(testTokens.size()).should_be(3U);
        }

        TEST_METHOD(should_accumulate_tokens_from_successive_strings)
        {
            test_tokens<2> testTokens(3);
            testTokens.from_string("irrelevant");
            _(testTokens.size()).should_be(5U);
        }

        TEST_METHOD(should_be_able_to_enumerate_tokens)
        {
            test_tokens<0> testTokens(3);
            size_t i = 0;
            for (s tok : testTokens) { ++i; }
            _(i).should_be(3U);
        }

        TEST_METHOD(should_be_able_to_enumerate_const_tokens_from_a_const_container)
        {
            const test_tokens<0> testTokens(3);
            size_t i = 0;
            for (const s tok : testTokens) { ++i; }
            _(i).should_be(3U);
        }

        TEST_METHOD(should_be_able_to_enumerate_const_tokens_from_a_non_const_container)
        {
            typedef test_tokens<0>::const_iterator const_iter;
            test_tokens<0> testTokens(3);
            size_t i = 0;
            for (const_iter it = testTokens.cbegin(); it != testTokens.cend(); ++it) { ++i; }
            _(i).should_be(3U);
        }

        TEST_METHOD(should_be_able_to_access_tokens_by_their_index)
        {
            test_tokens<0> testTokens = { { "one" }, { "two" }, { "three" } };
            _(testTokens[0].val).should_be("one");
            _(testTokens[1].val).should_be("two");
            _(testTokens[2].val).should_be("three");
        }

        TEST_METHOD(should_be_able_to_access_tokens_from_a_const_container_by_ther_index)
        {
            const test_tokens<0> testTokens = { { "one" } };
            _(testTokens[0].val).should_be("one");
        }

    };
}}
