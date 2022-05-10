/**
 * @file DeBruijnGraph.h
 * @author Anna Catenacci
 *
 * A class that constructs and describes a De Bruijn Graph
 * Notes to self: -create a display function that more clearly shows the links between vertices.
 * 
 * @note Takes a longer time to run after adding the lambda to the DFS traversal function!
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
    int mSize = 0;

    /// Map of Debruijn vertex objects to their values/data
    map<DeBruijnVertex, DBGraphValue> mVertices;

    /// Vector of all Vertices the map contains
    // (will this variable be necessary to keep around if I already have a flag attribute in DBValue?)
    vector<DeBruijnVertex> mBranchedVertices;

    /**
     * Create a sequence for the current constructor
     * @param num_input vector of numbers to create a graph from
     * @param kmer_length length of each vertex ID
     */
    void ConstructFromSequence(vector<int> num_input, int kmer_length){
        string input = "";
        for(int i = 0; i < int(num_input.size()); ++i){
            input += std::to_string(num_input[i]);
        }
        this->ConstructFromString(input, kmer_length);
    }

    /**
     * Construct nodes from a string of genetic information
     * @param input string containing all genetic data sequentially
     */
    void ConstructFromString(string input, int kmer_length){
        if(int(input.length()) == kmer_length){
            this->set_empty_vertex(DeBruijnVertex(input));
        }
        while(int(input.length()) >= kmer_length + 1){
            this->set_size( this->get_size() + 1 );
            this->add_edge(DeBruijnVertex(input.substr(0, kmer_length)),
                    DeBruijnVertex(input.substr(1, kmer_length)));
            this->set_empty_vertex(DeBruijnVertex(DeBruijnVertex(input.substr(1, kmer_length))));

            input = input.substr(1, input.length()-1);
        }
        this->set_size( this->get_size() + 1 );
    }

public:
///Should I disable the default constructor if we are just using this to construct an intial population?
///For now I think no, and I will keep this and add an add_vertex function
    DeBruijnGraph()=default;
    ~DeBruijnGraph()=default;

    /**
     * Construct a De Bruijn Graph object from a vector of strings
     * @param input a vector containing strings to convert to a graph
     * @param kmer_length length of indecies to pull from input to create a kmer ID
     */
    DeBruijnGraph(vector<string> input, int kmer_length){
        string result = "";
        for(int i = 0; i < int(input.size()); ++i){
            result += input[i];
        }
        this->ConstructFromString(result, kmer_length);
    }

    /**
     * Construct a De Bruijn Graph object from a vector of integers
     * @param input a vector containing ints to convert to a graph
     * @param kmer_length length of indecies to pull from input to create a kmer ID
     */
    DeBruijnGraph(vector<int> input, int kmer_length){
        this->ConstructFromSequence(input, kmer_length);
    }

    /**
     * Construct a De Bruijn Graph object from an integer
     * @param input a integer to convert to a graph
     * @param kmer_length length of indecies to pull from input to create a kmer ID
     */
    DeBruijnGraph(int input, int kmer_length){
        this->ConstructFromString(std::to_string(input), kmer_length);
    }

    /**
     * Construct a De Bruijn Graph object from an string
     * @param input a string to convert to a graph
     * @param kmer_length length of indecies to pull from input to create a kmer ID
     */
    DeBruijnGraph(string input, int kmer_length){
        this->ConstructFromString(input, kmer_length);
    }

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
        start_v.set_order(mSize);
        end_v.set_order(mSize + 1);
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

    template <typename FuncType>
    /**
     * Traversal function that currently prints each vertex ID
     * @todo Put in an If statement that checks whether the vertex branches
     */
    void depth_first_traversal(FuncType func){
        vector<DeBruijnVertex> traversing = this->get_all_vertices();
        for(int i = 0; i < mSize; ++i){
            auto iter = std::find(traversing.begin(), traversing.end(), traversing[i]);
             if(iter != traversing.end()){
                 //visit vertex and preform action
                 if(!mVertices[traversing[i]].get_visitor_flag()){
                     mVertices[traversing[i]].change_visitor_flag(true);
                     func(traversing[i]);
                 }

                 dfs_recursion(traversing[i], func);
             }
        }
    }

    template <typename FuncType>
    /**
     * Recursive function used in depth_first_traversal
     * @param vertex DeBruijn vertex with adjacencies to be traversed
     */
    void dfs_recursion(DeBruijnVertex vertex, FuncType func){
        vector<DeBruijnVertex> edges = mVertices[vertex].get_adj_list();
        for(int i = 0; i < int(edges.size()); ++i){
            //if flag is false, if vertex has not yet been visited
            if(!mVertices[edges[i]].get_visitor_flag()){
                mVertices[edges[i]].change_visitor_flag(true);
                func(edges[i]);
                dfs_recursion(edges[i], func);
            }
        }
    }

    /**
     * Set the size object
     * @param s size of graph
     */
    void set_size(int s) { mSize = s; }

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
        return all_vertices; 
    }

    /**
     * Return vector containing vertices with more than one adjacency in graph
     * @return vector containing branched DeBruijn vertex objects
     */
    vector<DeBruijnVertex> get_branch_vertices() { return mBranchedVertices; }

    bool vertex_branch_check(DeBruijnVertex vertex) { return mVertices[vertex].get_branch(); }

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNGRAPH_H
