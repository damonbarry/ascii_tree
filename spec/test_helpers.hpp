#include "grammar.hpp"
#include <CppUnitTest.h>
#include <string>

namespace Microsoft { namespace VisualStudio { namespace CppUnitTestFramework
{
    using namespace ascii_tree;

    template<>
    inline std::wstring ToString<token::toktype>(const token::toktype& type)
    {
        switch (type)
        {
        case token::root_node:
            return L"root_node";
        case token::named_node:
            return L"named_node";
        case token::edge_name:
            return L"edge_name";
        case token::horizontal_edge:
            return L"horizontal_edge";
        case token::ascending_edge_part:
            return L"ascending_edge_part";
        case token::descending_edge_part:
            return L"descending_edge_part";
        case token::vertical_edge_part:
            return L"vertical_edge_part";
        default:
            return L"unknown token";
        }
    }

    template<>
    inline std::wstring ToString<token>(const token& tok)
    {
        std::wstring name(tok.name.begin(), tok.name.end());
        return ToString(tok.type) + L" " + name;
    }
}}}

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
