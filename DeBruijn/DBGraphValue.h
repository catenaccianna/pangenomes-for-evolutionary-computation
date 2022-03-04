/**
 * @file DBGraphValue.h
 * @author Anna Catenacci
 *
 *
 */

#ifndef PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DBGRAPHVALUE_H
#define PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DBGRAPHVALUE_H

#include "DeBruijnVertex.h"

class DBGraphValue {
private:
    vector<DeBruijnVertex> mAdjList = {};
    bool mVisitedFlag = false;
    bool mContainsBranch = false;

public:
    DBGraphValue()=default;
    DBGraphValue(vector<DeBruijnVertex> a, bool v, bool c) : mAdjList(a), mVisitedFlag(v),
                                                             mContainsBranch(c) {};
    ~DBGraphValue()=default;

    ///return vector of all vertices in adjacency list
    vector<DeBruijnVertex> get_adj_list() const{
        return mAdjList;
    }

    ///return size of adjacency list
    int adj_list_size(){
        return mAdjList.size();
    }

    ///return true if this vertex has already been visited in a traversal
    bool get_visitor_flag(){
        return mVisitedFlag;
    }

    ///add to adjacency list (creates edge)
    void add_to_adj_list(DeBruijnVertex& addition){
        mAdjList.push_back(addition);
    }

    ///set visitor flag
    ///true if visited, false if not
    void change_visitor_flag(bool value) {
        mVisitedFlag = value;
    }

    ///set branching flag
    ///true if the vertex has more than one value in it's adjacency list
    ///this implies the vertex contains a branch point
    void set_branch(bool value) {
        mContainsBranch = value;
    }

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DBGRAPHVALUE_H
