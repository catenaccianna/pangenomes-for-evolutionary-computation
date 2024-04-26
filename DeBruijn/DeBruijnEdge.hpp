/**
 * @file DeBruijnEdge.hpp
 * @author Anna Catenacci
 *
 * Class describing the attributes of a DeBruijnEdge object
 * Includes information about the weight of the edge between two nodes
 * This is different from the DBValue class because we are recording the 
 * current node connected to only one possible adjacent node.
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
    string mHead;

    /// Tail node (pointing from, which nodes lead TO this one, looking backwards)
    string mTail;

    /// How many times this edge appears in the graph
    int mCount = 0;

    /// Visitor flag
    int mVisits = 0;

public:
    /// Constructors
    DeBruijnEdge()=default;
    DeBruijnEdge(string origin, string destination) { mHead=origin; mTail=destination; };

    /// Destructor                                                        
    ~DeBruijnEdge()=default;

    /**
     * Set head node
     */
    //void set_head(string h) { mHead.insert(h); }
        
    /**
     * Get head node
     */
    string get_head() { return mHead; }

    /**
     * Get tail node
     */
    string get_tail() { return mTail; }

    /**
     * Increment number of times this edge has been seen in the graph
     */
    void increment_count() { mCount ++; }

    /**
     * Decrement number of times this edge has been seen in the graph
     */
    void decrement_count() { mCount --; }

    /**
     * Get number of times this edge appears in the graph
     */
    int get_count() { return mCount; }

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
        std::cout<<"Edge Info - Head: "<<mHead<<", ";
        std::cout<<" Tail: "<<mTail<<"\n";
    }

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNEDGE_H
