/**
 * @file DeBruijnValue.cpp
 * @author Anna Catenacci
 * 
 * Unit tests to make the DeBruijn Values (vertex / nodes) and DeBruijn Edges work corrently.
 * 
 * To run this file, type "make assert_value" into your command line terminal.
 * 
 */

#include "DeBruijnValue.hpp"
#include "DeBruijnEdge.hpp"
#define CATCH_CONFIG_MAIN
#include "../../../mabe/MABE2/source/third-party/empirical/third-party/Catch/single_include/catch2/catch.hpp"

TEST_CASE("DeBruijnValue__empty", "[DeBruijnValue.hpp]")
{
    {
        CHECK( 1 == 1 );
    }
}

TEST_CASE("DeBruijnValue__flags", "[DeBruijnValue.hpp]")
{
    {
        // initialize a single node
        DeBruijnValue a;
        CHECK(a.get_endpoint() == 0);
        CHECK(a.get_kmer_occurrences() == 0);
        CHECK(a.get_loop_flag() == 0);
        CHECK(a.get_visitor_flag() == 0);

        // increment values
        a.increment_endpoint();
        a.increment_kmer_occurrences();
        a.set_loop_flag(1);
        a.increment_visitor_flag();

        CHECK(a.get_endpoint() == 1);
        CHECK(a.get_kmer_occurrences() == 1);
        CHECK(a.get_loop_flag() == 1);
        CHECK(a.get_visitor_flag() == 1);

        // decrement values
        a.decrement_endpoint();
        a.decrement_kmer_occurrences();
        a.set_loop_flag(0);
        a.change_visitor_flag(0);

        CHECK(a.get_endpoint() == 0);
        CHECK(a.get_kmer_occurrences() == 0);
        CHECK(a.get_loop_flag() == 0);
        CHECK(a.get_visitor_flag() == 0);

    }
}

TEST_CASE("DeBruijnValue__adj-lists", "[DeBruijnValue.hpp]")
{
    {
        DeBruijnValue a;

        CHECK(a.adj_list_size() == 0);
        CHECK(a.adj_availible_size() == 0);

        // add adjacencies into a single node
        a.add_to_adj_list("000");
        a.add_to_adj_list("001");
        a.add_to_adj_list("010");
        a.add_to_adj_list("011");
        a.add_to_adj_list("100");
        a.add_to_adj_list("101");
        a.add_to_adj_list("110");
        a.add_to_adj_list("111");
        CHECK(a.adj_availible_size() == 0);
        CHECK(a.adj_list_size() == 8);

        // assign path lengths
        a.append_path_len(1, "000");
        a.append_path_len(2, "001");
        a.append_path_len(3, "010"); 
        a.append_path_len(4, "011");
        a.append_path_len(5, "100");
        a.append_path_len(6, "101");
        a.append_path_len(7, "110");
        a.append_path_len(std::numeric_limits<int>::max(), "111");
        CHECK(a.get_all_paths().size() == 8);

        // remove some adj.s from the list
        a.remove_from_adj_list("110");
        a.remove_from_adj_list("101");
        a.remove_path_len("110");
        a.remove_path_len("101");
        CHECK(a.adj_list_size() == 6);
        CHECK(a.get_all_paths().size() == 6);

        // make some adjacencies availible
        a.clear_adj_availible();
        a.append_adj_availible("000");
        CHECK(a.adj_availible_size() == 1);

        vector<string> vec({"000", "001"});
        a.append_adj_availible(vec);
        CHECK(a.adj_availible_size() == 2);

        a.clear_adj_availible();
        CHECK(a.adj_availible_size() == 0);

        // make all adjacencies availible
        a.make_all_adj_availible();
        CHECK(a.get_all_adj_availible() == a.get_adj_list());
        CHECK(a.adj_availible_size() == 6);

        // get desired output from availible adj.s list
        tuple<int, vector<string>> min_adj = a.get_min_length();
        CHECK(std::get<0>(min_adj) == 1);
        CHECK(std::get<1>(min_adj).size() == 1);
        CHECK(std::get<1>(min_adj)[0] == "000");

        tuple<int, vector<string>> max_adj = a.get_max_length();
        CHECK(std::get<0>(max_adj) == std::numeric_limits<int>::max());
        CHECK(std::get<1>(max_adj).size() == 1);
        CHECK(std::get<1>(max_adj)[0] == "111");
        CHECK(a.adj_availible_size() == 6);

        a.clear_adj_availible();
        a.not_too_short(6, 8);
        CHECK(a.adj_availible_size() == 5);
        std::set<string> s{"001", "010", "011", "100", "111"};
        CHECK(a.get_all_adj_availible() == s);

        a.clear_adj_availible();
        a.not_too_short(9, 8);
        CHECK(a.adj_availible_size() == 6);
        s = {"000", "001", "010", "011", "100", "111"};
        CHECK(a.get_all_adj_availible() == s);

        a.clear_adj_availible();
        a.not_too_short(6, 8, 1);
        CHECK(a.adj_availible_size() == 6);
        s = {"000", "001", "010", "011", "100", "111"};
        CHECK(a.get_all_adj_availible() == s);

        a.clear_adj_availible();
        a.non_inf_paths();
        CHECK(a.adj_availible_size() == 5);
        s = {"000", "001", "010", "011", "100"};
        CHECK(a.get_all_adj_availible() == s);
        CHECK(a.adj_list_size() == 6);
        CHECK(a.get_all_paths().size() == 6);

        a.remove_inf_path();
        max_adj = a.get_max_length();
        CHECK(std::get<0>(max_adj) == 5);
        CHECK(std::get<1>(max_adj).size() == 1);
        CHECK(std::get<1>(max_adj)[0] == "100");
        CHECK(a.get_all_paths().size() == 5);

        // remove adjacencies from the list
        a.remove_from_adj_list("111");
        CHECK(a.adj_list_size() == 5);
        s = {"000", "001", "010", "011", "100"};
        CHECK(a.get_adj_list() == s);
        a.remove_adj_availible("000", 0); // why does this have a bool???
        CHECK(a.adj_availible_size() == 4);
        s = {"001", "010", "011", "100"};
        CHECK(a.get_all_adj_availible() == s);
        
    }
}
// organize vertex file in the way this test is organized, not by getters and setters but by topic
// in the getters for adj avail based on path len, some return stuff and some just set adj_avail_list -- are these the way we want?
// write out all function names by group and think of better ones
// make sure we clear_adj_availible(); before we assign adj list in main
// i don't think you can do math with inf based on these test cases with not_too_short, you just have to compare it to inf

TEST_CASE("DeBruijnValue__edges", "[DeBruijnValue.hpp]")
{
    {
        // initialize an empty node
        DeBruijnValue a;
        DeBruijnEdge in0;
    }
}
