/**
 * @file DeBruijnGraph.cpp
 * @author Anna Catenacci
 */

#include "DeBruijnGraph.hpp"
#include "DeBruijnValue.hpp"
#define CATCH_CONFIG_MAIN
#include "../../../mabe/MABE2/source/third-party/empirical/third-party/Catch/single_include/catch2/catch.hpp"

TEST_CASE("DeBruijnGraph__construction-and-removal", "[DeBruijnGraph.hpp]")
//create graph with loops and stuff and test simple addition and removal, is valid, num seqs, size, contains_branch, endpoints, maybe visited value
{
    {
 /*       DeBruijnGraph not_a_bits_org;
        not_a_bits_org.add_sequence("12345678");
        CHECK(not_a_bits_org.get_sequence_size() == 1);
        CHECK(not_a_bits_org.get_size() == 6);
        CHECK(not_a_bits_org.get_branch_vertices().size() == 0);
        CHECK(not_a_bits_org.get_start_vertices().size() == 1);
        CHECK(not_a_bits_org.get_value("123").get_empty_bool() == 0);
        CHECK(not_a_bits_org.get_value("234").get_empty_bool() == 0);
        CHECK(not_a_bits_org.get_value("345").get_empty_bool() == 0);
        CHECK(not_a_bits_org.get_value("567").get_empty_bool() == 0);
        CHECK(not_a_bits_org.get_value("678").get_empty_bool() == 1);
        CHECK(not_a_bits_org.vertex_branch_check("123") == 0);
        CHECK(not_a_bits_org.vertex_branch_check("123") == 0);
        CHECK(not_a_bits_org.vertex_branch_check("234") == 0);
        CHECK(not_a_bits_org.vertex_branch_check("345") == 0);
        CHECK(not_a_bits_org.vertex_branch_check("567") == 0);
        CHECK(not_a_bits_org.vertex_branch_check("678") == 0);
        not_a_bits_org.add_sequence("12345000");
        CHECK(not_a_bits_org.get_sequence_size() == 2);
        CHECK(not_a_bits_org.get_size() == 9);
        CHECK(not_a_bits_org.get_branch_vertices().size() == 1);
        CHECK(not_a_bits_org.get_start_vertices().size() == 1);
        CHECK(not_a_bits_org.get_value("123").get_empty_bool() == 0);
        CHECK(not_a_bits_org.get_value("234").get_empty_bool() == 0);
        CHECK(not_a_bits_org.get_value("345").get_empty_bool() == 0);
        CHECK(not_a_bits_org.get_value("567").get_empty_bool() == 0);
        CHECK(not_a_bits_org.get_value("678").get_empty_bool() == 1);
        CHECK(not_a_bits_org.vertex_branch_check("123") == 0);
        CHECK(not_a_bits_org.vertex_branch_check("234") == 0);
        CHECK(not_a_bits_org.vertex_branch_check("345") == 1);
        CHECK(not_a_bits_org.vertex_branch_check("456") == 0);
        CHECK(not_a_bits_org.vertex_branch_check("450") == 0);
        not_a_bits_org.remove_sequence("12345678");
        CHECK(not_a_bits_org.get_sequence_size() == 1);
        CHECK(not_a_bits_org.get_size() == 6);
        //CHECK(not_a_bits_org.get_branch_vertices().size() == 0);
        CHECK(not_a_bits_org.get_start_vertices().size() == 1);
        CHECK(!not_a_bits_org.is_valid("12345678"));
        CHECK(not_a_bits_org.is_valid("12345000"));
        CHECK(not_a_bits_org.get_value("500").get_empty_bool() == 0);
        CHECK(not_a_bits_org.get_value("000").get_empty_bool() == 1);
        CHECK(not_a_bits_org.vertex_branch_check("123") == 0);
        CHECK(not_a_bits_org.vertex_branch_check("234") == 0);
        //CHECK(not_a_bits_org.vertex_branch_check("345") == 0);
        CHECK(not_a_bits_org.vertex_branch_check("450") == 0);
        CHECK(not_a_bits_org.vertex_branch_check("500") == 0);
        not_a_bits_org.add_sequence("99945000");
        CHECK(not_a_bits_org.get_sequence_size() == 2);
        CHECK(not_a_bits_org.get_size() == 9);
        //CHECK(not_a_bits_org.get_branch_vertices().size() == 0);
        CHECK(not_a_bits_org.get_start_vertices().size() == 2);
*/
        //create graph with loops and stuff and test simple addition and removal, is valid, num seqs, size, contains_branch, endpoints, maybe visited value
        DeBruijnGraph g;
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

        CHECK(g.get_sequence_size() == 11);
        
        CHECK(g.get_size() == 8);
        CHECK(g.is_valid("1111000110111101110101100101000010101110000001011000011101110101000001110000100101110100111010100110"));
        CHECK(g.is_valid("1111111110111100011110100010101010101110010011100111010000101110001101010010110010010111110111100011"));
        CHECK(g.is_valid("1101100110101001001111110000100010110001101010100100101100010000111001101001011000011010111001000001"));
        CHECK(g.is_valid("1001010100101010100101111100111001101100000101110110001010010011110110110010111111111111101101101111"));
        CHECK(g.is_valid("1110001001111100101011001011011100101000101000000000011110100101001100010101000000001010111011101010"));
        CHECK(g.is_valid("0101000101001000001011010110101010011101000100101100000111000100000010010010011111101011101001010011"));
        CHECK(g.is_valid("1100010101110001101101101000010000001000101100011011001110101001111001101011101101011000111110110011"));

        ///@note is_valid isn't really working on bits orgs because the genomes have so few possible values and therefore are so similar
        //CHECK(!g.is_valid("1100010101110001100001101000010000001000101100011011001110101001111001101011101101011000111110110011"));
        //CHECK(!g.is_valid("1100010101110001101101101000010000001000101100011011001110101011111001101011101101011000110110110011"));
        //CHECK(!g.is_valid("1100000100000101101010001010000101001000011011001111110100110101101011101010101101100101010110100111"));
        //CHECK(!g.is_valid("0100011001111111010100010011000100001101101001100011011001101101111001000111001110110000110101111011"));
        //CHECK(!g.is_valid("1101011111110101101010012311110101001011010101100011000000001011111110110101100110110111010101010111"));


        g.remove_sequence("1111000110111101110101100101000010101110000001011000011101110101000001110000100101110100111010100110");
        CHECK(g.get_sequence_size() == 10);

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

TEST_CASE("DeBruijnGraph__loops-and-weights", "[DeBruijnGraph.hpp]")
//create graph with loops and stuff and test simple addition and removal, is valid, num seqs, size, contains_branch, endpoints, maybe visited value
{
    DeBruijnGraph g;
    g.add_sequence("ABCDBCDE");
    g.update_loops();
    DBGraphValue bcd = g.get_value("BCD");
    CHECK(bcd.get_loop_flag() == 1);

    DBGraphValue cde = g.get_value("CDE");
    CHECK(cde.get_endpoint() == 1);

    g.update_weights();

    /**for( auto i : g.get_all_vertices()){
        DBGraphValue val = g.get_value(i);
        std::cout<<"Val "<<i<<" Max "<<std::get<0>(val.get_max_len())<<" "<<std::get<1>(val.get_max_len())<<" Min "<<std::get<0>(val.get_min_len())<<" "<<std::get<1>(val.get_min_len())<<std::endl;
    }**/

    


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

    

}