/**
 * @file DBGraphValue.h
 * @author Anna Catenacci
 *
 * Class describing the attributes of a DeBruijnVertex object
 * Includes information about the vertex adjacency list, whether or
 * not the vertex branches, and whether it has been visited in traversal
 */

#ifndef PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DBGRAPHVALUE_H
#define PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DBGRAPHVALUE_H

#include <string>
#include <vector>
#include <algorithm>

using std::string; using std::vector;

class DBGraphValue {
private:

    /// Adjacency list--vector of all verticies this vertex points to
    vector<string> mAdjList = {};

    /// True if this vertex has been visited in traversal
    //bool mVisitedFlag = false;

    // so you'd to to the adj list being pointed to, and then check the index to be accessed, and is the size == 
    // the index +1, then you know this vertex is done
    int mVisits = 0;

    /// True if this vertex contains more than one adjacent vertex
    bool mContainsBranch = false;

    /// True if the adjacency list contains either nothing or an empty vertex
    bool mEmptyAdjList = true;

    /// True if this node is a beginning
    bool mStart = false;

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
     * Return size of adjacency list
     * @return size
     * 
     * @todo subtract 1 form size if there is one empty node in adj list
     */
    int adj_list_size(){ return mAdjList.size(); }

    /**
     * Get the visitor flag object
     * @return true if this vertex has already been visited in a traversal
     */
    //bool get_visitor_flag(){ return mVisitedFlag; }

    /**
     * Get the visitor flag object
     * @return true if this vertex has already been visited in a traversal
     */
    int get_visitor_flag(){ return mVisits; }

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
     * Set visitor flag
     * @param value true if visited, false if not
     */
    //void change_visitor_flag(bool value) { mVisitedFlag = value; }

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
    void set_empty_bool(bool value) { mEmptyAdjList = value; }

    /**
     * Get the branch truth value
     * @return true if there is more than one value in the adjacency list
     */
    bool get_empty_bool() { return mEmptyAdjList; }

    /**
     * Set the bool for whether is node is a beginning of the graph
     * @param value true if this node is a beginning
     */
    void set_beginning(bool value) { mStart = value; }

    /**
     * Get the starting truth value
     * @return true if this node is a beginning of the graph
     */
    bool get_beginning() { return mStart; }

    /**
     * Get the adjacent DeBruijn Vertex
     * @param index at which to pull the vertex from
     * @return string reresenting kmer adjacency 
     */
    string get_adjacency(int index) { return mAdjList[index]; }

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DBGRAPHVALUE_H
