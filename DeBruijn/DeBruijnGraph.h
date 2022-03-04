/**
 * @file DeBruijnGraph.h
 * @author Anna Catenacci
 *
 *A class that describes a De Bruijn Graph
 * Notes to self: -change constructors & private variables to be able to accept ints
 *              -create a display function that more clearly shows the links
 *              between vertices.
 */

#ifndef PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNGRAPH_H
#define PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNGRAPH_H

#include "DeBruijnVertex.h"
#include "DBGraphValue.h"
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iostream>

using std::string; using std::vector; using std::map;
using std::cout;

class DeBruijnGraph {
private:
    int mSize;
    map<DeBruijnVertex, DBGraphValue> mVertices;
    vector<DeBruijnVertex> mBranchedVertices;

public:
    DeBruijnGraph()=default;

    ///constructor for an input of a vector containing 3 character strings
    DeBruijnGraph(vector<string> input){

        // if size of input is one, add a vertex with no outgoing edges
        if(input.size() == 1){
            this->set_empty_vertex(DeBruijnVertex(input[0]));
        }

        // if size of input is greater than one, call add_to_graph to create an edge
        if(input.size() > 1) {
            for (int i = 0; i<input.size()-1; ++i) {
                this->add_edge(DeBruijnVertex(input[i]),
                        DeBruijnVertex(input[i+1]));
                this->set_empty_vertex(DeBruijnVertex(input[i+1]));
            }
        }

        this->set_size(input.size());
    }

    ///a second constructor that I'm adding after looking at the test!
    DeBruijnGraph(vector<int> num_input){
        this->ConstructFromSequence(num_input, 3);
    }

    ///constructor for an input of one long string of characters
    DeBruijnGraph(string genetic_string){
        this->ConstructFromString(genetic_string);
    }

    ~DeBruijnGraph()=default;

    ///construct a graph from an input sequence
    void ConstructFromSequence(vector<int> num_input, int kmer_length){
        string input = "";
        for(int i = 0; i < num_input.size(); ++i){
            input += std::to_string(num_input[i]);
        }
        this->ConstructFromString(input);
    }

    ///construct from string
    void ConstructFromString(string input){
        if(input.length() == 3){
            this->set_empty_vertex(DeBruijnVertex(input));
        }
        while(input.length() >= 4){

            this->add_edge(DeBruijnVertex(input.substr(0, 3)),
                    DeBruijnVertex(input.substr(1, 3)));
            this->set_empty_vertex(DeBruijnVertex(DeBruijnVertex(input.substr(1, 3))));

            input = input.substr(1, input.length()-1);
        }

        this->set_size(input.length() / 3);
    }

    ///return size of graph
    int get_size() { return mSize; }

    ///return vector containing all vertices in graph
    vector<DeBruijnVertex> get_all_vertices() {
        vector<DeBruijnVertex> all_vertices;
        for (auto const& element : mVertices) {
            all_vertices.push_back(element.first);
        }
        return all_vertices; }

    ///set graph size
    void set_size(int s) { mSize = s; }

    ///set a vertex with no value
    ///in the the case that the vertex is at the end of the input
    void set_empty_vertex(DeBruijnVertex v){
        mVertices[v] = DBGraphValue();
    }

    ///add an edge between two DBG Vertices
    void add_edge(DeBruijnVertex start_v, DeBruijnVertex end_v){

        // if the start k-mer is not already recorded in the graph, add a new edge
        if(mVertices.find(start_v) == mVertices.end()){
            mVertices[start_v].add_to_adj_list(end_v);
        }

            // if the start k-mer is already in the graph, add the end k-mer to the adjacency list
        else{
            mVertices[start_v].add_to_adj_list(end_v);
            //if the adj_list is not empty, this implies the vertex is a branch point
            if(mVertices[start_v].adj_list_size() > 0){
                mVertices[start_v].set_branch(true);
                mBranchedVertices.push_back(start_v);
            }
        }
    }

    ///to print out all vertices in graph
    ///can perhaps modify this to show the vertices that branch later
    void display(){
        for (auto const& element : mVertices) {
            cout << element.first.get_kmer() << "\n";
        }
    }

    ///reset visitor flags for after a traversal
    void reset_vertex_flags() {
        vector<DeBruijnVertex> all_vertices;
        for (auto & element : mVertices) {
            element.second.change_visitor_flag(false);
        }
    }

    ///initial traversal function. can put in an if statement checking for
    ///branching vertices later!
    void depth_first_traversal(){
        vector<DeBruijnVertex> traversing = this->get_all_vertices();
        for(int i = 0; i < mSize; ++i){
            auto iter = std::find(traversing.begin(), traversing.end(), traversing[i]);
            if(iter != traversing.end()){
                //visit vertex and preform action
                mVertices[traversing[i]].change_visitor_flag(true);
                cout << traversing[i].get_kmer() << "\n";

                dfs_recursion(traversing[i]);
            }
        }
    }

    void dfs_recursion(DeBruijnVertex vertex){
        vector<DeBruijnVertex> edges = mVertices[vertex].get_adj_list();
        for(int i = 0; i < edges.size(); ++i){
            //if flag is false, if vertex has not yet been visited
            if(!mVertices[edges[i]].get_visitor_flag()){
                mVertices[edges[i]].change_visitor_flag(true);
                cout << edges[i].get_kmer() << "\n";

                dfs_recursion(edges[i]);
            }
        }
    }

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNGRAPH_H
