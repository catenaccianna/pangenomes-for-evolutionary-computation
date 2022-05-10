#include "DeBruijnGraph.h"
#include <vector>

void TestConstructGraph() {
    cout << "[Empty DeBruijn Graph] \n";
    DeBruijnGraph g;
    cout << "Size = " << g.get_size() << "\n";
    g.depth_first_traversal( [] (DeBruijnVertex vertex) { cout << vertex.get_kmer() << "\n"; });

    cout << "\n DeBruijn Graph size 1: \n";
    std::vector<int> vec({4,6,2});
    DeBruijnGraph g0 = DeBruijnGraph(vec, 3);
    cout << "Size = " << g0.get_size() << "\n";
    g0.depth_first_traversal([] (DeBruijnVertex vertex) { cout << vertex.get_kmer() << "\n"; });

    cout << "\n DeBruijn Graph from vector of ints: \n";
    vec = {4,6,2,8,9,3,5,7};
    DeBruijnGraph g1 = DeBruijnGraph(vec, 3);
    cout << "Size = " << g1.get_size() << "\n";
    // Traverse graph here
    // It should have the following sequence:
    // 462 -> 628 -> 289 -> 893 -> 935 -> 357
    g1.depth_first_traversal([] (DeBruijnVertex vertex) { cout << vertex.get_kmer() << "\n"; });

    cout << "\n DeBruijn Graph from int: \n";
    int num = 12345678;
    DeBruijnGraph g2 = DeBruijnGraph(num, 3);
    cout << "Size = " << g2.get_size() << "\n";
    g2.depth_first_traversal([] (DeBruijnVertex vertex) { cout << vertex.get_kmer() << "\n"; });

    cout << "\n DeBruijn Graph from string: \n";
    string str = "98765432";
    DeBruijnGraph g3 = DeBruijnGraph(str, 4);
    cout << "Size = " << g3.get_size() << "\n";
    g3.depth_first_traversal([] (DeBruijnVertex vertex) { cout << vertex.get_kmer() << "\n"; });

    cout << "\n DeBruijn Graph from vector of strings: \n";
    std::vector<string> vec2({"2","3","4","5","6","7","8","9"});
    DeBruijnGraph g4 = DeBruijnGraph(vec2, 5);
    cout << "Size = " << g4.get_size() << "\n";
    g4.depth_first_traversal([] (DeBruijnVertex vertex) { cout << vertex.get_kmer() << "\n"; });
}

void TestAddVertex() {

}

///@todo Need to go over if I understand what a branch means as far as sequence goes
void TestBranchingGraph() {
    // no branches
    std::vector<int> vec({4,6,2,8,9,3,5,7});
    DeBruijnGraph g = DeBruijnGraph(vec, 3);
    //g.depth_first_traversal([&g] (DeBruijnVertex vertex) { cout << g.vertex_branch_check(vertex) << "\n"; });

    // one branch: "628" points to both "289" and "280"
    vec = {4, 6,2,8, 9, 6,2,8, 0};
    DeBruijnGraph g0 = DeBruijnGraph(vec, 3);


    // multiple branches: "289" -> "896", "894" and "967" -> "672", "678"
    vec = {4,6, 2,8,9, 6,7, 2,8,9, 4, 9,6,7, 8};
    DeBruijnGraph g1 = DeBruijnGraph(vec, 3);
}

void TestTraversalLambda() {
    //can maybe modify this depending on what we're traversing for--
    //if we need to compare adjacent verticies can make a 2-vertex parameter!

    std::vector<int> vec({4,6,2,8,9,3,5,7});
    DeBruijnGraph g = DeBruijnGraph(vec, 3);

    //print all the verticies and their adjacency lists
    g.depth_first_traversal([&g] (DeBruijnVertex vertex) { 
        cout << vertex.get_kmer() << "->";
        vector<DeBruijnVertex> adj_list = g.get_value(vertex).get_adj_list();

        for(int i = 0; i < int(adj_list.size()); ++i){
            cout << adj_list[i].get_kmer() << ",";
        } 

        cout << "\n"; });
}

int main() {
    //TestConstructGraph();
    TestBranchingGraph();
    //TestTraversalLambda();
}