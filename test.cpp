#include "de_bruijn_graph.h"
#include <vector>

void TestConstructGraph(){
    DeBruijnGraph g;
    std::vector<int> vec({4,6,2,8,9,3,5,7});
    g.ConstructFromSequence(vec, 3);

    // Traverse graph here
    // It should have the following sequence:
    // 462 -> 628 -> 289 -> 893 -> 935 -> 357


}


int main() {
    TestConstructGraph();
}