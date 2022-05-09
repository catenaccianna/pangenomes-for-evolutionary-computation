/**
 * @file DeBruijnGraph.h
 * @author Anna Catenacci
 *
 * A class that constructs and describes a De Bruijn Graph
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

    /// Number of vertices the graph contains
    int mSize;

    /// Map of Debruijn vertex objects to their values/data
    map<DeBruijnVertex, DBGraphValue> mVertices;

    /// Vector of all Vertices the map contains
    // (will this variable be necessary to keep around?)
    vector<DeBruijnVertex> mBranchedVertices;

public:
    DeBruijnGraph()=default;

    /**
     * Construct a new De Bruijn Graph object
     * @todo Make an int parameter that allows you to change the length of the string for a vertex ID
     * @param input a vector containing 3 character strings to convert to a graph
     */
    DeBruijnGraph(vector<string> input){

        // if size of input is one, add a vertex with no outgoing edges
        if(input.size() == 1){
            this->set_empty_vertex(DeBruijnVertex(input[0]));
        }

        // if size of input is greater than one, call add_to_graph to create an edge
        if(input.size() > 1) {
            for (int i = 0; i< int(input.size()) -1; ++i) {
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

    /**
     * Construct a new De Bruijn Graph object
     * @todo Do I really need this? Probably not
     * @param genetic_string an input of one long string of characters
     */
    DeBruijnGraph(string genetic_string){
        this->ConstructFromString(genetic_string);
    }

    ~DeBruijnGraph()=default;

    /**
     * Create a sequence for the current constructor
     * @todo optimization should look more like this function
     * @todo add functionality to be able to construct vertex IDs of various length
     * @param num_input vector of numbers to create a graph from
     * @param kmer_length length of each vertex ID
     */
    void ConstructFromSequence(vector<int> num_input, int kmer_length){
        string input = "";
        for(int i = 0; i < int(num_input.size()); ++i){
            input += std::to_string(num_input[i]);
        }
        this->ConstructFromString(input);
    }

    /**
     * Construct nodes from a string of genetic information
     * @todo Add functionality for more kmer ID length options (as a parameter)
     * @param input string containing all genetic data sequentially
     */
    void ConstructFromString(string input){
        mSize = 0;

        if(input.length() == 3){
            this->set_empty_vertex(DeBruijnVertex(input));
            this->set_size( this->get_size() + 1 );
        }
        while(input.length() >= 4){

            this->add_edge(DeBruijnVertex(input.substr(0, 3)),
                    DeBruijnVertex(input.substr(1, 3)));
            this->set_empty_vertex(DeBruijnVertex(DeBruijnVertex(input.substr(1, 3))));

            input = input.substr(1, input.length()-1);
            this->set_size( this->get_size() + 1 );
        }

        //this->set_size(input.length() / 3);
    }

    /**
     * Return size of graph
     * @return number of vertices the graph contains
     */
    int get_size() { return mSize; }

    /**
     * Return vector containing all vertices in graph
     * @return vector containing all DeBruijn vertex objects
     */
    vector<DeBruijnVertex> get_all_vertices() {
        vector<DeBruijnVertex> all_vertices;
        for (auto const& element : mVertices) {
            all_vertices.push_back(element.first);
        }
        return all_vertices; }

    /**
     * Set the size object
     * @param s size of graph
     */
    void set_size(int s) { mSize = s; }

    /**
     * Set a vertex with no value as a place-holder
     * @param v vertex object to add to the graph's list of vertices
     */
    void set_empty_vertex(DeBruijnVertex v){
        mVertices[v] = DBGraphValue();
    }

    /**
     * Add an edge between two vertices
     * @param start_v Starting Debruijn vertex
     * @param end_v Vertex being pointed to
     */
    void add_edge(DeBruijnVertex start_v, DeBruijnVertex end_v){
        start_v.set_start(mSize);
        end_v.set_start(mSize + 1);
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

    /**
     * Print all vertices on the map to disply them
     * @todo Would like to eventually use Julia to display the graph as a whole
     */
    void display(){
        for (auto const& element : mVertices) {
            cout << element.first.get_kmer() << "\n";
        }
    }

    /**
     * Reset all vertex flags to show they are Unvisited
     * To be used in traversals
     */
    void reset_vertex_flags() {
        vector<DeBruijnVertex> all_vertices;
        for (auto & element : mVertices) {
            element.second.change_visitor_flag(false);
        }
    }

    /**
     * Traversal function that currently prints each vertex ID
     * @todo Put in an If statement that checks whether the vertex branches
     */
    void depth_first_traversal(){
        vector<DeBruijnVertex> traversing = this->get_all_vertices();
        for(int i = 0; i < mSize; ++i){
            auto iter = std::find(traversing.begin(), traversing.end(), traversing[i]);
             if(iter != traversing.end()){
                 //visit vertex and preform action
                 if(!mVertices[traversing[i]].get_visitor_flag()){
                     mVertices[traversing[i]].change_visitor_flag(true);
                     cout << traversing[i].get_kmer() << "\n";
                 }

                 dfs_recursion(traversing[i]);
             }
        }
    }

    /**
     * Recursive function used in depth_first_traversal
     * @param vertex DeBruijn vertex with adjacencies to be traversed
     */
    void dfs_recursion(DeBruijnVertex vertex){
        vector<DeBruijnVertex> edges = mVertices[vertex].get_adj_list();
        for(int i = 0; i < int(edges.size()); ++i){
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
