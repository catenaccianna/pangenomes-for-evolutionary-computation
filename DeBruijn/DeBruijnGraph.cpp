/**
 * @file DeBruijnGraph.cpp
 * @author Anna Catenacci
 * 
 * Unit tests to make the DeBruijn Graph and MABE opertations work corrently.
 * 
 * To run this file, type "make assert" into your command line terminal.
 * 
 */

#include "DeBruijnGraph.hpp"
#include "DeBruijnValue.hpp"
#define CATCH_CONFIG_MAIN
#include "../../../mabe/MABE2/source/third-party/empirical/third-party/Catch/single_include/catch2/catch.hpp"


TEST_CASE("DeBruijnGraph__helper-functions", "[DeBruijnGraph.hpp]")
{
    {
        // add_sequence helper functions (add_edge, set_path_length, set_empty_vertex--but this one is private)
        DeBruijnGraph g;
        g.add_sequence("abcd");
        DeBruijnValue a = g.get_value("abc");
        DeBruijnValue b = g.get_value("bcd");
        // path length on both
        tuple<int, set<string>> lengths = {1, set<string>{"bcd"}};
        set<string> paths;
        CHECK(a.get_max_length() == lengths);
        CHECK(a.get_min_length() == lengths);
        CHECK(b.get_all_paths() == paths);
        
        // empty vertex on second

        // edges -- this is also in DeBruijnValue.cpp
        set<string> head, tail;
        CHECK(a.get_in_edge().get_head() == head);
        CHECK(b.get_out_edge().get_tail() == tail);
        head = {"abc"};
        tail = {"bcd"};
        CHECK(a.get_out_edge().get_tail() == tail);
        CHECK(b.get_in_edge().get_head() == head);

        head = {"abc", "aaa", "bbb"};
        tail = {"bcd", "ddd", "ccc"};
        a.get_out_edge().set_tail("ddd");
        a.get_out_edge().set_tail("ccc");
        a.get_out_edge().set_tail("xyz");
        b.get_in_edge().set_head("bbb");
        b.get_in_edge().set_head("aaa");
        b.get_in_edge().set_head("tgf");
        a.get_out_edge().remove_tail("xyz");
        b.get_in_edge().remove_head("tgf");
        CHECK(a.get_out_edge().get_tail() == tail);
        CHECK(b.get_in_edge().get_head() == head);

        // path length helper functions

        // remove_sequence helper functions (remove_from_adj_list, erase, remove_path_len, mStarts.erase)
        set<string> full_adj_list = {"bcd"};
        CHECK(a.get_adj_list() == full_adj_list);
        full_adj_list.clear();
        a.remove_from_adj_list("bcd");
        CHECK(a.get_adj_list() == full_adj_list);

        map<int, set<string>> pathlen = {{1, set<string>{"bcd"}}};
        CHECK(a.get_all_paths() == set<string>{"bcd"});
        // check path availibilities
        a.remove_path_len("bcd");
        CHECK(a.get_all_paths() == set<string>{});

        map<string, DeBruijnValue> g_object= g.get_graph();
        g_object.erase("abc");
        CHECK(g_object.size() == 1);
        CHECK(g_object.begin()->first == "bcd");
        
        set<string> starts = g.get_starts();
        CHECK(starts == set<string>{"abc"});
        auto iter = starts.find("abc");
        if(iter != starts.end()) { starts.erase(iter); }
        CHECK(starts == set<string>{});

        // path length dictionary in DeBruijnBalue class
        DeBruijnValue c;
        c.append_path_len(2, "aaa");
        CHECK(c.get_all_path_lens() == set<int> {2});
        CHECK(c.get_all_paths() == set<string> {"aaa"});
        c.append_path_len(std::numeric_limits<int>::max(), "bbb");
        CHECK(c.get_all_path_lens() == set<int> {2, std::numeric_limits<int>::max()});
        CHECK(c.get_all_paths() == set<string> {"aaa", "bbb"});
        c.append_path_len(std::numeric_limits<int>::max(), "ccc");
        CHECK(c.get_all_path_lens() == set<int> {2, std::numeric_limits<int>::max()});
        CHECK(c.get_all_paths() == set<string> {"aaa", "bbb", "ccc"});
        c.remove_path_len("bbb");
        CHECK(c.get_all_path_lens() == set<int> {2, std::numeric_limits<int>::max()});
        CHECK(c.get_all_paths() == set<string> {"aaa", "ccc"});

        // modify_sequence helper functions

        // infinity length function by itself (not working right, a loop flag can get set but no inf path length set)

        // loop_detection traversal function by itself ()

        // remove_inf_path (not clearing the loop flag)
        DeBruijnGraph g2;
        g2.reset_loops();

    }
}


TEST_CASE("DeBruijnGraph__BitsOrgs", "[DeBruijnGraph.hpp]")
{
    {
        DeBruijnGraph g;

        // create a small graph
        g.add_sequence("11101234567890");
        CHECK(g.get_size() == 12);

        // remove sequence
        g.remove_sequence("11101234567890");
        CHECK(g.get_size() == 0);
        CHECK(g.is_valid("11101234567890") == false);
        CHECK(g.get_size() == 0);

        g.add_sequence("111031117"); // 111-110 -103-031-311-111-117
        CHECK(g.get_value("111").get_loop_flag() == 1);
        CHECK(g.get_value("110").get_loop_flag() == 0);
        CHECK(g.get_value("103").get_loop_flag() == 0);
        CHECK(g.get_value("031").get_loop_flag() == 0);
        CHECK(g.get_value("117").get_loop_flag() == 0);
        CHECK(get<0>(g.get_value("111").get_max_length()) == std::numeric_limits<int>::max()); // inf if you take 110, finite if you take 117
        CHECK(g.get_value("111").get_all_path_lens().size() == 3); // 1 if taking 117, 6 if taking 110 non inf path, inf if in loop
        CHECK(get<0>(g.get_value("110").get_max_length()) == std::numeric_limits<int>::max()); // inf if you take adj 103, finite if you take 103
        CHECK(get<0>(g.get_value("103").get_max_length()) == std::numeric_limits<int>::max()); // inf if you take 031, finite if you take 031
        CHECK(get<0>(g.get_value("031").get_max_length()) == std::numeric_limits<int>::max()); // inf if you take 311, finite if you take 311
        CHECK(get<0>(g.get_value("117").get_max_length()) != std::numeric_limits<int>::max()); // this is the endpoint, no more paths

        g.add_sequence("1110317");
        g.remove_sequence("111031117");
        CHECK(g.is_valid("111031117") == false);
        
        CHECK(g.get_value("111").get_loop_flag() == 0);
        CHECK(g.get_value("110").get_loop_flag() == 0);
        CHECK(g.get_value("103").get_loop_flag() == 0);
        CHECK(g.get_value("031").get_loop_flag() == 0);
        CHECK(g.get_value("317").get_loop_flag() == 0);
        CHECK(get<0>(g.get_value("111").get_max_length()) != std::numeric_limits<int>::max());
        CHECK(get<0>(g.get_value("110").get_max_length()) != std::numeric_limits<int>::max());
        CHECK(get<0>(g.get_value("103").get_max_length()) != std::numeric_limits<int>::max());
        CHECK(get<0>(g.get_value("031").get_max_length()) != std::numeric_limits<int>::max());
        CHECK(get<0>(g.get_value("317").get_max_length()) != std::numeric_limits<int>::max());
        g.remove_sequence("1110317");

        // create a regular BitsOrg graph 
        g.add_sequence("1111000110111101110101100101000010101110000001011000011101110101000001110000100101110100111010100110");
        // remove sequence
        g.remove_sequence("1111000110111101110101100101000010101110000001011000011101110101000001110000100101110100111010100110");
        CHECK(!g.is_valid("1111000110111101110101100101000010101110000001011000011101110101000001110000100101110100111010100110"));
        CHECK(g.get_size() == 0);
        // create a BitsOrg graph
        g.add_sequence("1111000110111101110101100101000010101110000001011000011101110101000001110000100101110100111010100110");
        g.add_sequence("1111111110111100011110100010101010101110010011100111010000101110001101010010110010010111110111100011");
        g.add_sequence("1101100110101001001111110000100010110001101010100100101100010000111001101001011000011010111001000001");
        g.add_sequence("1001010100101010100101111100111001101100000101110110001010010011110110110010111111111111101101101111");
        g.add_sequence("1110001001111100101011001011011100101000101000000000011110100101001100010101000000001010111011101010");
        g.add_sequence("0101000101001000001011010110101010011101000100101100000111000100000010010010011111101011101001010011");
        g.add_sequence("0100011001111111010100010011000100001101101001100011011001101101111001000110001110110000110101111011");
        g.add_sequence("1100000000000101101010001010000101001000011011001111110100110101101011101010101101100101010110100111");
        g.add_sequence("1101011111110101101010010011110101001011010101100011000000001011111110110101100110110111010101010111");
        g.add_sequence("1010110110001010000011000111000101101101100101010001101111000111001000011010101111010110110000001010");
        g.add_sequence("1100010101110001101101101000010000001000101100011011001110101001111001101011101101011000111110110011");
        CHECK(g.get_size() == 8);
        CHECK(g.is_valid("1111000110111101110101100101000010101110000001011000011101110101000001110000100101110100111010100110"));
        CHECK(g.is_valid("1111111110111100011110100010101010101110010011100111010000101110001101010010110010010111110111100011"));
        CHECK(g.is_valid("1101100110101001001111110000100010110001101010100100101100010000111001101001011000011010111001000001"));
        CHECK(g.is_valid("1001010100101010100101111100111001101100000101110110001010010011110110110010111111111111101101101111"));
        CHECK(g.is_valid("1110001001111100101011001011011100101000101000000000011110100101001100010101000000001010111011101010"));
        CHECK(g.is_valid("0101000101001000001011010110101010011101000100101100000111000100000010010010011111101011101001010011"));
        CHECK(g.is_valid("1100010101110001101101101000010000001000101100011011001110101001111001101011101101011000111110110011"));

        // genome crossover
        emp::Random random;
        string new_genome = g.modify_org(random, "1100010101110001101101101000010000001000101100011011001110101001111001101011101101011000111110110011");
        CHECK(g.is_valid(new_genome));
        new_genome = g.modify_org(random, "1010110110001010000011000111000101101101100101010001101111000111001000011010101111010110110000001010");
        CHECK(g.is_valid(new_genome));
        new_genome = g.modify_org(random, "1101011111110101101010010011110101001011010101100011000000001011111110110101100110110111010101010111");
        CHECK(g.is_valid(new_genome));
        new_genome = g.modify_org(random, "1100000000000101101010001010000101001000011011001111110100110101101011101010101101100101010110100111");
        CHECK(g.is_valid(new_genome));
    }
}

TEST_CASE("DeBruijnGraph__VirtualCPUOrgs", "[DeBruijnGraph.hpp]")
{
    {
        // create graph for Virtual CPU Orgs
        DeBruijnGraph g;
        emp::Random random;
        // add the same beginning sequences that MABE would add
        g.add_sequence("cccccccccccccccccccccccccccccccccccccccccccccccccc");
        g.add_sequence("cccccccccccccccccccccccccccccccccccccccccccccccccc");
        g.add_sequence("cccccccccccccccccccccccccccccccccccccccccccccccccc");
        g.add_sequence("cccccccccccccccccccccccccccccccccccccccccccccccccc");
        g.add_sequence("cccccccccccccccccccccccccccccccccccccccccccccccccc");
        g.add_sequence("cccccccccccccccccccccccccccccccccccccccccccccccccc");
        g.add_sequence("cccccccccccccccccccccccccccccccccccccccccccccccccc");
        CHECK(g.get_size() == 1);
        CHECK(g.get_value("ccc").adj_list_size() == 1);
        CHECK(g.get_value("ccc").get_endpoint() == 7);
        CHECK(g.get_value("ccc").get_loop_flag() == 1);
        CHECK(g.is_valid("cccccccccccccccccccccccccccccccccccccccccccccccccc"));

        // first recombination -- nothing about the graph should change
        g.modify_org_variable_len(random,"cccccccccccccccccccccccccccccccccccccccccccccccccc");
        g.modify_org_variable_len(random,"cccccccccccccccccccccccccccccccccccccccccccccccccc");
        g.modify_org_variable_len(random,"cccccccccccccccccccccccccccccccccccccccccccccccccc");
        g.modify_org_variable_len(random,"cccccccccccccccccccccccccccccccccccccccccccccccccc");
        g.modify_org_variable_len(random,"cccccccccccccccccccccccccccccccccccccccccccccccccc");
        g.modify_org_variable_len(random,"cccccccccccccccccccccccccccccccccccccccccccccccccc");
        CHECK(g.get_size() == 1);
        CHECK(g.get_value("ccc").adj_list_size() == 1);
        CHECK(g.get_value("ccc").get_endpoint() == 7);
        CHECK(g.get_value("ccc").get_loop_flag() == 1);
        CHECK(g.is_valid("cccccccccccccccccccccccccccccccccccccccccccccccccc"));

        // remove a few sequences
        g.remove_sequence("cccccccccccccccccccccccccccccccccccccccccccccccccc");
        g.remove_sequence("cccccccccccccccccccccccccccccccccccccccccccccccccc");
        g.remove_sequence("cccccccccccccccccccccccccccccccccccccccccccccccccc");
        g.remove_sequence("cccccccccccccccccccccccccccccccccccccccccccccccccc");
        CHECK(g.get_size() == 1);
        CHECK(g.get_value("ccc").adj_list_size() == 1);
        CHECK(g.get_value("ccc").get_endpoint() == 3);
        CHECK(g.get_value("ccc").get_loop_flag() == 1);
        CHECK(g.is_valid("cccccccccccccccccccccccccccccccccccccccccccccccccc"));

        g.add_sequence("cccdaccnnccclccnncccocccbtcccqfccgookccccbhccptccqfccclccnccc");
        g.add_sequence("nnccctjctbccocccdccc");
        g.add_sequence("cccnncccsccnncccoccclccncccccfccgookcccfcccbhccptccbhccptccr");
        g.add_sequence("cccbtcctbccocccsccnnccccttccqfccgoocccocccoocccdaccnccclccnccc");
        g.add_sequence("cccslccncccocccncccnncccocccbfcccqfccptcctjctbccoocccfccc");
        g.add_sequence("cccslccncccscccdcccttccbtccr");
        g.add_sequence("cccqfccgooccclccncccooccctjctbccoocccslccncccdccclccnccc");
        g.add_sequence("cccslccncccslccncccncccbfccptccbtccr");
        g.add_sequence("cmccbfccgoocccbtcccqfccclccncccocccttcccscccslccncccsccc");
        g.add_sequence("cccoccctbccocccttccbhccptcctbccoocccttccr");

        g.add_sequence("cccfccptcctqccncccdcccbtcccclccnccccscccttcctqccdcccbtccr");
        g.add_sequence("ccctqccdcccqfcccfccgoocccqfcccscccscccfccgooccclccnccc");
        g.add_sequence("cccbhccptcccqfccptccbhccptccttcccdcccctgcctgcctcctccr");
        g.add_sequence("cccsccclccncccocccocccdcccslccnccccdcccncccocccqfccgooccc");
        g.add_sequence("cccttcccfqccdcccdccclccnccccqfcccscccoocccslccncccnccc");
        g.add_sequence("cccqfccgoocccnccclccncccdcccctgcktqccncccdccctgcctcctccr");
        g.add_sequence("cccdccccfqccdcccbhccptcccbfccptcctbccooccclccncccqfccc");
        g.add_sequence("ccctqccdccclccncccdcccncccdcccqfccptccbfcccnccctgcctqccnccc");
        g.add_sequence("cccncccfqccncccoocccfqccncccfccptccclccnccctjctbccoccc");
        g.add_sequence("cccocccncccdccctjctbccoocccscccqfccgoocccqfccptccr");

        g.remove_sequence("cccfccptcctqccncccdcccbtcccclccnccccscccttcctqccdcccbtccr");
        g.remove_sequence("ccctqccdcccqfcccfccgoocccqfcccscccscccfccgooccclccnccc");
        g.remove_sequence("cccbhccptcccqfccptccbhccptccttcccdcccctgcctgcctcctccr");
        g.remove_sequence("cccsccclccncccocccocccdcccslccnccccdcccncccocccqfccgooccc");
        g.remove_sequence("cccttcccfqccdcccdccclccnccccqfcccscccoocccslccncccnccc");
        g.remove_sequence("cccqfccgoocccnccclccncccdcccctgcktqccncccdccctgcctcctccr");
        g.remove_sequence("cccdccccfqccdcccbhccptcccbfccptcctbccooccclccncccqfccc");
        g.remove_sequence("ccctqccdccclccncccdcccncccdcccqfccptccbfcccnccctgcctqccnccc");
        g.remove_sequence("cccncccfqccncccoocccfqccncccfccptccclccnccctjctbccoccc");
        g.remove_sequence("cccocccncccdccctjctbccoocccscccqfccgoocccqfccptccr");

        g.modify_org_variable_len(random,"cccfccptcctqccncccdcccbtcccclccnccccscccttcctqccdcccbtccr");
        g.modify_org_variable_len(random,"cccbhccptcccqfccptccbhccptccttcccdcccctgcctgcctcctccr");
        g.modify_org_variable_len(random,"cccoccctbccocccttccbhccptcctbccoocccttccr");
        g.modify_org_variable_len(random,"cccslccncccslccncccncccbfccptccbtccr");
        g.modify_org_variable_len(random,"cccbtcctbccocccsccnnccccttccqfccgoocccocccoocccdaccnccclccnccc");
        g.modify_org_variable_len(random,"nnccctjctbccocccdccc");
        g.modify_org_variable_len(random,"cccncccfqccncccoocccfqccncccfccptccclccnccctjctbccoccc");
        g.modify_org_variable_len(random,"cccocccncccdccctjctbccoocccscccqfccgoocccqfccptccr");
        g.modify_org_variable_len(random,"cccslccncccscccdcccttccbtccr");
        g.modify_org_variable_len(random,"cccdaccnnccclccnncccocccbtcccqfccgookccccbhccptccqfccclccnccc");

    }
}


/**
 * @brief Clone of DFT in the DeBruijnGraph class, but we don't reset the vertex flags at the end of this one so that we can test them.
 * @note If we change DTF function in the real class at any point, we should also change this one
 * 
 * @param mSize number of vertices in the graph (passed in from instance of a real graph)
 * @param mStarts set of staring vertices (passed in from instance of a real graph)
 * @param mVertices map of the graph (passed in from instance of a real graph)
 */
void traversal_clone(int mSize, set<string> mStarts, map<string, DeBruijnValue> mVertices){
        // edge case--this traversal did not work for size of 1 without it
        if(mSize == 1){
            mVertices[*mStarts.begin()].increment_visitor_flag();
        }
        else{
            // because this is a directed graph, I want to make sure each path start is covered
            set<string> path = mStarts;
            string current = "";
            while(path.size() > 0){
                current = *path.begin();
                path.erase(*path.begin());
                // if the vertex has been visited fewer times than it appears in the graph, continue:
                if(mVertices[current].get_visitor_flag() < 1){
                    // if this is the first time the vertex is being visited, we need to add it's adj_list into the queue
                    // otherwise, the adjacencies are already in there somewhere, so not needed
                    for (auto it = mVertices[current].get_adj_list().begin(); it != mVertices[current].get_adj_list().end();) {
                        path.insert(*it);
                    }
                    mVertices[current].increment_visitor_flag();

                }   
            } 
        }
    }


TEST_CASE("DeBruijnGraph__DepthFirstTraversal", "[DeBruijnGraph.hpp]")
{
    {
        // create a small graph
        /**DeBruijnGraph g;
        emp::Random random;
        g.add_sequence("ccc");
        CHECK(g.get_size() == 1);
        CHECK(g.get_sequence_size() == 1);
        CHECK(g.get_value("ccc").adj_list_size() == 0);
        CHECK(g.get_value("ccc").get_endpoint() == 1);
        CHECK(g.get_value("ccc").get_loop_flag() == 0);
        CHECK(g.get_value("ccc").get_kmer_occurrences() == 1);
        CHECK(g.get_value("ccc").get_visitor_flag() == 0);
        CHECK(g.is_valid("ccc"));

        // use the clone of DFT because the one in the class resets all vertex flags afterwards.
        traversal_clone(g.get_size(), g.get_starts(), g.get_graph());
        CHECK(g.get_size() == 1);
        CHECK(g.get_sequence_size() == 1);
        CHECK(g.get_value("ccc").adj_list_size() == 0);
        CHECK(g.get_value("ccc").get_endpoint() == 1);
        CHECK(g.get_value("ccc").get_loop_flag() == 0);
        CHECK(g.get_value("ccc").get_kmer_occurrences() == 1);
        CHECK(g.get_value("ccc").get_visitor_flag() == 1);
        CHECK(g.is_valid("ccc"));
        g.reset_vertex_flags();

        // create a larger graph
        g.add_sequence("cccdaccnnccclccnncccocccbtcccqfccgookccccbhccptccqfccclccnccc");
        g.add_sequence("nnccctjctbccocccdccc");
        g.add_sequence("cccnncccsccnncccoccclccncccccfccgookcccfcccbhccptccbhccptccr");
        g.add_sequence("cccbtcctbccocccsccnnccccttccqfccgoocccocccoocccdaccnccclccnccc");
        g.add_sequence("cccslccncccocccncccnncccocccbfcccqfccptcctjctbccoocccfccc");
        g.add_sequence("cccslccncccscccdcccttccbtccr");
        g.add_sequence("cccqfccgooccclccncccooccctjctbccoocccslccncccdccclccnccc");
        g.add_sequence("cccslccncccslccncccncccbfccptccbtccr");
        g.add_sequence("cmccbfccgoocccbtcccqfccclccncccocccttcccscccslccncccsccc");
        g.add_sequence("cccoccctbccocccttccbhccptcctbccoocccttccr");

        g.add_sequence("cccfccptcctqccncccdcccbtcccclccnccccscccttcctqccdcccbtccr");
        g.add_sequence("ccctqccdcccqfcccfccgoocccqfcccscccscccfccgooccclccnccc");
        g.add_sequence("cccbhccptcccqfccptccbhccptccttcccdcccctgcctgcctcctccr");
        g.add_sequence("cccsccclccncccocccocccdcccslccnccccdcccncccocccqfccgooccc");
        g.add_sequence("cccttcccfqccdcccdccclccnccccqfcccscccoocccslccncccnccc");
        g.add_sequence("cccqfccgoocccnccclccncccdcccctgcktqccncccdccctgcctcctccr");
        g.add_sequence("cccdccccfqccdcccbhccptcccbfccptcctbccooccclccncccqfccc");
        g.add_sequence("ccctqccdccclccncccdcccncccdcccqfccptccbfcccnccctgcctqccnccc");
        g.add_sequence("cccncccfqccncccoocccfqccncccfccptccclccnccctjctbccoccc");
        g.add_sequence("cccocccncccdccctjctbccoocccscccqfccgoocccqfccptccr");

        // test the same thing
        traversal_clone(g.get_size(), g.get_starts(), g.get_graph());
        for ( string i : g.get_all_vertices() ) {
            CHECK(g.get_value(i).get_visitor_flag() == 1);
        }
        g.reset_vertex_flags();*/
        
    }
}


TEST_CASE("DeBruijnGraph__value-attributes", "[DeBruijnGraph.hpp]")
{
    {
        /**
        DeBruijnGraph g;
        g.add_sequence("ABCDBCDE");
        g.update_loops();
        DBGraphValue bcd = g.get_value("BCD");
        CHECK(bcd.get_loop_flag() == 1);

        DBGraphValue cde = g.get_value("CDE");
        CHECK(cde.get_endpoint() == 1);

        g.update_weights();

        


        DeBruijnGraph virtualCPU;
        virtualCPU.add_sequence("cccicccdtceskcckcckeccfccdqcclcccbeccfccceskccepcc");
        virtualCPU.add_sequence("ccckcccrcqcrdecrecrcecrcecrecrcqcrdecrecrecrcecrce");
        virtualCPU.add_sequence("cccepcccesccrcqcrcqcrccctcesccoesccoesccrcccbeccfc");
        virtualCPU.add_sequence("cccepccctcccqsccrcccdtceskccpskcckdcccfccdtceskccc");
        virtualCPU.add_sequence("cccicccqsccckdccfccdtcesccrcecrecrcqcrdecrcqcrdecr");
        virtualCPU.add_sequence("cccepccccdqcclccckdccrcccbeccfcccqsccoescccfccdtce");
        virtualCPU.add_sequence("ccclcccescccrcccbeccfcccicccepcccqsccoescccdcnccce");
        virtualCPU.add_sequence("cccdqcclcckccckccpskccclcccbeccfccdccccrcecrcccdtc");
        virtualCPU.add_sequence("ccctcccbeccfcccrcccbeccfccdcncccfccdcncccfcccbeccf");
        virtualCPU.add_sequence("cccesccoesccoesccrcqcrcecrecrecrecrcecrcecrecrcccq");
        virtualCPU.add_sequence("cccbeccfccdqcclccclcccfcccdqcclccckeccfcccrcqcrdec");
        virtualCPU.add_sequence("cccclcccepcccbeccfccckcccbeccfccctcccrcecrcecrecrc");
        virtualCPU.add_sequence("cccrcqcrcecrecrcqcrdecrcecrecrcqcrdecrcecrecrecrcc");
        virtualCPU.add_sequence("ccclcckdcccbeccfccdtceskccesccrcccesccclcckcccfccd");
        virtualCPU.add_sequence("cckeccfccclcccepcccdqcclcccfccdctcesccoesccoesccrc");
        virtualCPU.add_sequence("cccdqcclccckeccfccdctceskccepccckccpskccctcccciccc");
        virtualCPU.add_sequence("ccctceskccesccrcqcrdecrccccqsccoesccoescccicccbecc");
        virtualCPU.add_sequence("ccccdqcclcckccepcccbeccfccccdtceskccepccciccctccct");
        virtualCPU.add_sequence("ccdctcesccoesccrcecrcecrecrecrecrcccdtceskcceskcce");

        CHECK(virtualCPU.get_sequence_size() == 19);
        **/
    }   

}

//create graph with loops and stuff and test simple addition and removal, is valid, num seqs, size, contains_branch, endpoints, maybe visited value
// double check where the rest flag funcitons are being used