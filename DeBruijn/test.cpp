#include "DeBruijnGraph.hpp"
#include <vector>
#include <algorithm>
#include <string>
#include "../../../mabe/MABE2/source/third-party/empirical/third-party/Catch/single_include/catch2/catch.hpp"
#include "../../../mabe/MABE2/source/third-party/empirical/include/emp/data/DataFile.hpp"

using std::unique; using std::string;

void TestConstructGraph() {
    
    cout << "CONSTRUCTOR TEST\n[Empty DeBruijn Graph] \n";
    DeBruijnGraph g;
    cout << "Size = " << g.get_size() << "\n";
    g.display();

    cout << "\nDeBruijn Graph size 1: \n";
    std::vector<int> vec({4,6,2});
    DeBruijnGraph h = DeBruijnGraph(vec, 3);
    cout << "Size = " << h.get_size() << "\n";
    h.display();

    cout << "\nDeBruijn Graph size 2: \n";
    vec = {4,6,2,5};
    DeBruijnGraph g0 = DeBruijnGraph(vec, 3);
    cout << "Size = " << g0.get_size() << "\n";
    g0.display();

    cout << "\nDeBruijn Graph from vector of ints: \n";
    vec = {4,6,2,8,9,3,5,7};
    DeBruijnGraph g1 = DeBruijnGraph(vec, 3);
    cout << "Size = " << g1.get_size() << "\n";
    // Traverse graph here
    // It should have the following sequence:
    // 462 -> 628 -> 289 -> 893 -> 935 -> 357
    g1.display();

    cout << "\nDeBruijn Graph from int: \n";
    int num = 12345678;
    DeBruijnGraph g2 = DeBruijnGraph(num, 3);
    cout << "Size = " << g2.get_size() << "\n";
    g2.display();

    cout << "\nDeBruijn Graph from string: \n";
    string str = "98765432";
    DeBruijnGraph g3 = DeBruijnGraph(str, 4);
    cout << "Size = " << g3.get_size() << "\n";
    g3.display();

    cout << "\nDeBruijn Graph from vector of strings: \n";
    std::vector<string> vec2({"2","3","4","5","6","7","8","9"});
    DeBruijnGraph g4 = DeBruijnGraph(vec2, 5);
    cout << "Size = " << g4.get_size() << "\n";
    g4.display();
    
}

void TestBranchingGraph() {
    cout<<"\nBRANCHING GRAPH TEST\n";
    cout<<"\"empty bool\" represents whether or not there is an empty node in the vertex's adjacency list "<<
    "(would happen if the node is the last one)\n";
    cout<<"\"contains branch\" is a boolean representing whether the vertex contains >1 adjacent vertex (a branch)\n";

    // no branches
    std::vector<int> vec({4,6,2,8,9,3,5,7});
    DeBruijnGraph g = DeBruijnGraph(vec, 3);
    cout<<"\nVerticies in this graph that contain >1 adjacencies, or a branch: ";
    for(int i = 0; i<int(g.get_branch_vertices().size()); ++i){
        cout <<g.get_branch_vertices()[i]<<", ";
    }
    cout<<"\n";
    g.depth_first_traversal([&g] (string vertex) { 
        cout << vertex<< " empty bool-"<<g.get_value(vertex).get_empty_bool()<<
        " contains branch-"<<g.get_value(vertex).get_branch() << "\n"; });

    cout<<"\n";

    // one branch: "628" points to both "289" and "280"
    vec = {4, 6,2,8, 9, 6,2,8, 0};
    DeBruijnGraph g0 = DeBruijnGraph(vec, 3);
    cout<<"Verticies in this graph that contain >1 adjacencies, or a branch: ";
    for(int i = 0; i<int(g0.get_branch_vertices().size()); ++i){
        cout <<g0.get_branch_vertices()[i]<<", ";
    }
    cout<<"\n";

    g0.depth_first_traversal([&g0] (string vertex) { 
        cout << vertex<< " empty bool-"<<g0.get_value(vertex).get_empty_bool()<<
        " contains branch-"<<g0.get_value(vertex).get_branch() << "\n"; });

    cout<<"\n";

    // multiple branches: "289" -> "896", "894" and "967" -> "672", "678"
    vec = {4,6, 2,8,9, 6,7, 2,8,9, 4, 9,6,7, 8};
    DeBruijnGraph g1 = DeBruijnGraph(vec, 3);
    cout<<"Verticies in this graph that contain >1 adjacencies, or a branch: ";
    for(int i = 0; i<int(g1.get_branch_vertices().size()); ++i){
        cout << g1.get_branch_vertices()[i] <<", ";
    }
    cout<<"\n";
    g1.depth_first_traversal([&g1] (string vertex) { 
        cout << vertex<< " empty bool-"<<g1.get_value(vertex).get_empty_bool()<<
        " contains branch-"<<g1.get_value(vertex).get_branch() << "\n"; });

    cout<<"\nNote: not in order because of traversal--\n";

    g1.display();
    cout<<"\n";

    // multiple limbs on the branches: "289" -> "896", "894", "895"
    vec = {4,6, 2,8,9, 6,7, 2,8,9, 4,9, 2,8,9, 5 };
    DeBruijnGraph g2 = DeBruijnGraph(vec, 3);
    cout<<"Verticies in this graph that contain >1 adjacencies, or a branch: ";
    for(int i = 0; i<int(g2.get_branch_vertices().size()); ++i){
        cout << g2.get_branch_vertices()[i]<<", ";
    }
    cout<<"\n";
    g2.depth_first_traversal([&g2] (string vertex) { 
        cout << vertex<< " empty bool-"<<g2.get_value(vertex).get_empty_bool()<<
        " contains branch-"<<g2.get_value(vertex).get_branch() << "\n"; });

}

void TestAddSequence() {
    cout<<"\nADD SEQUENCE TEST\n";
    cout<<"This test is meant to test that traversing and adding sequences work in unison\n";
    // one branch (at 123)
    string str1 = "12345";
    DeBruijnGraph g = DeBruijnGraph(str1, 3);
    cout << "Original graph:\n";
    cout<<"size "<<g.get_size()<<", num sequences "<<g.get_sequence_size()<<"\n";
    g.display();

    cout << "\nAfter first sequence (branch from 123, different ends):\n";
    g.add_sequence("1236");
    cout<<"size "<<g.get_size()<<", num sequences "<<g.get_sequence_size()<<"\n";
    g.display();

    // multiple limbs on the same branch (at 123)
    cout << "\nAfter second sequence (another branch from 123, another different end):\n";
    g.add_sequence("1239476");
    cout<<"size "<<g.get_size()<<", num sequences "<<g.get_sequence_size()<<"\n";
    g.display();

    // different beginnings
    cout << "\nAfter third sequence (different beginnings):\n";
    g.add_sequence("7239436");
    cout<<"size "<<g.get_size()<<", num sequences "<<g.get_sequence_size()<<"\n";
    g.display();

    // different middles
    cout << "\nAfter fourth sequence (different middles):\n";
    g.add_sequence("723111436");
    cout<<"size "<<g.get_size()<<", num sequences "<<g.get_sequence_size()<<"\n";
    g.display();
    ///239 visited twice

}

bool comparison (string i, string j) {
     if( !i.empty() && !j.empty() ){
         if( i == j){
             cout<<"Found a repetition of "<<i<<"\n";
         }
     }
    return (i==j);
}

void TestUniqueVerticies() {
    cout<<"\nUNIQUE TEST\n";
    cout<<"This is meant to show whether there are repeats of the same verticies in the graph, "<<"\n";
    cout<<"which would most likely happpen where there is a branch."<<"\n";
    cout<<"This test will print out a message whenever it comes across a copy. (So no message means we passed)\n\n";
    ///@note the print statment is in the comparison function above - looks like this is doing a whole lot of nothing w/o that

    // make sure in this graph especially that there's only one node for something like 289
    // multiple limbs on the branches: "289" -> "896", "894", "895"
    vector<int> vec = {4,6, 2,8,9, 6,7, 2,8,9, 4,9, 2,8,9, 5 };
    cout<<"Testing sequence 4628967289492895... (multiple limbs on branches)\n";
    DeBruijnGraph g = DeBruijnGraph(vec, 3);
    vector<string> graph = g.get_all_vertices();
    vector<string> g_result = g.get_all_vertices();
    vector<string>::iterator it;
    it = std::unique_copy (graph.begin(), graph.end(), g_result.begin(), comparison);

    // make sure there are unique elements in the vector that stores branch points
    cout<<"Testing \"branched verticies\" vector for above sequence...\n";
    vector<string> branches = g.get_branch_vertices();
    vector<string> branch_result = g.get_branch_vertices();
    it = std::unique_copy (branches.begin(), branches.end(), branch_result.begin(), comparison);

    // no branches
    vec = {4,6,2,8,9,3,5,7};
    cout<<"\nTesting sequence 46289357... (no branches)\n";
    DeBruijnGraph g0 = DeBruijnGraph(vec, 3);
    graph = g0.get_all_vertices();
    g_result = g0.get_all_vertices();
    it = std::unique_copy (graph.begin(), graph.end(), g_result.begin(), comparison);
    cout<<"Testing \"branched verticies\" vector for above sequence...\n";
    branches = g0.get_branch_vertices();
    branch_result = g0.get_branch_vertices();
    it = std::unique_copy (branches.begin(), branches.end(), branch_result.begin(), comparison);

    // one branch
    vec = {4, 6,2,8, 9, 6,2,8, 0};
    cout<<"\nTesting sequence 462896280... (one branch)\n";
    DeBruijnGraph g1 = DeBruijnGraph(vec, 3);
    graph = g1.get_all_vertices();
    g_result = g1.get_all_vertices();
    it = std::unique_copy (graph.begin(), graph.end(), g_result.begin(), comparison);
    cout<<"Testing \"branched verticies\" vector for above sequence...\n";
    branches = g1.get_branch_vertices();
    branch_result = g1.get_branch_vertices();
    it = std::unique_copy (branches.begin(), branches.end(), branch_result.begin(), comparison);

    // multiple branches
    vec = {4,6, 2,8,9, 6,7, 2,8,9, 4, 9,6,7, 8};
    cout<<"\nTesting sequence 462896728949678... (multiple branches)\n";
    DeBruijnGraph g2 = DeBruijnGraph(vec, 3);
    graph = g2.get_all_vertices();
    g_result = g2.get_all_vertices();
    it = std::unique_copy (graph.begin(), graph.end(), g_result.begin(), comparison);
    cout<<"Testing \"branched verticies\" vector for above sequence...\n";
    branches = g2.get_branch_vertices();
    branch_result = g2.get_branch_vertices();
    it = std::unique_copy (branches.begin(), branches.end(), branch_result.begin(), comparison);

    // MABE sequence
    cout<<"\nTesting sequence 1111000110111101110101100101000010101110000001011000011101110101000001110000100101110100111010100110...\n";
    string organism = "1111000110111101110101100101000010101110000001011000011101110101000001110000100101110100111010100110";
    DeBruijnGraph g3 = DeBruijnGraph(organism, 3);
    graph = g3.get_all_vertices();
    g_result = g3.get_all_vertices();
    it = std::unique_copy (graph.begin(), graph.end(), g_result.begin(), comparison);
    cout<<"Testing \"branched verticies\" vector for above sequence...\n";
    branches = g3.get_branch_vertices();
    branch_result = g3.get_branch_vertices();
    it = std::unique_copy (branches.begin(), branches.end(), branch_result.begin(), comparison);

    // MABE sequence
    cout<<"\nTesting sequence 1111111110111100011110100010101010101110010011100111010000101110001101010010110010010111110111100011...\n";
    organism = "1111111110111100011110100010101010101110010011100111010000101110001101010010110010010111110111100011";
    DeBruijnGraph g4 = DeBruijnGraph(organism, 3);
    graph = g4.get_all_vertices();
    g_result = g4.get_all_vertices();
    it = std::unique_copy (graph.begin(), graph.end(), g_result.begin(), comparison);
    cout<<"Testing \"branched verticies\" vector for above sequence...\n";
    branches = g4.get_branch_vertices();
    branch_result = g4.get_branch_vertices();
    it = std::unique_copy (branches.begin(), branches.end(), branch_result.begin(), comparison);

    // MABE sequence
    cout<<"\nTesting sequence 0101000101001000001011010110101010011101000100101100000111000100000010010010011111101011101001010011...\n";
    organism = "0101000101001000001011010110101010011101000100101100000111000100000010010010011111101011101001010011";
    DeBruijnGraph g5 = DeBruijnGraph(organism, 3);
    graph = g5.get_all_vertices();
    g_result = g5.get_all_vertices();
    it = std::unique_copy (graph.begin(), graph.end(), g_result.begin(), comparison);
    cout<<"Testing \"branched verticies\" vector for above sequence...\n";
    branches = g5.get_branch_vertices();
    branch_result = g5.get_branch_vertices();
    it = std::unique_copy (branches.begin(), branches.end(), branch_result.begin(), comparison);

}

void TestMultipleEnds() {
    cout<<"\nMULTIPLE ENDS TEST\n";
    cout<<"This is meant to make sure that graphs with sequences that have different endings are marked correctly\n";

    DeBruijnGraph g = DeBruijnGraph("12367", 3);
    g.add_sequence("12358");
    g.display();
    
    g.add_sequence("22359");
    cout<<"\nAdd another end:\n";
    g.display();
    ///235 visited twice

    cout<<"\nAdd a repetition/loop:\n";
    cout<<"In this case do we want to mark that 223 is an endpoint? Currently I am only checking to see if "<<
    "\nthe vertex is already in graph, could check beginnings and ends specifically\n";
    g.add_sequence("678223");
    g.display();
}

void TestRepetition() {
    cout<<"\nREPETITION TEST\n";
    cout<<"This is meant to detect loops and repetition.\n";
    cout<<"In the case that a node is both an endpoint and repeated within the sequence, \n"<<
    "it's adjacency list should contain an empty node as well as any other adjacencies.\n\n";

    // loop
    cout<<"New graph with a loop:\n";
    DeBruijnGraph g0 = DeBruijnGraph("123456123", 3);
    g0.display();

    // repetition -> loop
    cout<<"\nNew graph with an added sequence that creates a loop:\noriginal:\n";
    DeBruijnGraph g1 = DeBruijnGraph("1234567", 3);
    g1.display();
    cout<<"\nafter addition:\n";
    g1.add_sequence("98567123");
    g1.display();
    ///123 visited twice

    cout<<"\nNew graph with a completely repetitive sequence:\n";
    cout<<"(Currently, the sequence 55555 will traverse 2 555 kmers instead of 3, has a 555 value in branch vector, and has 1 value 555 in vertex map)\n";
    cout<<"Everything in here as far as functionality goes looks correct\n";
    DeBruijnGraph g2 = DeBruijnGraph("55555", 3);
    //cout<<"generated: "<<g2.next_genome_logic(2, "555")<<"\n";
    g2.display();
    for(auto i : g2.get_branch_vertices()){
        cout<<"branched "<<i<<"\n";
    }
    cout<<"number of times appears in sequence: "<<g2.get_value("555").get_sequence_count()<<"\n";
}

void TestGenerateSequence() {
    cout<<"\nGENERATE SEQUENCE: This is meant test the application to MABE.\n";
    cout<<"In a graph with multiple added sequences, a reasonable new sequence should be returned \n"<<
    "describing the new genetic information of the net generation offspring.\n\n";

    cout<<"Add 4 10-bit sequences to graph: (these have same beginnings)\n";
    cout<<"0123456789\n0129643789\n0125555666\n0129655688\n\n";
    DeBruijnGraph g = DeBruijnGraph("0123456789", 3);
    g.add_sequence("0129643789");
    g.add_sequence("0125555666");
    g.add_sequence("0129655688");

    emp::Random random;

    cout<<"next genome "<<g.modify_org(random, "012")<<"\n";
    cout<<"next genome "<<g.modify_org(random, "012")<<"\n";
    cout<<"next genome "<<g.modify_org(random, "012")<<"\n";
    cout<<"next genome "<<g.modify_org(random, "012")<<"\n";
    cout<<"next genome "<<g.modify_org(random, "012")<<"\n"; 
    cout<<"next genome "<<g.modify_org(random, "012")<<"\n";

    cout<<"\nAdd 8 10-bit sequences to graph: (these have same beginnings, but a loop)\n";
    cout<<"0128675012\n0129643789\n0125555666\n0129655688\n0120120120\n0122876012\n0000000000\n0128764789\n\n";
    DeBruijnGraph g1 = DeBruijnGraph("0128675012", 3);
    g1.add_sequence("0129643789");
    g1.add_sequence("0125555666");
    g1.add_sequence("0129655688");
    g1.add_sequence("0120120120");
    g1.add_sequence("0122876012");
    g1.add_sequence("0000000000");
    g1.add_sequence("0128764789");
    //this does get stuck so we'll fix it
    cout<<"next genome "<<g1.modify_org(random, "0120120120")<<"\n";
    cout<<"next genome "<<g1.modify_org(random, "0120120120")<<"\n";
    cout<<"next genome "<<g1.modify_org(random, "0120120120")<<"\n";
    cout<<"next genome "<<g1.modify_org(random, "0120120120")<<"\n";
    cout<<"next genome "<<g1.modify_org(random, "0120120120")<<"\n";
    cout<<"next genome "<<g1.modify_org(random, "0120120120")<<"\n";

}

void TestRemoveSequence() {
    //size is weird, and repetition does not disappear from graph
    cout<<"\nREMOVE SEQUENCE TEST: This is meant test the application to MABE.\n";
    cout<<"Before the death of an organism in MABE, we should be able to remove it's genome \n"<<
    "from the existing pangenome pool.\n";
    cout<<"Count represents the number of times the kmer appears in any sequence in the graph.\n";

    cout<<"\nTest a graph containing a single sequence:\n";
    DeBruijnGraph g = DeBruijnGraph("0128644", 3);
    cout<<"initial size: "<< g.get_size()<<"\n";
    cout<<"initial num sequences: "<< g.get_sequence_size()<<"\n";
    g.depth_first_traversal([&g] (string vertex) { 
       cout << vertex<< " count-"<<g.get_value(vertex).get_sequence_count()<< ", "; });
    g.remove_sequence("0128644");
    cout<<"\nsize after removal: "<<g.get_size()<<"\n";
    cout<<"num sequences after removal: "<< g.get_sequence_size()<<"\n";
    g.depth_first_traversal([&g] (string vertex) { 
        cout << vertex<< " count-"<<g.get_value(vertex).get_sequence_count()<< ", "; });

    cout<<"\nTest a graph containing a single repetitive sequence:\n";
    DeBruijnGraph g0 = DeBruijnGraph("01280127", 3);
    cout<<"initial size: "<< g0.get_size()<<"\n";
    cout<<"initial num sequences: "<< g.get_sequence_size()<<"\n";
    g0.depth_first_traversal([&g0] (string vertex) { 
        cout << vertex<< " count-"<<g0.get_value(vertex).get_sequence_count()<< ", "; });
    g0.remove_sequence("01280127");
    cout<<"\nsize after removal: "<<g0.get_size()<<"\n";
    cout<<"num sequences after removal: "<< g.get_sequence_size()<<"\n";
    g0.depth_first_traversal([&g0] (string vertex) { 
        cout << vertex<< " count-"<<g0.get_value(vertex).get_sequence_count()<< ", "; });

    cout<<"\nTest a graph containing a few sequences:\n";
    DeBruijnGraph g1 = DeBruijnGraph("0128675012", 3);
    g1.add_sequence("6543210");
    cout<<"initial size: "<< g1.get_size()<<"\n";
    cout<<"initial num sequences: "<< g.get_sequence_size()<<"\n";
    g1.depth_first_traversal([&g1] (string vertex) { 
        cout << vertex<< " count-"<<g1.get_value(vertex).get_sequence_count()<< ", "; });
    g1.remove_sequence("0128012");
    cout<<"\nsize after removal: "<<g1.get_size()<<"\n";
    cout<<"num sequences after removal: "<< g.get_sequence_size()<<"\n";
    g1.depth_first_traversal([&g1] (string vertex) { 
        cout << vertex<< " count-"<<g1.get_value(vertex).get_sequence_count()<< ", "; });

///@todo the 555555 doesn't work in display or in the removal from mVerts

    cout<<"\n\nTest a graph containing repetition in sequences:\n";
    DeBruijnGraph g2 = DeBruijnGraph("0128012", 3);
    g2.add_sequence("5555555");
    cout<<"initial size: "<< g2.get_size()<<"\n";
    cout<<"initial num sequences: "<< g.get_sequence_size()<<"\n";
    g2.depth_first_traversal([&g2] (string vertex) { 
        cout << vertex<< " count-"<<g2.get_value(vertex).get_sequence_count()<< ", "; });
    g2.remove_sequence("0128012");
    cout<<"\nsize after removal: "<<g2.get_size()<<"\n";
    cout<<"num sequences after removal: "<< g.get_sequence_size()<<"\n";
    g2.depth_first_traversal([&g2] (string vertex) { 
        cout << vertex<< " count-"<<g2.get_value(vertex).get_sequence_count()<< ", "; });
    g2.remove_sequence("5555555");
    cout<<"\nsize after 2nd removal: "<<g2.get_size()<<"\n";
    cout<<"num sequences after 2nd removal: "<< g.get_sequence_size()<<"\n";
    g2.depth_first_traversal([&g2] (string vertex) { 
        cout << vertex<< " count-"<<g2.get_value(vertex).get_sequence_count()<< ", "; });
    cout<<"\n";
}

void TestValid(){
    cout<<"\nVALIDITY TEST: Make sure we can test if a function is valid.\n";
    DeBruijnGraph g = DeBruijnGraph("0128012", 3);
    cout<<g.is_valid("0128012")<<g.is_valid("0128212")<<"\n";
    DeBruijnGraph g1 = DeBruijnGraph("0128675012", 3);
    g1.add_sequence("0129643789");
    g1.add_sequence("0125555666");
    g1.add_sequence("0129655688");
    g1.add_sequence("0120120120");
    g1.add_sequence("0122876012");
    g1.add_sequence("0000000000");
    g1.add_sequence("0128764789");
    cout<<"\nTest combinations that could be created from graph\n";
    cout<<"0128675012, 0129643789, 0125555666, 0129655688, 0120120120, 0122876012, 0000000000, 0128764789\n";
    cout<< "0120125555 IS VALID: "<<g1.is_valid("0120125555")<<"\n";
    cout<< "0120120129 IS VALID: "<<g1.is_valid("0120120129")<<"\n";
    cout<< "0120129643 IS VALID: "<<g1.is_valid("0120129643")<<"\n";
}

void TestBitOrgs(){
    cout<<"\nTEST BITSORGS\n";
    DeBruijnGraph g;
    emp::Random random;
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
    g.display();

    cout<<"next genome "<<g.modify_org(random, "111")<<"\n";
    cout<<"next genome "<<g.modify_org(random, "110")<<"\n";
    cout<<"next genome "<<g.modify_org(random, "101")<<"\n";
    cout<<"next genome "<<g.modify_org(random, "110")<<"\n";
    cout<<"next genome "<<g.modify_org(random, "111")<<"\n"; 
    cout<<"next genome "<<g.modify_org(random, "010")<<"\n";

    cout<<"next genome NSC "<<g.modify_org_NSC(random, "111")<<"\n";
    cout<<"next genome NSC "<<g.modify_org_NSC(random, "110")<<"\n";
    cout<<"next genome NSC "<<g.modify_org_NSC(random, "101")<<"\n";
    cout<<"next genome NSC "<<g.modify_org_NSC(random, "110")<<"\n";
    cout<<"next genome NSC "<<g.modify_org_NSC(random, "111")<<"\n"; 
    cout<<"next genome NSC "<<g.modify_org_NSC(random, "010")<<"\n";
    
}

void TestCsv(){
    DeBruijnGraph g;
    emp::Random random;
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
    emp::DataFile data = emp::DataFile("DeBruijnGraph");
    using fun_t = void(std::ostream &);
    g.csv("200");

}

void TestSeqCount(){
    DeBruijnGraph g;
    emp::Random random;
    string seq = "1111000110111101110101100101000010101110000001011000011101110101000001110000100101110100111010100110";
    string seq1 = "1111111110111100011110100010101010101110010011100111010000101110001101010010110010010111110111100011";
    string seq2 = "1101100110101001001111110000100010110001101010100100101100010000111001101001011000011010111001000001";
    string seq3 = "1001010100101010100101111100111001101100000101110110001010010011110110110010111111111111101101101111";
    string seq4 = "1110001001111100101011001011011100101000101000000000011110100101001100010101000000001010111011101010";
    string seq5 = "0101000101001000001011010110101010011101000100101100000111000100000010010010011111101011101001010011";
    string seq6 = "0100011001111111010100010011000100001101101001100011011001101101111001000110001110110000110101111011";
    string seq7 = "1100000000000101101010001010000101001000011011001111110100110101101011101010101101100101010110100111";
    string seq8 = "1101011111110101101010010011110101001011010101100011000000001011111110110101100110110111010101010111";
    string seq9 = "1010110110001010000011000111000101101101100101010001101111000111001000011010101111010110110000001010";
    string seq0 = "1100010101110001101101101000010000001000101100011011001110101001111001101011101101011000111110110011";
    seq += seq1+seq2+seq3+seq4+seq5+seq6+seq7+seq8+seq9+seq0;

    auto n = std::count(seq.begin(), seq.end(), '000');
    cout<<"\nCount 000: "<<n<<"\n";
    n = std::count(seq.begin(), seq.end(), '001');
    cout<<"\nCount 001: "<<n<<"\n";
    n = std::count(seq.begin(), seq.end(), '011');
    cout<<"\nCount 011: "<<n<<"\n";
    n = std::count(seq.begin(), seq.end(), '010');
    cout<<"\nCount 010: "<<n<<"\n";
    n = std::count(seq.begin(), seq.end(), '100');
    cout<<"\nCount 100: "<<n<<"\n";
    n = std::count(seq.begin(), seq.end(), '101');
    cout<<"\nCount 101: "<<n<<"\n";
    n = std::count(seq.begin(), seq.end(), '110');
    cout<<"\nCount 110: "<<n<<"\n";
    n = std::count(seq.begin(), seq.end(), '111');
    cout<<"\nCount 111: "<<n<<"\n";
    
}

int main() {
    //not sure if I've really tested attributes of the verticies and values and graph throughout this (size, adjlist, mbranch, mvert)

    // TestConstructGraph();
    // TestBranchingGraph();
    // TestAddSequence();
    // TestUniqueVerticies();
    // TestMultipleEnds();
    // TestRepetition();
    // TestGenerateSequence();
    // TestRemoveSequence();
    // TestValid();
    // TestBitOrgs();
      TestCsv();
    //TestSeqCount();
}