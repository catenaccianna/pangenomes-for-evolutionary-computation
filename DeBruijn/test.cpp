#include "DeBruijnGraph.h"
#include <vector>
#include <algorithm>
#include <string>

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
    cout<<"\"empty bool\" represents whether or not there is an empty node in the vertex's adjacency list (this would happen if the node is the last one)\n";
    cout<<"\"contains branch\" is a boolean representing whether the vertex contains >1 adjacent vertex (a branch)\n";

    // no branches
    std::vector<int> vec({4,6,2,8,9,3,5,7});
    DeBruijnGraph g = DeBruijnGraph(vec, 3);
    g.breadth_first_traversal([&g] (string vertex) { 
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

    g0.breadth_first_traversal([&g0] (string vertex) { 
        cout << vertex<< " empty bool-"<<g0.get_value(vertex).get_empty_bool()<<
        " contains branch-"<<g0.get_value(vertex).get_branch() << "\n"; });

    cout<<"\n";



///         THIS ONE BELOW IS NOT IN ORDER

    // multiple branches: "289" -> "896", "894" and "967" -> "672", "678"
    vec = {4,6, 2,8,9, 6,7, 2,8,9, 4, 9,6,7, 8};
    DeBruijnGraph g1 = DeBruijnGraph(vec, 3);
    cout<<"Verticies in this graph that contain >1 adjacencies, or a branch: ";
    for(int i = 0; i<int(g1.get_branch_vertices().size()); ++i){
        cout << g1.get_branch_vertices()[i] <<", ";
    }
    cout<<"\n";
    g1.breadth_first_traversal([&g1] (string vertex) { 
        cout << vertex<< " empty bool-"<<g1.get_value(vertex).get_empty_bool()<<
        " contains branch-"<<g1.get_value(vertex).get_branch() << "\n"; });

    cout<<"\nThis one is not in order, I think because of how the branches were added into and popped off the queue, \nbut does this matter? Ask Emily about this!\n";

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
    g2.breadth_first_traversal([&g2] (string vertex) { 
        cout << vertex<< " empty bool-"<<g2.get_value(vertex).get_empty_bool()<<
        " contains branch-"<<g2.get_value(vertex).get_branch() << "\n"; });

}

void TestAddSequence() {
    cout<<"\nADD SEQUENCE TEST\n";
    cout<<"This test is meant to test that traversing and adding sequences work in unison\n\n";
    // one branch (at 123)
    string str1 = "12345";
    DeBruijnGraph g = DeBruijnGraph(str1, 3);
    cout << "Original graph:\n";
    g.display();

    cout << "\nAfter first sequence (branch from 123, different ends):\n";
    g.add_sequence("1236");
    g.display();

    /*for(auto i: g.get_all_vertices()){
        cout<<"\n";
        cout<<i<<"->";
        for (auto i : g.get_value(i).get_adj_list()){
            cout<<i<<" ";
        }
    }*/

    // multiple limbs on the same branch (at 123)
    cout << "\nAfter second sequence (another branch from 123, another different end):\n";
    g.add_sequence("1239476");
    g.display();

    // different beginnings
    cout << "\nAfter third sequence (different beginnings):\n";
    g.add_sequence("7239436");
    g.display();

    // different middles
    cout << "\nAfter third sequence (different beginnings):\n";
    g.add_sequence("723111436");
    g.display();

    // loop
    cout<<"\nNew graph with a loop:\n";
    DeBruijnGraph g0 = DeBruijnGraph("123456123", 3);
    g0.display();
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

    // make sure in this graph especially that there's only one node for something like 289
    // multiple limbs on the branches: "289" -> "896", "894", "895"
    vector<int> vec = {4,6, 2,8,9, 6,7, 2,8,9, 4,9, 2,8,9, 5 };
    cout<<"Testing sequence 4628967289492895... (multiple limbs on branches)\n";
    DeBruijnGraph g = DeBruijnGraph(vec, 3);
    vector<string> graph = g.get_all_vertices();
    vector<string> g_result = g.get_all_vertices();
    vector<string>::iterator it;
    it = std::unique_copy (graph.begin(), graph.end(), graph.begin(), comparison);

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
    it = std::unique_copy (graph.begin(), graph.end(), graph.begin(), comparison);
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
    it = std::unique_copy (graph.begin(), graph.end(), graph.begin(), comparison);
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
    it = std::unique_copy (graph.begin(), graph.end(), graph.begin(), comparison);
    cout<<"Testing \"branched verticies\" vector for above sequence...\n";
    branches = g2.get_branch_vertices();
    branch_result = g2.get_branch_vertices();
    it = std::unique_copy (branches.begin(), branches.end(), branch_result.begin(), comparison);

}

void TestMultipleEnds() {
    cout<<"\nMULTIPLE ENDS TEST\n";
    cout<<"This is meant to make sure that graphs with sequences that have different endings are marked correctly\n\n";
    // multiple end nodes
    //try this with just a single graph and then also when we add a sequence

    //simple one
    DeBruijnGraph g = DeBruijnGraph("12367", 3);
    g.add_sequence("12358");
    g.display();
    
    g.add_sequence("22359");
    cout<<"\nAdd another end:\n";
    g.display();

    cout<<"\nAdd a repetition/loop:\n";
    cout<<"In this case do we want to mark that 223 is an endpoint? Currently I am only checking to see if \nthe vertex is already in graph, could check beginnings and ends specifically\n";
    g.add_sequence("678223");
    g.display();
}

void TestTraversalLambda() {
    ///@note can maybe modify this depending on what we're traversing for--
    //  - if we need to compare adjacent verticies can make a 2-vertex parameter!
    //  - could also adjust so that the recursive function returns something (or a template of something) if needed

    cout<<"\nTRAVERSAL TEST\n";
    std::vector<int> vec({4,6,2,8,9,3,5,7});
    DeBruijnGraph g = DeBruijnGraph(vec, 3);

    //print all the verticies and their adjacency lists
    g.breadth_first_traversal([&g] (string vertex) { 
        cout << vertex << "->";
        vector<string> adj_list = g.get_value(vertex).get_adj_list();

        for(int i = 0; i < int(adj_list.size()); ++i){
            cout << adj_list[i] << ",";
        } 

        cout << "\n"; });
}

int main() {
    //TestConstructGraph();
    TestBranchingGraph();
    TestAddSequence();
    //TestUniqueVerticies();
    TestMultipleEnds();
    //TestTraversalLambda();
}