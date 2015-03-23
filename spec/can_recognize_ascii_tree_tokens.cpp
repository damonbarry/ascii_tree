#include "grammar.hpp"
#include "test_helpers.hpp"

using namespace std;

namespace ascii_tree { namespace spec
{
    TEST_CLASS(can_recognize_ascii_tree_tokens)
    {
    public:
        
        TEST_METHOD(should_not_recognize_any_tokens_in_an_empty_string)
        {
            auto tokens = grammar("").tokens();
            _(tokens).should_be_empty();
        }

        TEST_METHOD(should_recognize_a_root_node)
        {
            token tok = grammar("[*]").root_node();
            _(tok).should_be(root_node());
        }

        TEST_METHOD(should_recognize_a_named_node)
        {
            token tok = grammar("[abc]").named_node();
            _(tok).should_be(named_node("abc"));
        }

        TEST_METHOD(should_reject_an_empty_node_when_it_expects_a_named_node)
        {
            should_throw_(parse_exception("[]", 1), []{
                grammar("[]").named_node();
            });
        }

        TEST_METHOD(should_recognize_an_edge_name)
        {
            token tok = grammar("(abc)").edge_name();
            _(tok).should_be(edge_name("abc"));
        }

        TEST_METHOD(should_reject_an_empty_edge_name)
        {
            should_throw_(parse_exception("()", 1), []{
                grammar("()").edge_name();
            });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part)
        {
            token tok = grammar("/").ascending_edge_part();
            _(tok).should_be(ascending_edge_part());
        }

        TEST_METHOD(should_recognize_a_descending_edge_part)
        {
            token tok = grammar("\\").descending_edge_part();
            _(tok).should_be(descending_edge_part());
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part)
        {
            token tok = grammar("|").vertical_edge_part();
            _(tok).should_be(vertical_edge_part());
        }

        TEST_METHOD(should_recognize_a_horizontal_edge)
        {
            token tok = grammar("---(xyz)--").horizontal_edge();
            _(tok).should_be(horizontal_edge("xyz"));
        }

        TEST_METHOD(should_reject_a_nameless_horizontal_edge)
        {
            should_throw_(parse_exception("--", 2), []{
                grammar("--").horizontal_edge();
            });
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_a_named_node)
        {
            auto tokens = grammar("[*][a]").tokens();
            _(tokens).should_equal({ root_node(), named_node("a") });
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_a_horizontal_edge)
        {
            auto tokens = grammar("[*]-(a)-").tokens();
            _(tokens).should_equal({ root_node(), horizontal_edge("a") });
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_a_descending_edge_part)
        {
            auto tokens = grammar("[*]\\").tokens();
            _(tokens).should_equal({ root_node(), descending_edge_part() });
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_an_ascending_edge_part)
        {
            auto tokens = grammar("[*]/").tokens();
            _(tokens).should_equal({ root_node(), ascending_edge_part() });
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_a_vertical_edge_part)
        {
            auto tokens = grammar("[*]|").tokens();
            _(tokens).should_equal({ root_node(), vertical_edge_part() });
        }

        TEST_METHOD(should_recognize_a_root_node_next_to_an_edge_name)
        {
            auto tokens = grammar("[*](a)").tokens();
            _(tokens).should_equal({ root_node(), edge_name("a") });
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_a_root_node)
        {
            auto tokens = grammar("[a][*]").tokens();
            _(tokens).should_equal({ named_node("a"), root_node() });
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_a_named_node)
        {
            auto tokens = grammar("[a][b]").tokens();
            _(tokens).should_equal({ named_node("a"), named_node("b") });
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_a_horizontal_edge)
        {
            auto tokens = grammar("[a]-(b)-").tokens();
            _(tokens).should_equal({ named_node("a"), horizontal_edge("b") });
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_a_descending_edge_part)
        {
            auto tokens = grammar("[a]\\").tokens();
            _(tokens).should_equal({ named_node("a"), descending_edge_part() });
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_an_ascending_edge_part)
        {
            auto tokens = grammar("[a]/").tokens();
            _(tokens).should_equal({ named_node("a"), ascending_edge_part() });
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_a_vertical_edge_part)
        {
            auto tokens = grammar("[a]|").tokens();
            _(tokens).should_equal({ named_node("a"), vertical_edge_part() });
        }

        TEST_METHOD(should_recognize_a_named_node_next_to_an_edge_name)
        {
            auto tokens = grammar("[a](b)").tokens();
            _(tokens).should_equal({ named_node("a"), edge_name("b") });
        }

        TEST_METHOD(should_recognize_a_horizontal_edge_next_to_a_root_node)
        {
            auto tokens = grammar("-(a)-[*]").tokens();
            _(tokens).should_equal({ horizontal_edge("a"), root_node() });
        }

        TEST_METHOD(should_recognize_a_horizontal_edge_next_to_a_named_node)
        {
            auto tokens = grammar("-(a)-[b]").tokens();
            _(tokens).should_equal({ horizontal_edge("a"), named_node("b") });
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_a_root_node)
        {
            auto tokens = grammar("\\[*]").tokens();
            _(tokens).should_equal({ descending_edge_part(), root_node() });
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_a_named_node)
        {
            auto tokens = grammar("\\[a]").tokens();
            _(tokens).should_equal({ descending_edge_part(), named_node("a") });
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_a_descending_edge_part)
        {
            auto tokens = grammar("\\\\").tokens();
            _(tokens).should_equal({ descending_edge_part(), descending_edge_part() });
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_an_ascending_edge_part)
        {
            auto tokens = grammar("\\/").tokens();
            _(tokens).should_equal({ descending_edge_part(), ascending_edge_part() });
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_a_vertical_edge_part)
        {
            auto tokens = grammar("\\|").tokens();
            _(tokens).should_equal({ descending_edge_part(), vertical_edge_part() });
        }

        TEST_METHOD(should_recognize_a_descending_edge_part_next_to_an_edge_name)
        {
            auto tokens = grammar("\\(a)").tokens();
            _(tokens).should_equal({ descending_edge_part(), edge_name("a") });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_a_root_node)
        {
            auto tokens = grammar("/[*]").tokens();
            _(tokens).should_equal({ ascending_edge_part(), root_node() });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_a_named_node)
        {
            auto tokens = grammar("/[a]").tokens();
            _(tokens).should_equal({ ascending_edge_part(), named_node("a") });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_a_descending_edge_part)
        {
            auto tokens = grammar("/\\").tokens();
            _(tokens).should_equal({ ascending_edge_part(), descending_edge_part() });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_an_ascending_edge_part)
        {
            auto tokens = grammar("//").tokens();
            _(tokens).should_equal({ ascending_edge_part(), ascending_edge_part() });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_a_vertical_edge_part)
        {
            auto tokens = grammar("/|").tokens();
            _(tokens).should_equal({ ascending_edge_part(), vertical_edge_part() });
        }

        TEST_METHOD(should_recognize_an_ascending_edge_part_next_to_an_edge_name)
        {
            auto tokens = grammar("/(a)").tokens();
            _(tokens).should_equal({ ascending_edge_part(), edge_name("a") });
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_a_root_node)
        {
            auto tokens = grammar("|[*]").tokens();
            _(tokens).should_equal({ vertical_edge_part(), root_node() });
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_a_named_node)
        {
            auto tokens = grammar("|[a]").tokens();
            _(tokens).should_equal({ vertical_edge_part(), named_node("a") });
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_a_descending_edge_part)
        {
            auto tokens = grammar("|\\").tokens();
            _(tokens).should_equal({ vertical_edge_part(), descending_edge_part() });
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_an_ascending_edge_part)
        {
            auto tokens = grammar("|/").tokens();
            _(tokens).should_equal({ vertical_edge_part(), ascending_edge_part() });
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_a_vertical_edge_part)
        {
            auto tokens = grammar("||").tokens();
            _(tokens).should_equal({ vertical_edge_part(), vertical_edge_part() });
        }

        TEST_METHOD(should_recognize_a_vertical_edge_part_next_to_an_edge_name)
        {
            auto tokens = grammar("|(a)").tokens();
            _(tokens).should_equal({ vertical_edge_part(), edge_name("a") });
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_a_root_node)
        {
            auto tokens = grammar("(a)[*]").tokens();
            _(tokens).should_equal({ edge_name("a"), root_node() });
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_a_named_node)
        {
            auto tokens = grammar("(a)[b]").tokens();
            _(tokens).should_equal({ edge_name("a"), named_node("b") });
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_a_descending_edge_part)
        {
            auto tokens = grammar("(a)\\").tokens();
            _(tokens).should_equal({ edge_name("a"), descending_edge_part() });
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_an_ascending_edge_part)
        {
            auto tokens = grammar("(a)/").tokens();
            _(tokens).should_equal({ edge_name("a"), ascending_edge_part() });
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_a_vertical_edge_part)
        {
            auto tokens = grammar("(a)|").tokens();
            _(tokens).should_equal({ edge_name("a"), vertical_edge_part() });
        }

        TEST_METHOD(should_recognize_an_edge_name_next_to_an_edge_name)
        {
            auto tokens = grammar("(a)(b)").tokens();
            _(tokens).should_equal({ edge_name("a"), edge_name("b") });
        }

        TEST_METHOD(should_recognize_tokens_across_multiple_lines)
        {
            auto tokens = grammar({ "(a)", "(b)" }).tokens();
            _(tokens).should_equal({ edge_name("a"), edge_name("b") });
        }
    };
}}
