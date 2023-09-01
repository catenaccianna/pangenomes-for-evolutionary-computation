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

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>
#include <limits>
#include <tuple>

#include <iostream>

using std::string; using std::vector; using std::map; using std::tuple;

class DBGraphValue {
private:

    /// Adjacency list--vector of all verticies this vertex points to
    vector<string> mAdjList = {};

    /// Adjacencies availible to use in genome modification
    vector<string> mAvailableAdj = {};

    // /// Map of each adj to their min value and max value
    // map<string, tuple<int, int> > mMinMax;

    /// Visitor flag
    // so you'd to to the adj list being pointed to, and then check the index to be accessed, and is the size == 
    // the index +1, then you know this vertex is done
    int mVisits = 0;

    /// True if this vertex contains more than one adjacent vertex
    bool mContainsBranch = false;

    /// True if the adjacency list contains either nothing or an empty vertex
    /// 2 if unassigned, 1 if list contains either nothing or an empty vertex, 0 if no empty vertex
    int mEmptyAdjList = 2;

    /// Count of the number of sequences in the graph that use this kmer
    /// Used to make sure we don't remove a duplicant sequence
    int mKmerOccurrences = 0;

    /// This is a boolean that will be 0 if this vertex is not an endpoint
    /// It will be appended to every time it is the endpoint of another sequence in the graph
    int mEndpoint = 0;

    /// This is a boolean value that is true if this vertex leads to a loop in the graph, and false if not
    int mLoop = 0;

    /// Maximum path length possible through adjacencies and adj that allows it 
    tuple<int, string> mMax = std::make_tuple(std::numeric_limits<int>::max(), "");

    /// Minimum path length possible through adjacencies and adj that allows it 
    tuple<int, string> mMin = std::make_tuple(0, "");

    // /// Maximum path length possible through adjacencies
    // int mMaxLen = std::numeric_limits<int>::max();

    // /// Minimum path length possible through adjacencies
    // int mMinLen = 0;

    // /// Unsure if i will use this variable, but weight/probability of choosing this path in modify_org
    // int mWeight = 0;

public:
    /// Constructors
    DBGraphValue()=default;
    DBGraphValue(vector<string> a, bool c) : mAdjList(a), mContainsBranch(c), mAvailableAdj({}) {};

    /// Destructor                                                        
    ~DBGraphValue()=default;

    /**
     * Get the adj list object
     * @return vector of adjacent verticies
     */
    vector<string> get_adj_list() const{ return mAdjList; }

    /**
     * Get the adjacent DeBruijn Vertex
     * @param index at which to pull the vertex from
     * @return string reresenting kmer adjacency 
     */
    string get_adjacency(int index) { return mAdjList[index]; }

    /**
     * Return size of adjacency list
     * @return size
     */
    int adj_list_size(){ return mAdjList.size(); }

    /**
     * Add to adjacency list (creates edge)
     * @param addition vertex to add to this vertex's adjacency list
     */
    void add_to_adj_list(string addition){ 
        mAdjList.push_back(addition);
        vector<string>::iterator it;
        std::sort( mAdjList.begin(), mAdjList.end() );
        mAdjList.erase( std::unique( mAdjList.begin(), mAdjList.end() ), mAdjList.end() );
    }

    /**
     * Remove edge, remove value from adjacency list
     * @param addition vertex to remove from the adjacency list
     */
    void remove_from_adj_list(string removal){
        mAdjList.erase(std::remove(mAdjList.begin(), mAdjList.end(), removal), mAdjList.end());
    }

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
     * Set the branch flag
     * A true value implies the vertex contains a branch point.
     * @param value true if the vertex has more than one value in it's adjacency list
     */
    void set_branch(bool value) { mContainsBranch = value; }

    /**
     * Get the branch truth value
     * @return true if there is more than one value in the adjacency list
     */
    bool get_branch() { return mContainsBranch; }

    /**
     * Set the bool for whether the adjacency list is empty
     * @param value true if the vertex does not point to anything yet
     */
    void set_empty_bool(int value) { mEmptyAdjList = value; }

    /**
     * Get the empty_adj_list truth value
     * @return 2 if unassigned, 1 if list contains either nothing or an empty vertex, 0 if no empty vertex
     */
    int get_empty_bool() { return mEmptyAdjList; }

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
    void decrement_kmer_occurrences() { mKmerOccurrences--; }

    /**
     * Set the available adjacencies to be all adjacencies of vertex
     * Used in generating new genomes
     */
    void set_adj_availible() {
        std::copy ( mAdjList.begin(), mAdjList.end(), back_inserter(mAvailableAdj) );
    }
        
    /**
     * Get the value of an available adjacency for a new genome
     * @param index of adjacency
     * @return string kmer
     */
    string get_adj_availible(int index) { return mAvailableAdj[index]; }

    /**
     * Get the number of adjacencies that are still valid&available to append to a new genome
     * @return int 
     */
    int adj_availible_size() { return mAvailableAdj.size(); }

    /**
     * When we've used a kmer in a new genome as many times it appears in our sequences, we want to make it unavailable for further use
     * @param val kmer to remove from the list of available adjacencies
     * @param still_an_end if true, add the kmer to a list of kmers that have used up their availibility except for as an end
     */
    void remove_adj_availible(string val, bool still_an_end=0) { 
    mAvailableAdj.erase(std::remove(mAvailableAdj.begin(), mAvailableAdj.end(), val), mAvailableAdj.end());
    }

    /**
     * clear availible adjacencies
     */
    void clear_adj_availible() { mAvailableAdj.clear(); }

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

    // /**
    //  * Set maximum path length from this vertex
    //  */
    // void set_max_len(int l) { mMaxLen = l; }
    //   
    // /**
    //  * Get maximum path length from this vertex
    //  */
    // int get_max_len() { return mMaxLen; }
    //
    // /**
    //  * Set minimum path length from this vertex
    //  */
    // void set_min_len(int l) { mMinLen = l; }
    //
    // /**
    //  * Get minimum path length from this vertex
    //  */
    // int get_min_len() { return mMinLen; }
    //
    // /**
    //  * Set weight
    //  */
    // void set_weight(int w) { mWeight = w; }
    //  
    // /**
    //  * Get weight
    //  */
    // int get_weight() { return mWeight; }

    /**
     * Set maximum path length from this vertex
     */
    void set_max_len(int l, string s) { mMax = std::make_tuple(l, s); }
        
    /**
     * Get maximum path length from this vertex
     */
    tuple<int, string> get_max_len() { return mMax; }

    /**
     * Set minimum path length from this vertex
     */
    void set_min_len(int l, string s) { mMin = std::make_tuple(l, s); }
        
    /**
     * Get minimum path length from this vertex
     */
    tuple<int, string> get_min_len() { return mMin; }

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNVALUE_H
