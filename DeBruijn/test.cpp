#include "DeBruijnGraph.h"
#include <vector>

void TestConstructGraph() {
    DeBruijnGraph g;
    std::vector<int> vec({4,6,2,8,9,3,5,7});
    //g.ConstructFromSequence(vec, 3);
    //g.depth_first_traversal();

    // Traverse graph here
    // It should have the following sequence:
    // 462 -> 628 -> 289 -> 893 -> 935 -> 357

    g.depth_first_traversal( [] (DeBruijnVertex vertex) { cout << vertex.get_kmer() << "\n"; });
    //the lambda that should be passed in here is to print--can try other ones too

    cout << "\n";
    DeBruijnGraph g1 = DeBruijnGraph(vec, 3);
    g1.depth_first_traversal([] (DeBruijnVertex vertex) { cout << vertex.get_kmer() << "\n"; });

    cout << "\n";
    int num = 12345678;
    DeBruijnGraph g2 = DeBruijnGraph(num, 3);
    g2.depth_first_traversal([] (DeBruijnVertex vertex) { cout << vertex.get_kmer() << "\n"; });

    cout << "\n";
    string str = "987654321";
    DeBruijnGraph g3 = DeBruijnGraph(str, 4);
    g3.depth_first_traversal([] (DeBruijnVertex vertex) { cout << vertex.get_kmer() << "\n"; });

    cout << "\n";
    std::vector<string> vec2({"2","3","4","5","6","7","8","9"});
    DeBruijnGraph g4 = DeBruijnGraph(vec2, 5);
    g4.depth_first_traversal([] (DeBruijnVertex vertex) { cout << vertex.get_kmer() << "\n"; });
}

void TestAddVertex() {

}

void TestBranchingGraph() {
    //add in test for a branching dna set
}

void TestTraversal() {

}

int main() {
    TestConstructGraph();
}