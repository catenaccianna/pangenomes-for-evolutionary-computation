/**
 * @file DeBruijnPathLength.hpp
 * @author Anna Catenacci
 *
 * Class describing the possible path lengths you could take from a node
 */

#ifndef PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNPATHLENGTH_H
#define PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNPATHLENGTH_H

#include "DeBruijnEdge.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>
#include <set>
#include <limits>
#include <tuple> 
#include <utility>
#include <cassert>
#include <stdexcept>
#include <iostream>

using std::string; using std::vector; using std::map; using std::tuple;
using std::pair; using std::set;

class DeBruijnPathLength {
private:

    /// Adjacency list with path lengths attached
    /// all possible path langths to all the adjs
    map<int, set<string>> mPathLenAdjList = {};

    /// Adjacency list with path lengths attached
    /// all possible path langths to all the adjs
    map<int, set<string>> mPathLenAdjList = {};

public:
    /// Constructors
    DeBruijnPathLength()=default;
    //DeBruijnPathLength(set<string> a, bool c) : mAdjList(a), mAvailableAdj({}) {};

    /// Destructor                                                        
    ~DeBruijnPathLength()=default;

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNPATHLENGTH_H