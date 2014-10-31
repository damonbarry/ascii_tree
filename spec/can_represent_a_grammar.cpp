#include "grammar.hpp"
#include <CppUnitTest.h>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace Microsoft { namespace VisualStudio { namespace CppUnitTestFramework
{
    using namespace ascii_tree;

    template<>
    wstring ToString<terminal>(const terminal& term)
    {
        switch (term)
        {
        case none:
            return L"none";
        case open_square_brace:
            return L"open_square_brace";
        case close_square_brace:
            return L"close_square_brace";
        case asterisk:
            return L"asterisk";
        case dash:
            return L"dash";
        case open_paren:
            return L"open_paren";
        case close_paren:
            return L"close_paren";
        case name_char:
            return L"name_char";
        case slash:
            return L"slash";
        case backslash:
            return L"backslash";
        case pipe:
            return L"pipe";
        default:
            return L"unknown char";
        }
    }
}}}

namespace ascii_tree { namespace spec
{
    TEST_CLASS(can_represent_a_grammar)
    {
    public:
        TEST_METHOD(should_recognize_an_open_square_brace)
        {
            terminal term = to_terminal('[');
            Assert::AreEqual(open_square_brace, term);
        }

        TEST_METHOD(should_recognize_an_asterisk)
        {
            terminal term = to_terminal('*');
            Assert::AreEqual(asterisk, term);
        }

        //TEST_METHOD(should_accept_a_terminal_when_it_matches_the_expected_value)
        //{
        //    grammar g("*");
        //    bool accepted = g.accept(asterisk);
        //    Assert::IsTrue(accepted);
        //}

    };
}}
