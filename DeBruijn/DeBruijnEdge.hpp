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
#include <set>

using std::string; using std::vector; using std::set;

class DeBruijnEdge {
private:

    /// Head node (pointing to, which nodes go out from this one, looking forward)
    set<string> mHead;

    /// Map specifying how many of each possible adjacency are in the graph
    /// Map version of the set mHeads (which contains the adjacencies from mTail)
    //map<string, int> mCounts;

    /// 
    //map<int, set<string>> mPathLenAdjList = {};

    /// Tail node (pointing from, which nodes lead TO this one, looking backwards)
    set<string> mTail;

    /// Visitor flag
    int mVisits = 0;

public:
    /// Constructors
    DeBruijnEdge()=default;
    DeBruijnEdge(string origin, string destination) { mHead.insert(origin); mTail.insert(destination); };

    /// Destructor                                                        
    ~DeBruijnEdge()=default;

    /**
     * Set head node
     */
    void set_head(string h) { mHead.insert(h); }
        
    /**
     * Get head node
     */
    std::set<string> get_head() { return mHead; }

    /**
     * Remove a head node
     */
    void remove_head(string h) { mHead.erase(h); }

    /**
     * Set tail node
     */
    void set_tail(string t) { mTail.insert(t); }
        
    /**
     * Get tail node
     */
    //std::vector<std::shared_ptr<DBGraphValue>> get_tail() { return mTail; }
    std::set<string> get_tail() { return mTail; }

    /**
     * Remove a tail node
     */
    void remove_tail(string t) { mTail.erase(t); }

    /**
     * Get the visitor flag object
     * @return true if this vertex has already been visited in a traversal
     */
    int get_visits(){ return mVisits; }

    /**
     * Set visitor flag
     * @param value true if visited, false if not
     */
    void increment_edge_visitor_flag() { mVisits+=1; }

    /**
     * Set visitor flag to 0
     */
    void clear_edge_visitor_flag() { mVisits = 0; }

    void show_edge() {
        std::cout<<"Edge Info - Head: ";
        for (auto i : mHead) {
            std::cout<<i<<", ";
        }
        std::cout<<" Tail: ";
        for (auto i : mTail) {
            std::cout<<i<<", ";
        }
        std::cout<<"\n";}

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNEDGE_H
