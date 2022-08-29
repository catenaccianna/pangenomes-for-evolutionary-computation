/**
 * @file DeBruijnValue.cpp
 * @author Anna Catenacci
 */

#include "DeBruijnValue.hpp"
#include "DeBruijnGraph.hpp"
#define CATCH_CONFIG_MAIN
#include "../../../mabe/MABE2/source/third-party/empirical/third-party/Catch/single_include/catch2/catch.hpp"

TEST_CASE("DeBruijnValue__variable-values", "[DeBruijnValue.hpp]")
{
    {
        CHECK( 1 == 1 );
    }
}

TEST_CASE("DeBruijnValue__adj-lists", "[DeBruijnValue.hpp]")
{
    {
        DeBruijnGraph pangenome_graph;

        pangenome_graph.add_sequence("1111000110111101110101100101000010101110000001011000011101110101000001110000100101110100111010100110");
        pangenome_graph.add_sequence("1111111110111100011110100010101010101110010011100111010000101110001101010010110010010111110111100011");
        pangenome_graph.add_sequence("1101100110101001001111110000100010110001101010100100101100010000111001101001011000011010111001000001");
        pangenome_graph.add_sequence("1001010100101010100101111100111001101100000101110110001010010011110110110010111111111111101101101111");
        pangenome_graph.add_sequence("1110001001111100101011001011011100101000101000000000011110100101001100010101000000001010111011101010");
        pangenome_graph.add_sequence("0101000101001000001011010110101010011101000100101100000111000100000010010010011111101011101001010011");
        pangenome_graph.add_sequence("0100011001111111010100010011000100001101101001100011011001101101111001000110001110110000110101111011");
        pangenome_graph.add_sequence("1100000000000101101010001010000101001000011011001111110100110101101011101010101101100101010110100111");
        pangenome_graph.add_sequence("1101011111110101101010010011110101001011010101100011000000001011111110110101100110110111010101010111");
        pangenome_graph.add_sequence("1010110110001010000011000111000101101101100101010001101111000111001000011010101111010110110000001010");
        pangenome_graph.add_sequence("1100010101110001101101101000010000001000101100011011001110101001111001101011101101011000111110110011");
        pangenome_graph.add_sequence("0100100111000010110000001000001101101111001100011100110100001000010001000111010100001111000010010010");
        pangenome_graph.add_sequence("1110010111001111101100101100001100001111110110000110101111011010100001010110111001111111101000100101");
        pangenome_graph.add_sequence("0010010100110000100111011110111101011101011110110100001101100111001001101000101110110001100001100111");
        pangenome_graph.add_sequence("0100010111101101011011101011101101110111001110010010000100111101001011010110000110101111111110111000");
        pangenome_graph.add_sequence("1100000011110100111100001010101100010100111010010001100111010110110011011101111011110101110010001001");
        pangenome_graph.add_sequence("0000100011100111011001101111011001111110001101000011000010001110101101100001110010101110010011100110");
        pangenome_graph.add_sequence("0001100101100011000001111000011100010000010110111000100010101101001010101111110000001101010001000101");
        pangenome_graph.add_sequence("0011101001110000110001111111110101011100011011101101010100101010000111111010111001001010010011110011");
        pangenome_graph.add_sequence("0001011111111000000001000100110000110010010101111101111011011011000110110101011010000001100111011001");

        // CHECK SETTING OF AVAILIBLE ADJ LIST LOGIC
        pangenome_graph.set_avail_adj_list("000");
        pangenome_graph.set_avail_adj_list("001");
        pangenome_graph.set_avail_adj_list("010");
        pangenome_graph.set_avail_adj_list("011");
        pangenome_graph.set_avail_adj_list("100");
        pangenome_graph.set_avail_adj_list("101");
        pangenome_graph.set_avail_adj_list("110");
        pangenome_graph.set_avail_adj_list("111");

        CHECK(pangenome_graph.get_value("000").adj_availible_size()==pangenome_graph.get_value("000").adj_list_size());
        CHECK(pangenome_graph.get_value("001").adj_availible_size()==pangenome_graph.get_value("001").adj_list_size());
        CHECK(pangenome_graph.get_value("010").adj_availible_size()==pangenome_graph.get_value("010").adj_list_size());
        CHECK(pangenome_graph.get_value("011").adj_availible_size()==pangenome_graph.get_value("011").adj_list_size());
        CHECK(pangenome_graph.get_value("100").adj_availible_size()==pangenome_graph.get_value("100").adj_list_size());
        CHECK(pangenome_graph.get_value("101").adj_availible_size()==pangenome_graph.get_value("101").adj_list_size());
        CHECK(pangenome_graph.get_value("110").adj_availible_size()==pangenome_graph.get_value("110").adj_list_size());
        CHECK(pangenome_graph.get_value("111").adj_availible_size()==pangenome_graph.get_value("111").adj_list_size());

        // CHECK GETTING OF AVAILIBLE ADJ LIST LOGIC
        for (auto vertex : pangenome_graph.get_all_vertices()){
            for (int i = 0; i < pangenome_graph.get_value(vertex).adj_availible_size(); i++){
                CHECK(pangenome_graph.get_value(vertex).get_adj_availible(i)==pangenome_graph.get_value(vertex).get_adj_availible(i));
            }
        }

        // CHECK REMOVING OF AVAILIBLE ADJ LIST LOGIC

        //void remove_avail_adj_list(string kmer, string adj) { mVertices[kmer].remove_adj_availible(adj); }
        //mVertices[current].remove_adj_availible(next);

        pangenome_graph.remove_avail_adj_list("000", 0);
        pangenome_graph.remove_avail_adj_list("001", 0);
        pangenome_graph.remove_avail_adj_list("010", 0);
        pangenome_graph.remove_avail_adj_list("011", 0);

        CHECK(pangenome_graph.get_value("000").adj_availible_size()==pangenome_graph.get_value("000").adj_list_size()-1);
        CHECK(pangenome_graph.get_value("001").adj_availible_size()==pangenome_graph.get_value("001").adj_list_size()-1);
        CHECK(pangenome_graph.get_value("010").adj_availible_size()==pangenome_graph.get_value("010").adj_list_size()-1);
        CHECK(pangenome_graph.get_value("011").adj_availible_size()==pangenome_graph.get_value("011").adj_list_size()-1);

        CHECK( 1 == 1 );
    }
}
