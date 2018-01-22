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
        syntax_tree tree({ { root_node(position(2)) } });
        auto result = tree.analyze();
        _(result).should_be(root_node());
    }

    TEST_CASE("should generate a tree with root horizontal edge leaf", "[can generate a syntax tree]")
    {
        position p1 = position("123", 0), p2 = position::from(p1, 0, 1), p3 = position::from(p2, 0, 1);
        syntax_tree tree({ { root_node(p1), horizontal_edge("e", p2), named_node("n", p3) } });
        auto result = tree.analyze();
        _(result).should_have_node_along_edge("n", "e");
    }

    TEST_CASE("should generate the same tree regardless of token order", "[can generate a syntax tree]")
    {
        auto root = root_node(position("123", 0));
        auto edge = horizontal_edge("e", position::from(root.position, 0, 1));
        auto node = named_node("n", position::from(root.position, 0, 2));

        syntax_tree tree012{ { root, edge, node } };
        syntax_tree tree021{ { root, node, edge } };
        syntax_tree tree102{ { edge, root, node } };
        syntax_tree tree120{ { edge, node, root } };
        syntax_tree tree201{ { node, root, edge } };
        syntax_tree tree210{ { node, edge, root } };

        _(tree012.analyze()).should_equal(tree021.analyze());
        _(tree012.analyze()).should_equal(tree102.analyze());
        _(tree012.analyze()).should_equal(tree120.analyze());
        _(tree012.analyze()).should_equal(tree201.analyze());
        _(tree012.analyze()).should_equal(tree210.analyze());
    }

    TEST_CASE("should generate a tree with leaf horizontal edge root", "[can generate a syntax tree]")
    {
        position p1 = position("123", 0), p2 = position::from(p1, 0, 1), p3 = position::from(p2, 0, 1);
        syntax_tree tree({ { named_node("n", p1), horizontal_edge("e", p2), root_node(p3) } });
        auto result = tree.analyze();
        _(result).should_have_node_along_edge("n", "e");
    }

    TEST_CASE("should generate a tree with root vertical edge leaf", "[can generate a syntax tree]")
    {
        auto grid = make_grid(grid_type { "5", "4", "3", "2", "1" });
        syntax_tree tree({ {
            root_node(position(grid, 4, 0)),
            vertical_edge_part(position(grid, 3, 0)),
            edge_name("e", position(grid, 2, 0)),
            vertical_edge_part(position(grid, 1, 0)),
            named_node("n", position(grid, 0, 0))
        } });
        auto result = tree.analyze();
        _(result).should_have_node_along_edge("n", "e");
    }

    TEST_CASE("should generate a tree with leaf vertical edge root", "[can generate a syntax tree]")
    {
        auto grid = make_grid(grid_type { "1", "2", "3", "4", "5" });
        syntax_tree tree({ {
            root_node(position(grid, 0, 0)),
            vertical_edge_part(position(grid, 1, 0)),
            edge_name("e", position(grid, 2, 0)),
            vertical_edge_part(position(grid, 3, 0)),
            named_node("n", position(grid, 4, 0))
        } });
        auto result = tree.analyze();
        _(result).should_have_node_along_edge("n", "e");
    }

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
