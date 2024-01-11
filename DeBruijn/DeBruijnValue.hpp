/**
 * @file DeBruijnValue.hpp
 * @author Anna Catenacci
 *
 * Class describing the attributes of a DeBruijnVertex object
 * Includes information about the vertex adjacency list, whether or
 * not the vertex branches, and whether it has been visited in traversal
 */

#ifndef PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNVALUE_H
#define PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNVALUE_H

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

class DeBruijnValue {
private:

    /// Adjacency list--vector of all verticies this vertex points to
    set<string> mAdjList = {};

    /// Adjacency list with path lengths attached
    /// all possible path langths to all the adjs
    //map<int, set<pair<string, int>>> mPathLenAdjList = {};
    map<int, set<string>> mPathLenAdjList = {};
    // vector of a struct (containing kmer and count) or a pair (of the same thing)

    /// Adjacencies availible to use in genome modification
    set<string> mAvailableAdj = {};

    /// Visitor flag
    // so you'd to to the adj list being pointed to, and then check the index to be accessed, and is the size == 
    // the index +1, then you know this vertex is done
    int mVisits = 0;

    /// Count of the number of sequences in the graph that use this kmer
    /// Used to make sure we don't remove a duplicant sequence
    int mKmerOccurrences = 0;

    /// This is a boolean that will be 0 if this vertex is not an endpoint
    /// It will be appended to every time it is the endpoint of another sequence in the graph
    int mEndpoint = 0;

    /// This is a boolean value that is true if this vertex leads to a loop in the graph, and false if not
    int mLoop = 0;

    /// Edges into and out of this node
    DeBruijnEdge mInEdge;
    DeBruijnEdge mOutEdge;

public:
    /// Constructors
    DeBruijnValue()=default;
    DeBruijnValue(set<string> a, bool c) : mAdjList(a), mAvailableAdj({}) {};

    /// Destructor                                                        
    ~DeBruijnValue()=default;

    //* Important and informational flags to describe the node *//

    /**
     * Get the visitor flag object
     * @return true if this vertex has already been visited in a traversal
     */
    int get_visitor_flag(){ return mVisits; }

    /**
     * Set visitor flag
     * @param value true if visited, false if not
     */
    void change_visitor_flag(int value) { mVisits = value; }

    /**
     * Set visitor flag
     * @param value true if visited, false if not
     */
    void increment_visitor_flag() { mVisits++; }

    /**
     * Get the loop flag object
     * @return true if this vertex leads to a loop in the graph, false if not
     */
    int get_loop_flag(){ return mLoop; }

    /**
     * Set loop flag
     * @param value true if this vertex leads to a loop in the graph, false if not
     */
    void set_loop_flag(int value) { mLoop = value; }

    /**
     * Get the count of kmer occurrences in the pangenome
     * @return number of sequences this kmer is  used in
     */
    int get_kmer_occurrences() { return mKmerOccurrences; }

    /**
     * Increment number of sequences this kmer is  used in
     */
    void increment_kmer_occurrences() { mKmerOccurrences++; }

    /**
     * Decrement number of sequences this kmer is  used in
     */
    void decrement_kmer_occurrences() { 
        if(mKmerOccurrences == 0) {
            throw std::invalid_argument( "Kmer Occurences are already 0!\n\n" );
            //std::assert(false);
        }
        if(mKmerOccurrences == -1) {
            throw std::invalid_argument( "Kmer Occurences are -1??????????????????????\n\n" );
            //std::assert(false);
        }
        mKmerOccurrences--; 
        }

    /**
     * Get the endpoint truth value
     * @return the number of times the vertex has been observed to be an enpoint
     */
    int get_endpoint() { return mEndpoint; }

    /**
     * Append to the times this vertex is an endpoint if it has been observed ending another sequence
     */
    void increment_endpoint() { mEndpoint++; }

    /**
     * Decrement the times this vertex is an endpoint the 
     * sequence it is in is being removed
     */
    void decrement_endpoint() { mEndpoint--; }

//* Node edges (using DeBruijnEdge class) *//

    /**
     * Set edge into this node
     */
    void set_in_head(string head) { mInEdge.set_head(head); }

    /**
     * Set edge into this node
     */
    void set_in_tail(string tail) { mInEdge.set_tail(tail); }
        
    /**
     * Get edge into this node
     */
    DeBruijnEdge & get_in_edge() { return mInEdge; }

    /**
     * Set edge out of this node
     */
    void set_out_head(string head) { mOutEdge.set_head(head); }

    /**
     * Set edge out of this node
     */
    void set_out_tail(string tail) { mOutEdge.set_tail(tail); }

    /**
     * Get edge out of this node
     */
    DeBruijnEdge & get_out_edge() { return mOutEdge; }
    
//* All possible adjacencies that this node has *//

    /**
     * Get the adj list object
     * @return vector of adjacent verticies
     */
    set<string> get_adj_list() const{ return mAdjList; }

    /**
     * Get the adjacent DeBruijn Vertex
     * @param index at which to pull the vertex from
     * @return string reresenting kmer adjacency 
     */
    string get_adjacency(int index) { 
        std::set<string>::iterator it = mAdjList.begin();
        advance(it, index);
        return *it;
     }

    /**
     * Return size of adjacency list
     * @return size
     */
    int adj_list_size(){ return mAdjList.size(); }

    /**
     * Add to adjacency list (creates edge)
     * @param addition vertex to add to this vertex's adjacency list
     */
    void add_to_adj_list(string addition){ mAdjList.insert(addition); }

    /**
     * Remove edge, remove value from adjacency list
     * @param addition vertex to remove from the adjacency list
     */
    void remove_from_adj_list(string removal){ mAdjList.erase(removal); }

    /**
     * clear availible adjacencies
     */
    void clear_adj_availible() { mAvailableAdj.clear(); }

    /**
     * Check to see whether this vertex-to-adjacency path is valid
     * @param adj adjacency to check
     * @return true if the adjacency is in the adj_list for this vertex
     */
    bool valid_adj(string adj){
        if (std::find(mAdjList.begin(), mAdjList.end(), adj) != mAdjList.end()){
            return true;
        }
        return false;
    }
   
//* Only the adjacencies that we want to make availible for possible new genomes *//

    /**
     * Get the value of an available adjacency for a new genome
     * @param index of adjacency
     * @return string kmer
     */
    string get_adj_availible(int index) { 
        std::set<string>::iterator it = mAvailableAdj.begin();
        advance(it, index);
        return *it; 
    }

    /**
     * Get all availible adjacencies
     * @return vecotr containing strings of all adjacent kmers
     */
    set<string> get_all_adj_availible() { return mAvailableAdj; }

    /**
     * Get the number of adjacencies that are still valid&available to append to a new genome
     * @return int 
     */
    int adj_availible_size() { return mAvailableAdj.size(); }

    void append_adj_availible(string val) { mAvailableAdj.insert(val); }

    void append_adj_availible(set<string> val) { 
        for(auto i : val) {
            ///////// there are some blank i's at this point
            mAvailableAdj.insert(i);
        }
    }

    /**
     * Set the available adjacencies to be all adjacencies of vertex
     * Used in generating new genomes
     */
    void make_all_adj_availible() {
        mAvailableAdj = mAdjList;
    }
    
    /**
     * When we've used a kmer in a new genome as many times it appears in our sequences, we want to make it unavailable for further use
     * @param val kmer to remove from the list of available adjacencies
     * @param still_an_end if true, add the kmer to a list of kmers that have used up their availibility except for as an end
     */
    void remove_adj_availible(string val, bool still_an_end=0) { mAvailableAdj.erase(val); }

    void append_path_len(int len, string adj) {
        mPathLenAdjList[len].insert(adj);
    }

    void remove_path_len(string adj) {
        for (std::map<int, std::set<std::string>>::iterator it = mPathLenAdjList.begin(); it != mPathLenAdjList.end();) {
            set<string> & lists = it->second;
            lists.erase(adj);
            if (lists.size() < 1) {
                it = mPathLenAdjList.erase(it);
            }
            else
                ++it;
        }
    }

    void remove_inf_path() {
        auto iter = mPathLenAdjList.find(std::numeric_limits<int>::max());
        if(iter != mPathLenAdjList.end()) {
            mPathLenAdjList.erase(iter);
        }
    }

    set<string> get_all_paths() {
        set<string> results;
        for (auto i : mPathLenAdjList) {
            for (auto element : i.second) {
                results.insert(element);
            }
        }
        return results;
    }

    set<int> get_all_path_lens() {
        set<int> results;
        for (auto i : mPathLenAdjList) {
            results.insert(i.first);
        }
        return results;
    }
    
    tuple<int, set<string>> get_min_length() {
        int minimum = std::numeric_limits<int>::max();
        for (auto iter = mPathLenAdjList.begin(); iter != mPathLenAdjList.end(); iter++) {
            if(iter->first < minimum) {
                minimum = iter->first;
            }
        }
        return make_tuple(minimum, mPathLenAdjList[minimum]);
    }

    tuple<int, set<string>> get_max_length() {
        int maximum = 0;
        if (mPathLenAdjList.find(std::numeric_limits<int>::max()) != mPathLenAdjList.end()) {
            maximum = std::numeric_limits<int>::max();
        }
        else {
            for (auto iter = mPathLenAdjList.begin(); iter != mPathLenAdjList.end(); iter++) {
                if(iter->first > maximum) {
                    maximum = iter->first;
                }
            }
        }
        return make_tuple(maximum, mPathLenAdjList[maximum]);
    }

    
    void not_too_short(int current_len, int parent_len, int threshold=0) {
        for (auto i : mPathLenAdjList) {
            if (current_len + i.first >= (parent_len-threshold) || i.first == std::numeric_limits<int>::max()) {
                append_adj_availible(i.second);
            }
        }
    }

    void non_inf_paths() {
        for (auto i : mPathLenAdjList) {
            if (i.first < std::numeric_limits<int>::max()) {
                append_adj_availible(i.second);
            }
        }
    }

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNVALUE_H
