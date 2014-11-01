#include "grammar.hpp"
#include <CppUnitTest.h>
#include <string>

namespace ascii_tree { namespace spec
{

    template<typename Fn>
    void should_throw(const ascii_tree::ascii_tree_parse_exception& expected, Fn fn)
    {
        namespace cpput = Microsoft::VisualStudio::CppUnitTestFramework;

        cpput::Assert::ExpectException<ascii_tree::ascii_tree_parse_exception>([&]
        {
            try
            {
                fn();
            }
            catch (ascii_tree::ascii_tree_parse_exception& actual)
            {
                cpput::Assert::AreEqual(expected.s.c_str(), actual.s.c_str(),
                    L"value of ascii_tree_parse_exception::s is wrong");
                cpput::Assert::AreEqual(expected.pos, actual.pos,
                    L"value of ascii_tree_parse_exception::pos is wrong");
                throw;
            }
        });
    }

    template<typename Fn>
    void should_not_throw(Fn fn)
    {
        namespace cpput = Microsoft::VisualStudio::CppUnitTestFramework;

        try
        {
            fn();
        }
        catch (...)
        {
            cpput::Assert::Fail(L"Shouldn't have thrown an exception, but it did.");
        }
    }

}}
