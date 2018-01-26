#include "grammar.hpp"
#include "test_helpers.hpp"

using namespace std;

namespace ascii_tree { namespace spec
{
    inline grid_ptr make_grid(const grid_type& grid)
    {
        return std::make_shared<grid_type>(grid);
    }

    TEST_CASE("should not recognize any tokens in an empty string", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("").tokens();
        _(tokens).should_be_empty();
    }

    TEST_CASE("should recognize a root node", "[can recognize ascii tree tokens]")
    {
        token tok = grammar("[*]").root_node();
        _(tok).should_be(root_node());
    }

    TEST_CASE("should recognize a named node", "[can recognize ascii tree tokens]")
    {
        token tok = grammar("[abc]").named_node();
        _(tok).should_be(named_node("abc"));
    }

    TEST_CASE("should reject an empty node when it expects a named node", "[can recognize ascii tree tokens]")
    {
        should_throw_(parse_exception("[]", 1), []{
            grammar("[]").named_node();
        });
    }

    TEST_CASE("should recognize an edge name", "[can recognize ascii tree tokens]")
    {
        token tok = grammar("(abc)").edge_name();
        _(tok).should_be(edge_name("abc"));
    }

    TEST_CASE("should reject an empty edge name", "[can recognize ascii tree tokens]")
    {
        should_throw_(parse_exception("()", 1), []{
            grammar("()").edge_name();
        });
    }

    TEST_CASE("should recognize an ascending edge part", "[can recognize ascii tree tokens]")
    {
        token tok = grammar("/").ascending_edge_part();
        _(tok).should_be(ascending_edge_part());
    }

    TEST_CASE("should recognize a descending edge part", "[can recognize ascii tree tokens]")
    {
        token tok = grammar("\\").descending_edge_part();
        _(tok).should_be(descending_edge_part());
    }

    TEST_CASE("should recognize a vertical edge part", "[can recognize ascii tree tokens]")
    {
        token tok = grammar("|").vertical_edge_part();
        _(tok).should_be(vertical_edge_part());
    }

    TEST_CASE("should recognize a horizontal edge", "[can recognize ascii tree tokens]")
    {
        token tok = grammar("---(xyz)--").horizontal_edge();
        _(tok).should_be(horizontal_edge("xyz"));
    }

    TEST_CASE("should reject a nameless horizontal edge", "[can recognize ascii tree tokens]")
    {
        should_throw_(parse_exception("--", 2), []{
            grammar("--").horizontal_edge();
        });
    }

    TEST_CASE("should recognize a root node next to a named node", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("[*][a]").tokens();
        _(tokens).should_equal({ root_node(), named_node("a") });
    }

    TEST_CASE("should recognize a root node next to a horizontal edge", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("[*]-(a)-").tokens();
        _(tokens).should_equal({ root_node(), horizontal_edge("a") });
    }

    TEST_CASE("should recognize a root node next to a descending edge part", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("[*]\\").tokens();
        _(tokens).should_equal({ root_node(), descending_edge_part() });
    }

    TEST_CASE("should recognize a root node next to an ascending edge part", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("[*]/").tokens();
        _(tokens).should_equal({ root_node(), ascending_edge_part() });
    }

    TEST_CASE("should recognize a root node next to a vertical edge part", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("[*]|").tokens();
        _(tokens).should_equal({ root_node(), vertical_edge_part() });
    }

    TEST_CASE("should recognize a root node next to an edge name", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("[*](a)").tokens();
        _(tokens).should_equal({ root_node(), edge_name("a") });
    }

    TEST_CASE("should recognize a named node next to a root node", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("[a][*]").tokens();
        _(tokens).should_equal({ named_node("a"), root_node() });
    }

    TEST_CASE("should recognize a named node next to a named node", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("[a][b]").tokens();
        _(tokens).should_equal({ named_node("a"), named_node("b") });
    }

    TEST_CASE("should recognize a named node next to a horizontal edge", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("[a]-(b)-").tokens();
        _(tokens).should_equal({ named_node("a"), horizontal_edge("b") });
    }

    TEST_CASE("should recognize a named node next to a descending edge part", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("[a]\\").tokens();
        _(tokens).should_equal({ named_node("a"), descending_edge_part() });
    }

    TEST_CASE("should recognize a named node next to an ascending edge part", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("[a]/").tokens();
        _(tokens).should_equal({ named_node("a"), ascending_edge_part() });
    }

    TEST_CASE("should recognize a named node next to a vertical edge part", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("[a]|").tokens();
        _(tokens).should_equal({ named_node("a"), vertical_edge_part() });
    }

    TEST_CASE("should recognize a named node next to an edge name", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("[a](b)").tokens();
        _(tokens).should_equal({ named_node("a"), edge_name("b") });
    }

    TEST_CASE("should recognize a horizontal edge next to a root node", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("-(a)-[*]").tokens();
        _(tokens).should_equal({ horizontal_edge("a"), root_node() });
    }

    TEST_CASE("should recognize a horizontal edge next to a named node", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("-(a)-[b]").tokens();
        _(tokens).should_equal({ horizontal_edge("a"), named_node("b") });
    }

    TEST_CASE("should recognize a descending edge part next to a root node", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("\\[*]").tokens();
        _(tokens).should_equal({ descending_edge_part(), root_node() });
    }

    TEST_CASE("should recognize a descending edge part next to a named node", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("\\[a]").tokens();
        _(tokens).should_equal({ descending_edge_part(), named_node("a") });
    }

    TEST_CASE("should recognize a descending edge part next to a descending edge part", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("\\\\").tokens();
        _(tokens).should_equal({ descending_edge_part(), descending_edge_part() });
    }

    TEST_CASE("should recognize a descending edge part next to an ascending edge part", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("\\/").tokens();
        _(tokens).should_equal({ descending_edge_part(), ascending_edge_part() });
    }

    TEST_CASE("should recognize a descending edge part next to a vertical edge part", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("\\|").tokens();
        _(tokens).should_equal({ descending_edge_part(), vertical_edge_part() });
    }

    TEST_CASE("should recognize a descending edge part next to an edge name", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("\\(a)").tokens();
        _(tokens).should_equal({ descending_edge_part(), edge_name("a") });
    }

    TEST_CASE("should recognize an ascending edge part next to a root node", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("/[*]").tokens();
        _(tokens).should_equal({ ascending_edge_part(), root_node() });
    }

    TEST_CASE("should recognize an ascending edge part next to a named node", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("/[a]").tokens();
        _(tokens).should_equal({ ascending_edge_part(), named_node("a") });
    }

    TEST_CASE("should recognize an ascending edge part next to a descending edge part", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("/\\").tokens();
        _(tokens).should_equal({ ascending_edge_part(), descending_edge_part() });
    }

    TEST_CASE("should recognize an ascending edge part next to an ascending edge part", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("//").tokens();
        _(tokens).should_equal({ ascending_edge_part(), ascending_edge_part() });
    }

    TEST_CASE("should recognize an ascending edge part next to a vertical edge part", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("/|").tokens();
        _(tokens).should_equal({ ascending_edge_part(), vertical_edge_part() });
    }

    TEST_CASE("should recognize an ascending edge part next to an edge name", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("/(a)").tokens();
        _(tokens).should_equal({ ascending_edge_part(), edge_name("a") });
    }

    TEST_CASE("should recognize a vertical edge part next to a root node", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("|[*]").tokens();
        _(tokens).should_equal({ vertical_edge_part(), root_node() });
    }

    TEST_CASE("should recognize a vertical edge part next to a named node", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("|[a]").tokens();
        _(tokens).should_equal({ vertical_edge_part(), named_node("a") });
    }

    TEST_CASE("should recognize a vertical edge part next to a descending edge part", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("|\\").tokens();
        _(tokens).should_equal({ vertical_edge_part(), descending_edge_part() });
    }

    TEST_CASE("should recognize a vertical edge part next to an ascending edge part", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("|/").tokens();
        _(tokens).should_equal({ vertical_edge_part(), ascending_edge_part() });
    }

    TEST_CASE("should recognize a vertical edge part next to a vertical edge part", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("||").tokens();
        _(tokens).should_equal({ vertical_edge_part(), vertical_edge_part() });
    }

    TEST_CASE("should recognize a vertical edge part next to an edge name", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("|(a)").tokens();
        _(tokens).should_equal({ vertical_edge_part(), edge_name("a") });
    }

    TEST_CASE("should recognize an edge name next to a root node", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("(a)[*]").tokens();
        _(tokens).should_equal({ edge_name("a"), root_node() });
    }

    TEST_CASE("should recognize an edge name next to a named node", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("(a)[b]").tokens();
        _(tokens).should_equal({ edge_name("a"), named_node("b") });
    }

    TEST_CASE("should recognize an edge name next to a descending edge part", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("(a)\\").tokens();
        _(tokens).should_equal({ edge_name("a"), descending_edge_part() });
    }

    TEST_CASE("should recognize an edge name next to an ascending edge part", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("(a)/").tokens();
        _(tokens).should_equal({ edge_name("a"), ascending_edge_part() });
    }

    TEST_CASE("should recognize an edge name next to a vertical edge part", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("(a)|").tokens();
        _(tokens).should_equal({ edge_name("a"), vertical_edge_part() });
    }

    TEST_CASE("should recognize an edge name next to an edge name", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar("(a)(b)").tokens();
        _(tokens).should_equal({ edge_name("a"), edge_name("b") });
    }

    TEST_CASE("should recognize tokens across multiple lines", "[can recognize ascii tree tokens]")
    {
        auto tokens = grammar({ "(a)", "(b)" }).tokens();
        _(tokens).should_equal({ edge_name("a"), edge_name("b") });
    }

    TEST_CASE("tokens should know their position", "[can recognize ascii tree tokens]")
    {
        std::vector<std::string> text {
            "[*][a](b)",    // root node, named node, edge name
            "-(c)-/\\|"     // hor. edge, asc. edge part, desc. edge part, vert. edge part
        };

        auto tokens = grammar(text).tokens();

        auto g = make_grid(text);

        _(positions_from(tokens)).should_equal({
            { g, 0, 0 }, // root node
            { g, 0, 3 }, // named node
            { g, 0, 6 }, // edge name
            { g, 1, 0 }, // horizontal edge
            { g, 1, 5 }, // ascending edge part
            { g, 1, 6 }, // descending edge part
            { g, 1, 7 }  // vertical edge part
        });
    }
}}
