/**
 * @file unit_test.cpp
 * @author Anna Catenacci
 *
 * A file to quickly test if everything is running properly
 */


// CATCH
#define CATCH_CONFIG_MAIN
#include "../../../mabe/MABE2/source/third-party/empirical/third-party/Catch/include/catch.hpp"

#include "../DeBruijn/DeBruijnGraph.hpp"


TEST_CASE("DeBruijnGraph__Constructors", "[DeBruijn/DeBruijnGraph.hpp]"){
{
    REQUIRE(1 == 1);
    //this is a good place to check getters and setters i think. or in a diferent test.
  }
}

TEST_CASE("DeBruijnGraph__valid", "[DeBruijn/DeBruijnGraph.hpp]"){
{
    REQUIRE(1 == 1);
    //try valid and invalid sequences
  }
}

TEST_CASE("DeBruijnGraph__adj_list", "[DeBruijn/DeBruijnGraph.hpp]"){
{
    REQUIRE(1 == 1);
    //make sure modify_org SC and NSC are the same and that availible_adj_list gets the same thing as a normal adj_list at first
    //make sure size of adj_list is right after adding and removing lots of numbers. this should also be done in constructor test case
  }
}

TEST_CASE("DeBruijnGraph__add_sequence", "[DeBruijn/DeBruijnGraph.hpp]"){
{
    REQUIRE(1 == 1);
    //make sure flags are all right here?
  }
}

TEST_CASE("DeBruijnGraph__remove_sequence", "[DeBruijn/DeBruijnGraph.hpp]"){
{
    REQUIRE(1 == 1);
    //check flags and sizes of things
  }
}

TEST_CASE("DeBruijnGraph__modify_sequence", "[DeBruijn/DeBruijnGraph.hpp]"){
{
    REQUIRE(1 == 1);
    //check that it adds and removes the sequence from the graph at the end using valid
  }
}
