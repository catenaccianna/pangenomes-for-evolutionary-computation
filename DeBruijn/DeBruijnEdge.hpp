/**
 * @file DeBruijnEdge.hpp
 * @author Anna Catenacci
 *
 * Class describing the attributes of a DeBruijnEdge object
 * Includes information about the weight of the edge between two modes
 * 
 */

#ifndef PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNEDGE_H
#define PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNEDGE_H

#include "DeBruijnValue.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>
#include <limits>
#include <iostream>
#include <memory>

using std::string; using std::vector;

class DeBruijnEdge {
private:

    /// Head node (pointing to, which nodes go out from this one, looking forward)
    std::vector<std::shared_ptr<DBGraphValue>> mHead;

    /// Tail node (pointing from, which nodes lead TO this one, looking backwards)
    std::vector<std::shared_ptr<DBGraphValue>> mTail;

public:
    /// Constructors
    DeBruijnEdge()=default;
    //DBGraphEdge(vector<string> a, bool c) : mWeight(w), mContainsBranch(c), mAvailableAdj({}) {};

    /// Destructor                                                        
    ~DeBruijnEdge()=default;

    /**
     * Set head node
     */
    void set_head(std::shared_ptr<DBGraphValue> h) { mHead.push_back(h); }
        
    /**
     * Get head node
     */
    std::vector<std::shared_ptr<DBGraphValue>> get_head() { return mHead; }

    /**
     * Set tail node
     */
    void set_tail(std::shared_ptr<DBGraphValue> t) { mTail.push_back(t); }
        
    /**
     * Get tail node
     */
    std::vector<std::shared_ptr<DBGraphValue>> get_tail() { return mTail; }

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNEDGE_H
