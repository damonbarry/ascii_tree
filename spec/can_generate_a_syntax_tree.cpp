#include "syntax_tree.hpp"
#include "test_helpers.hpp"
#include <vector>

using namespace std;

namespace ascii_tree { namespace spec
{
    TEST_CASE("should throw when root node is absent", "[can generate a syntax tree]")
    {
        should_throw_(analyze_exception("missing root node"), []{
            syntax_tree({ { named_node("a") } }).analyze();
        });
    }

    TEST_CASE("should throw if more than one root node is present", "[can generate a syntax tree]")
    {
        should_throw_(analyze_exception("more than one root node"), []{
            syntax_tree({ { root_node(), root_node() } }).analyze();
        });
    }

    TEST_CASE("should point to the root node when it is present", "[can generate a syntax tree]")
    {
        syntax_tree tree({ { root_node(2) } });
        auto result = tree.analyze();
        _(result).should_be(root_node());
    }

    TEST_CASE("should generate a tree with root horizontal edge leaf", "[can generate a syntax tree]")
    {
        syntax_tree tree({ { root_node(0), horizontal_edge("e", 1), named_node("n", 2) } });
        auto result = tree.analyze();
        _(result).should_have_node_along_edge("n", "e");
    }

    TEST_CASE("should generate the same tree regardless of token order", "[can generate a syntax tree]")
    {
        syntax_tree tree1({ { root_node(0), horizontal_edge("e", 1), named_node("n", 2) } });
        syntax_tree tree2({ { named_node("n", 2), root_node(0), horizontal_edge("e", 1) } });
        _(tree1.analyze()).should_equal(tree2.analyze());
    }

    // TEST_CASE("should generate a tree with leaf horizontal edge root", "[can generate a syntax tree]")
    // {
    //     syntax_tree tree({ { named_node("n"), horizontal_edge("e"), root_node() } });
    //     auto result = tree.analyze();
    //     _(result).should_have_node_along_edge("n", "e");
    // }

    // TEST_CASE("should generate a tree with root vertical edge leaf", "[can generate a syntax tree]")
    // {
    //     syntax_tree tree({ { root_node(), vertical_edge_part(), edge_name("e"), vertical_edge_part(), named_node("n") } });
    //     auto result = tree.analyze();
    //     _(result).should_have_node_along_edge("n", "e");
    // }

    // TEST_CASE("should generate a tree with leaf vertical edge root", "[can generate a syntax tree]")
    // {
    //     syntax_tree tree({ { named_node("n"), vertical_edge_part(), edge_name("e"), vertical_edge_part(), root_node() } });
    //     auto result = tree.analyze();
    //     _(result).should_have_node_along_edge("n", "e");
    // }

    // TEST_CASE("should generate a tree with root ascending edge leaf", "[can generate a syntax tree]")
    // {
    //     syntax_tree tree({ { root_node(), ascending_edge_part(), edge_name("e"), ascending_edge_part(), named_node("n") } });
    //     auto result = tree.analyze();
    //     _(result).should_have_node_along_edge("n", "e");
    // }

    // TEST_CASE("should generate a tree with leaf ascending edge root", "[can generate a syntax tree]")
    // {
    //     syntax_tree tree({ { named_node("n"), ascending_edge_part(), edge_name("e"), ascending_edge_part(), root_node() } });
    //     auto result = tree.analyze();
    //     _(result).should_have_node_along_edge("n", "e");
    // }

    // TEST_CASE("should generate a tree with root descending edge leaf", "[can generate a syntax tree]")
    // {
    //     syntax_tree tree({ { root_node(), descending_edge_part(), edge_name("e"), descending_edge_part(), named_node("n") } });
    //     auto result = tree.analyze();
    //     _(result).should_have_node_along_edge("n", "e");
    // }

    // TEST_CASE("should generate a tree with leaf descending edge root", "[can generate a syntax tree]")
    // {
    //     syntax_tree tree({ { named_node("n"), descending_edge_part(), edge_name("e"), descending_edge_part(), root_node() } });
    //     auto result = tree.analyze();
    //     _(result).should_have_node_along_edge("n", "e");
    // }
}}
