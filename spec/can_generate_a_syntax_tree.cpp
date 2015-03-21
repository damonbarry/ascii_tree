#include "syntax_tree.hpp"
#include "test_helpers.hpp"
#include <vector>

using namespace std;

namespace ascii_tree { namespace spec
{
    TEST_CLASS(can_generate_a_syntax_tree)
    {
    public:

        TEST_METHOD(should_throw_when_root_node_is_absent)
        {
            should_throw_(analyze_exception("missing root node"), []{
                syntax_tree({ { named_node("a") } }).analyze();
            });
        }

        TEST_METHOD(should_point_to_the_root_node_when_it_is_present)
        {
            syntax_tree tree({ { named_node("b"), horizontal_edge("a"), root_node(), horizontal_edge("c"), named_node("d") } }); // TODO: remove c and d when analyze isn't hard-coded to require an edge and node after the root
            auto result = tree.analyze();
            _(result).should_be(root_node());
        }

        TEST_METHOD(should_throw_if_more_than_one_root_node_is_present)
        {
            should_throw_(analyze_exception("more than one root node"), []{
                syntax_tree({ { root_node(), root_node() } }).analyze();
            });
        }

        TEST_METHOD(should_generate_a_tree_with_root_horizontal_edge_leaf)
        {
            syntax_tree tree({ { root_node(), horizontal_edge("e"), named_node("n") } });
            auto result = tree.analyze();
            _(result).should_have_node_along_edge("n", "e");
        }

        TEST_METHOD(should_generate_a_tree_with_leaf_horizontal_edge_root)
        {
            syntax_tree tree({ { named_node("n"), horizontal_edge("e"), root_node() } });
            auto result = tree.analyze();
            _(result).should_have_node_along_edge("n", "e");
        }

        TEST_METHOD(should_generate_a_tree_with_root_vertical_edge_leaf)
        {
            syntax_tree tree({ { root_node(), vertical_edge_part(), edge_name("e"), vertical_edge_part(), named_node("n") } });
            auto result = tree.analyze();
            _(result).should_have_node_along_edge("n", "e");
        }

        TEST_METHOD(should_generate_a_tree_with_leaf_vertical_edge_root)
        {
            syntax_tree tree({ { named_node("n"), vertical_edge_part(), edge_name("e"), vertical_edge_part(), root_node() } });
            auto result = tree.analyze();
            _(result).should_have_node_along_edge("n", "e");
        }

        TEST_METHOD(should_generate_a_tree_with_root_ascending_edge_leaf)
        {
            syntax_tree tree({ { root_node(), ascending_edge_part(), edge_name("e"), ascending_edge_part(), named_node("n") } });
            auto result = tree.analyze();
            _(result).should_have_node_along_edge("n", "e");
        }

        TEST_METHOD(should_generate_a_tree_with_leaf_ascending_edge_root)
        {
            syntax_tree tree({ { named_node("n"), ascending_edge_part(), edge_name("e"), ascending_edge_part(), root_node() } });
            auto result = tree.analyze();
            _(result).should_have_node_along_edge("n", "e");
        }

        TEST_METHOD(should_generate_a_tree_with_root_descending_edge_leaf)
        {
            syntax_tree tree({ { root_node(), descending_edge_part(), edge_name("e"), descending_edge_part(), named_node("n") } });
            auto result = tree.analyze();
            _(result).should_have_node_along_edge("n", "e");
        }

        TEST_METHOD(should_generate_a_tree_with_leaf_descending_edge_root)
        {
            syntax_tree tree({ { named_node("n"), descending_edge_part(), edge_name("e"), descending_edge_part(), root_node() } });
            auto result = tree.analyze();
            _(result).should_have_node_along_edge("n", "e");
        }
    };
}}
