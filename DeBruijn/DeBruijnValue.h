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

    //could make this a map of the adj to the number of sequences it's used in if it helps with remove_sequence
    /// Adjacency list--vector of all verticies this vertex points to
    vector<string> mAdjList = {};

    // so you'd to to the adj list being pointed to, and then check the index to be accessed, and is the size == 
    // the index +1, then you know this vertex is done
    int mVisits = 0;

    /// True if this vertex contains more than one adjacent vertex
    bool mContainsBranch = false;

    /// True if the adjacency list contains either nothing or an empty vertex
    ///2 if unassigned, 1 if list contains either nothing or an empty vertex, 0 if no empty vertex
    //bool mEmptyAdjList = false;
    int mEmptyAdjList = 2;

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

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNVALUE_H
