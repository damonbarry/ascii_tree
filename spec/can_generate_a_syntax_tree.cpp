#include "syntax_tree.hpp"
#include "test_helpers.hpp"

using namespace std;

namespace ascii_tree { namespace spec
{
    TEST_CLASS(can_generate_a_syntax_tree)
    {
    public:

        TEST_METHOD(should_throw_when_root_node_is_absent)
        {
            should_throw_(analyze_exception("missing root node"), []{
                syntax_tree().analyze("");
            });
        }

    };
}}
