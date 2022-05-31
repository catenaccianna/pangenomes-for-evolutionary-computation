/**
 * @file DeBruijnValue.h
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

using std::string; using std::vector;

class DBGraphValue {
private:

    /// Adjacency list--vector of all verticies this vertex points to
    vector<string> mAdjList = {};

    vector<string> mAvailableAdj;

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
    int mSequenceCount = 0;

public:
    /// Constructors
    DBGraphValue()=default;
    DBGraphValue(vector<string> a, bool c) : mAdjList(a), mContainsBranch(c) {};

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
     * 
     * @todo subtract 1 form size if there is one empty node in adj list
     */
    int adj_list_size(){ return mAdjList.size(); }

    /**
     * Add to adjacency list (creates edge)
     * @param addition vertex to add to this vertex's adjacency list
     */
    void add_to_adj_list(string addition){ 
        mAdjList.push_back(addition);
        vector<string>::iterator it;
        it = std::unique(mAdjList.begin(), mAdjList.end());
        mAdjList.resize( std::distance(mAdjList.begin(),it) ); }

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
     * Get the branch truth value
     * @return true if there is more than one value in the adjacency list
     */
    int get_empty_bool() { return mEmptyAdjList; }

    /**
     * Get the sequence count
     * @return number of sequences this kmer is  used in
     */
    int get_sequence_count() { return mSequenceCount; }

    /**
     * Increment the sequence count
     */
    void increment_sequence_count() { mSequenceCount++; }

    /**
     * Decrement the sequence count
     */
    void decrement_sequence_count() { mSequenceCount--; }

    void set_adj_availible() { mAvailableAdj = mAdjList; }
    string get_adj_availible(int index) { return mAvailableAdj[index]; }
    int adj_availible_size() { return mAvailableAdj.size(); }
    void remove_adj_availible(string val) { 
    mAvailableAdj.erase(std::remove(mAvailableAdj.begin(), mAvailableAdj.end(), val), mAvailableAdj.end()); }

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNVALUE_H
