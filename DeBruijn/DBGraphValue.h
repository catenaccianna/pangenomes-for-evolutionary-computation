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

#include "DeBruijnVertex.h"

class DBGraphValue {
private:

    /// Adjacency list--vector of all verticies this vertex points to
    vector<DeBruijnVertex> mAdjList = {};

    /// True if this vertex has been visited in traversal
    bool mVisitedFlag = false;

    /// True if this vertex contains more than one adjacent vertex
    bool mContainsBranch = false;

public:
    /// Constructors
    DBGraphValue()=default;
    DBGraphValue(vector<DeBruijnVertex> a, bool v, bool c) : mAdjList(a), mVisitedFlag(v),
                                                             mContainsBranch(c) {};

    /// Destructor                                                        
    ~DBGraphValue()=default;

    /**
     * Get the adj list object
     * @return vector of adjacent verticies
     */
    vector<DeBruijnVertex> get_adj_list() const{
        return mAdjList;
    }

    /**
     * Return size of adjacency list
     * @return size
     */
    int adj_list_size(){
        return mAdjList.size();
    }

    /**
     * Get the visitor flag object
     * @return true if this vertex has already been visited in a traversal
     */
    bool get_visitor_flag(){
        return mVisitedFlag;
    }

    /**
     * Add to adjacency list (creates edge)
     * @param addition vertex to add to this vertex's adjacency list
     */
    void add_to_adj_list(DeBruijnVertex& addition){
        mAdjList.push_back(addition);
    }

    /**
     * Set visitor flag
     * @param value true if visited, false if not
     */
    void change_visitor_flag(bool value) {
        mVisitedFlag = value;
    }

    /**
     * @brief Set the branch objectset branching flag
     * A true value implies the vertex contains a branch point.
     * @param value true if the vertex has more than one value in it's adjacency list
     */
    void set_branch(bool value) {
        mContainsBranch = value;
    }

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DBGRAPHVALUE_H
