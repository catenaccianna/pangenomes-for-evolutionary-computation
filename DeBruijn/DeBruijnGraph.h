/**
 * @file DeBruijnGraph.h
 * @author Anna Catenacci
 *
 * A class that constructs and describes a De Bruijn Graph
 * Notes to self: -create a display function that more clearly shows the links between vertices.
 * 
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

    /// Starting vertex so that we know which vertex to start traversing with
    DeBruijnVertex mStart;

    /// Vector of all Vertices the map contains
    // (will this variable be necessary to keep around if I already have a flag attribute in DBValue?)
    vector<DeBruijnVertex> mBranchedVertices;

    /**
     * Create a useable sequence from the input to construct the graph
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
     * Construct a graph from a string of genetic information
     * This function is the common constructor
     * @param input string containing all genetic data sequentially
     */
    void ConstructFromString(string input, int kmer_length){
        mStart = DeBruijnVertex(input.substr(0, kmer_length));
        //if the graph is one vertex long:
        if(int(input.length()) == kmer_length){
            this->set_empty_vertex(DeBruijnVertex(input));
        }
        //add to size and add an edge for each vertex, and an empty vertex for the end
        while(int(input.length()) >= kmer_length + 1){
            this->set_size( this->get_size() + 1 );
            this->add_edge(DeBruijnVertex(input.substr(0, kmer_length)),
                    DeBruijnVertex(input.substr(1, kmer_length)));
            this->set_empty_vertex(DeBruijnVertex(DeBruijnVertex(input.substr(1, kmer_length))));
            //take one character off the input, continue
            input = input.substr(1, input.length()-1);
        }
        this->set_size( this->get_size() + 1 );
    }

public:
///Should I disable the default constructor if we are just using this to construct an intial population?
///For now I think no, and I will keep this and add an add_vertex function if it becomes necessary
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
        mVertices[v];
        mVertices[v].set_empty_bool(true);
    }

    /**
     * Add an edge between two vertices
     * @param start_v Starting Debruijn vertex
     * @param end_v Vertex being pointed to
     */
    void add_edge(DeBruijnVertex start_v, DeBruijnVertex end_v){
        //if the adj_list is not empty, this implies the vertex is a branch point
        if(mVertices[start_v].adj_list_size() > 0){
            mVertices[start_v].set_branch(true);
            mBranchedVertices.push_back(start_v);
            std::unique(mBranchedVertices.begin(), mBranchedVertices.begin() + mBranchedVertices.size());
        }
        mVertices[start_v].set_empty_bool(false);
        mVertices[start_v].add_to_adj_list(end_v);
        //OLD start_v.set_order(mSize);
        //OLD end_v.set_order(mSize + 1);
       
        /* OLD
        // if the start k-mer is not already recorded in the graph, add a new edge
        if(mVertices.find(start_v) == mVertices.end()){
            mVertices[start_v].add_to_adj_list(end_v);
        }
        // if the start k-mer is already in the graph, add the end k-mer to the adjacency list
        else{
            mVertices[start_v].add_to_adj_list(end_v);
            //if the adj_list is not empty, this implies the vertex is a branch point
            //if(mVertices[start_v].adj_list_size() > 0){
            if(!mVertices[start_v].get_empty_bool()){
                mVertices[start_v].set_branch(true);
                mBranchedVertices.push_back(start_v);
            }
        }*/
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

    /** OLD
     * Reset all vertex flags to show they are Unvisited
     * To be used in traversals
     
    void reset_vertex_flags() {
        vector<DeBruijnVertex> all_vertices;
        for (auto & element : mVertices) {
            element.second.change_visitor_flag(false);
        }
    }
    */

    /**
     * Reset all vertex flags to show they are Unvisited
     * To be used in traversals
     */
    void reset_vertex_flags() {
        vector<DeBruijnVertex> all_vertices;
        for (auto & element : mVertices) {
            element.second.change_visitor_flag(0);
        }
    }

    template <typename FuncType>

    /**
     * Traversal function that currently prints each vertex ID
     * @param func lambda function to use when visiting the current vertex
     * @note Depending on what we're traversing for, like if we need to compare adjacent 
     *      verticies, can make a 2-vertex parameter on the lambda, or if we need the function
     *      to return something (or a template of something)
     */
    void depth_first_traversal(FuncType func){
        // edge case--this traversal did not work for size of 1 without it
        if(mSize == 1){
            func(mStart);
        }
        DeBruijnVertex current = mStart;
        dfs_recursion(mStart, func);
        this->reset_vertex_flags();
    }

    template <typename FuncType>

    /**
     * Recursive function used in depth_first_traversal
     * @param vertex DeBruijn vertex with adjacencies to be traversed
     * @param func lambda function to use when visiting the current vertex
     */
    void dfs_recursion(DeBruijnVertex vertex, FuncType func){
        // if the number of times we've visited this vertex's adj_list has not covered all adjacencies:
        if(mVertices[vertex].get_visitor_flag() < int(mVertices[vertex].adj_list_size())){
            // append to count of times we've visited this adj_list
            mVertices[vertex].change_visitor_flag(mVertices[vertex].get_visitor_flag()+1);
            func(vertex);
            // next vertex to visit will be at index based on number of times we've visited the adj_list
            // i.e. if we have 2 adjacencies, we will first visit index 0, then next time visit index 1
            // this works because the vector containing adjacencies is appended to in order of the sequence
            int next_index = mVertices[vertex].get_visitor_flag() - 1;
            DeBruijnVertex next_vertex = mVertices[vertex].get_adj_list()[next_index];
            if( !mVertices[next_vertex].get_empty_bool() ){
                // recur
                dfs_recursion(next_vertex, func);
            }
            // if empty_bool is true, we've reached the end vertex--its adj_list contains an empty vertex
            else{
                mVertices[next_vertex].change_visitor_flag(mVertices[vertex].get_visitor_flag()+1);
                func(next_vertex);
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

    /**
     * Given a vertex, retrun true if the vertex branches
     * @param vertex to check
     * @return true if the vertex has more than 2 verticies in it's adjacency list
     */
    bool vertex_branch_check(DeBruijnVertex vertex) { return mVertices[vertex].get_branch(); }

    /**
     * Get the value associated with a vertex
     * @return Debruijn vertex value object
     */
    DBGraphValue get_value(DeBruijnVertex vertex) { return mVertices[vertex]; }

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNGRAPH_H
