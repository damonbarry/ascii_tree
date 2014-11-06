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
                syntax_tree({}).analyze();
            });
        }

        TEST_METHOD(should_point_to_the_root_node_when_it_is_present)
        {
            syntax_tree tree(vector<token>{ root_node() });
            auto root = tree.analyze();
            _(root).should_not_be_null();
        }
    };
}}
