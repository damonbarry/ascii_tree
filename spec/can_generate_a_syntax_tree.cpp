#include "syntax_tree.hpp"
#include "test_helpers.hpp"
#include <vector>

using namespace std;

namespace ascii_tree { namespace spec
{
    inline grid_ptr make_grid(size_t height, size_t width)
    {
        return std::make_shared<grid_type>(height, std::string(width, ' '));
    }

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
        syntax_tree tree({ { root_node(position()) } });
        auto result = tree.analyze();
        _(result).should_be(root_node());
    }

    TEST_CASE("should generate the same tree regardless of token order within vector", "[can generate a syntax tree]")
    {
        grid_ptr g = make_grid(1, 3);
        auto root = root_node(position(g, 0, 0));
        auto edge = horizontal_edge("e", position(g, 0, 1));
        auto node = named_node("n", position(g, 0, 2));

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

    TEST_CASE("should generate a tree with root horizontal edge leaf", "[can generate a syntax tree]")
    {
        grid_ptr g = make_grid(1, 3);
        syntax_tree tree({ {
            root_node(position(g, 0, 0)),
            horizontal_edge("e", position(g, 0, 1)),
            named_node("n", position(g, 0, 2))
        } });
        auto result = tree.analyze();
        _(result).should_have_node_along_edge("n", "e");
    }

    TEST_CASE("should generate a tree with leaf horizontal edge root", "[can generate a syntax tree]")
    {
        grid_ptr g = make_grid(1, 3);
        syntax_tree tree({ {
            named_node("n", position(g, 0, 0)),
            horizontal_edge("e", position(g, 0, 1)),
            root_node(position(g, 0, 2))
        } });
        auto result = tree.analyze();
        _(result).should_have_node_along_edge("n", "e");
    }

    TEST_CASE("should throw when horizontal edge has no node", "[can generate a syntax tree]")
    {
        grid_ptr g = make_grid(1, 2);
        syntax_tree tree({ {
            root_node(position(g, 0, 0)),
            horizontal_edge("e", position(g, 0, 1))
        } });
        should_throw_(analyze_exception("missing named_node at the end of a horizontal edge"), [&]{
            tree.analyze();
        });
    }

    TEST_CASE("should generate a tree with root vertical edge leaf", "[can generate a syntax tree]")
    {
        grid_ptr g = make_grid(5, 1);
        syntax_tree tree({ {
            root_node(position(g, 4, 0)),
            vertical_edge_part(position(g, 3, 0)),
            edge_name("e", position(g, 2, 0)),
            vertical_edge_part(position(g, 1, 0)),
            named_node("n", position(g, 0, 0))
        } });
        auto result = tree.analyze();
        _(result).should_have_node_along_edge("n", "e");
    }

    TEST_CASE("should generate a tree with leaf vertical edge root", "[can generate a syntax tree]")
    {
        auto g = make_grid(5, 1);
        syntax_tree tree({ {
            root_node(position(g, 0, 0)),
            vertical_edge_part(position(g, 1, 0)),
            edge_name("e", position(g, 2, 0)),
            vertical_edge_part(position(g, 3, 0)),
            named_node("n", position(g, 4, 0))
        } });
        auto result = tree.analyze();
        _(result).should_have_node_along_edge("n", "e");
    }

    TEST_CASE("should throw when vertical edge has no name", "[can generate a syntax tree]")
    {
        auto g = make_grid(4, 1);
        syntax_tree tree({ {
            root_node(position(g, 3, 0)),
            vertical_edge_part(position(g, 2, 0)),
            vertical_edge_part(position(g, 1, 0)),
            named_node("n", position(g, 0, 0))
        } });
        should_throw_(analyze_exception("expected edge_name"), [&]{
            tree.analyze();
        });
    }

    TEST_CASE("should throw when vertical edge does not continue after edge_name", "[can generate a syntax tree]")
    {
        auto g = make_grid(4, 1);
        syntax_tree tree({ {
            root_node(position(g, 3, 0)),
            vertical_edge_part(position(g, 2, 0)),
            edge_name("e", position(g, 1, 0)),
            named_node("n", position(g, 0, 0))
        } });
        should_throw_(analyze_exception("expected vertical_edge_part"), [&]{
            tree.analyze();
        });
    }

    TEST_CASE("should throw when vertical edge has no node", "[can generate a syntax tree]")
    {
        auto g = make_grid(4, 1);
        syntax_tree tree({ {
            root_node(position(g, 3, 0)),
            vertical_edge_part(position(g, 2, 0)),
            edge_name("e", position(g, 1, 0)),
            vertical_edge_part(position(g, 0, 0))
        } });
        should_throw_(analyze_exception("expected named_node"), [&]{
            tree.analyze();
        });
    }

    TEST_CASE("should generate a tree with root ascending edge leaf", "[can generate a syntax tree]")
    {
        auto g = make_grid(5, 5);
        syntax_tree tree({ {
            root_node(position(g, 0, 0)),
            ascending_edge_part(position(g, 1, 1)),
            edge_name("e", position(g, 2, 2)),
            ascending_edge_part(position(g, 3, 3)),
            named_node("n", position(g, 4, 4))
        } });
        auto result = tree.analyze();
        _(result).should_have_node_along_edge("n", "e");
    }

    TEST_CASE("should generate a tree with leaf ascending edge root", "[can generate a syntax tree]")
    {
        auto g = make_grid(5, 5);
        syntax_tree tree({ {
            named_node("n", position(g, 0, 0)),
            ascending_edge_part(position(g, 1, 1)),
            edge_name("e", position(g, 2, 2)),
            ascending_edge_part(position(g, 3, 3)),
            root_node(position(g, 4, 4))
        } });
        auto result = tree.analyze();
        _(result).should_have_node_along_edge("n", "e");
    }

    TEST_CASE("should throw when ascending edge has no name", "[can generate a syntax tree]")
    {
        auto g = make_grid(4, 4);
        syntax_tree tree({ {
            root_node(position(g, 0, 0)),
            ascending_edge_part(position(g, 1, 1)),
            ascending_edge_part(position(g, 2, 2)),
            named_node("n", position(g, 3, 3))
        } });
        should_throw_(analyze_exception("expected edge_name"), [&]{
            tree.analyze();
        });
    }

    TEST_CASE("should throw when ascending edge does not continue after edge_name", "[can generate a syntax tree]")
    {
        auto g = make_grid(4, 4);
        syntax_tree tree({ {
            root_node(position(g, 0, 0)),
            ascending_edge_part(position(g, 1, 1)),
            edge_name("e", position(g, 2, 2)),
            named_node("n", position(g, 3, 3))
        } });
        should_throw_(analyze_exception("expected ascending_edge_part"), [&]{
            tree.analyze();
        });
    }

    TEST_CASE("should throw when ascending edge has no node", "[can generate a syntax tree]")
    {
        auto g = make_grid(4, 4);
        syntax_tree tree({ {
            root_node(position(g, 0, 0)),
            ascending_edge_part(position(g, 1, 1)),
            edge_name("e", position(g, 2, 2)),
            ascending_edge_part(position(g, 3, 3))
        } });
        should_throw_(analyze_exception("expected named_node"), [&]{
            tree.analyze();
        });
    }

    TEST_CASE("should generate a tree with root descending edge leaf", "[can generate a syntax tree]")
    {
        auto g = make_grid(5, 5);
        syntax_tree tree({ {
            root_node(position(g, 4, 0)),
            descending_edge_part(position(g, 3, 1)),
            edge_name("e", position(g, 2, 2)),
            descending_edge_part(position(g, 1, 3)),
            named_node("n", position(g, 0, 4))
        } });
        auto result = tree.analyze();
        _(result).should_have_node_along_edge("n", "e");
    }

    TEST_CASE("should generate a tree with leaf descending edge root", "[can generate a syntax tree]")
    {
        auto g = make_grid(5, 5);
        syntax_tree tree({ {
            named_node("n", position(g, 4, 0)),
            descending_edge_part(position(g, 3, 1)),
            edge_name("e", position(g, 2, 2)),
            descending_edge_part(position(g, 1, 3)),
            root_node(position(g, 0, 4))
        } });
        auto result = tree.analyze();
        _(result).should_have_node_along_edge("n", "e");
    }
}}
