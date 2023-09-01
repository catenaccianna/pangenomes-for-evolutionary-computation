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

class DBGraphEdge {
private:
    /// Weight of edge
    int mWeight = 0;

    /// Head node (pointing to)
    std::shared_ptr<DBGraphValue> mHead = nullptr;

    /// Tail node (pointing from)
    std::shared_ptr<DBGraphValue> mTail = nullptr;

public:
    /// Constructors
    DBGraphEdge()=default;
    //DBGraphEdge(vector<string> a, bool c) : mWeight(w), mContainsBranch(c), mAvailableAdj({}) {};

    /// Destructor                                                        
    ~DBGraphEdge()=default;

    /**
     * Set weight
     */
    void set_weight(int w) { mWeight = w; }
        
    /**
     * Get weight
     */
    int get_weight() { return mWeight; }

    /**
     * Set head node
     */
    void set_head(std::shared_ptr<DBGraphValue> h) { mHead = h; }
        
    /**
     * Get head node
     */
    std::shared_ptr<DBGraphValue> get_head() { return mHead; }

    /**
     * Set tail node
     */
    void set_tail(std::shared_ptr<DBGraphValue> t) { mTail = t; }
        
    /**
     * Get tail node
     */
    std::shared_ptr<DBGraphValue> get_tail() { return mTail; }

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNEDGE_H
