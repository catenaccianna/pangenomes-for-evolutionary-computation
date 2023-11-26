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
    //std::vector<std::shared_ptr<DBGraphValue>> mHead;
    std::vector<string> mHead;

    /// Tail node (pointing from, which nodes lead TO this one, looking backwards)
    //std::vector<std::shared_ptr<DBGraphValue>> mTail;
    std::vector<string> mTail;

    /// Visitor flag
    int mVisits = 0;

public:
    /// Constructors
    DeBruijnEdge()=default;
    DeBruijnEdge(string origin, string destination) { mHead.push_back(origin); mTail.push_back(destination); };

    /// Destructor                                                        
    ~DeBruijnEdge()=default;

    /**
     * Set head node
     */
    //void set_head(std::shared_ptr<DBGraphValue> h) { mHead.push_back(h); }
    void set_head(string h) { 
        mHead.push_back(h);
        std::sort(mHead.begin(), mHead.end());
        auto iter = std::unique(mHead.begin(), mHead.end());
        mHead.erase(iter, mHead.end()); 
    }
        
    /**
     * Get head node
     */
    //std::vector<std::shared_ptr<DBGraphValue>> get_head() { return mHead; }
    std::vector<string> get_head() { return mHead; }

    /**
     * Remove a head node
     */
    void remove_head(string h) { 
        mHead.erase(std::remove(mHead.begin(), mHead.end(), h), mHead.end());
    }

    /**
     * Set tail node
     */
    //void set_tail(std::shared_ptr<DBGraphValue> t) { mTail.push_back(t); }
    void set_tail(string t) { 
        mTail.push_back(t);
        std::sort(mTail.begin(), mTail.end());
        auto iter = std::unique(mTail.begin(), mTail.end());
        mTail.erase(iter, mTail.end()); 
    }
        
    /**
     * Get tail node
     */
    //std::vector<std::shared_ptr<DBGraphValue>> get_tail() { return mTail; }
    std::vector<string> get_tail() { return mTail; }

    /**
     * Remove a tail node
     */
    void remove_tail(string t) { 
        mTail.erase(std::remove(mTail.begin(), mTail.end(), t), mTail.end());
    }

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

    void show_edge() { std::cout<<this<<"\n";}

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNEDGE_H
