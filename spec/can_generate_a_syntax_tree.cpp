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
            syntax_tree tree({ { named_node("b"), horizontal_edge("a"), root_node() } });
            auto result = tree.analyze();
            _(result).should_be(root_node());
        }

        TEST_METHOD(should_throw_if_more_than_one_root_node_is_present)
        {
            should_throw_(analyze_exception("more than one root node"), []{
                syntax_tree({ { root_node(), root_node() } }).analyze();
            });
        }
    };
}}
