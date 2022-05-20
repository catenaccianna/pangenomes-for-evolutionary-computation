/**
 * @file DeBruijnGraph.h
 * @author Anna Catenacci
 *
 * A class that constructs and describes a De Bruijn Graph
 * Notes to self: -create a display function that more clearly shows the links between vertices.
 * 
 * so agenda is AddSequence function > AddSequenceTest > TestMultipleEnds > take out some unnecessary stuff
 * also test AddSequence in relevant tests (like unique verticies test)
 * for MABE, go through github ssh keys setup, and set one up so you can clone MABE2 without error, then follow through
 */

#ifndef PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNGRAPH_H
#define PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNGRAPH_H

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

    /// Length of the k-mer IDs
    int mKmerLength = 3;

    /// Map of Debruijn vertex objects to their values/data
    map<string, DBGraphValue> mVertices;

    /// Starting vertex so that we know which vertex to start traversing with
    string mStart;

    /// Vector of all Vertices the map contains
    // (will this variable be necessary to keep around if I already have a flag attribute in DBValue?)
    vector<string> mBranchedVertices;

    /// Vector of all ending verticies
    vector<string> mEnds;

    /// Vector of all beginning verticies
    vector<string> mStarts;

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
        mKmerLength = kmer_length;
        mStart = input.substr(0, kmer_length);
        mStarts.push_back(input.substr(0, kmer_length));
        //if the graph is one vertex long:
        if(int(input.length()) == kmer_length){
            this->set_empty_vertex(input);
        }
        //add to size and add an edge for each vertex, and an empty vertex for the end
        while(int(input.length()) >= kmer_length + 1){
            this->set_size( this->get_size() + 1 );
            this->add_edge(input.substr(0, kmer_length), input.substr(1, kmer_length));
            this->set_empty_vertex(input.substr(1, kmer_length));
            //take one character off the input, continue
            input = input.substr(1, input.length()-1);
        }
        this->set_size( this->get_size() + 1 );
    }

public:
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
    void set_empty_vertex(string v){
        mVertices[v];
        mVertices[v].set_empty_bool(true);
    }

    /**
     * Add an edge between two vertices
     * @param start_v Starting Debruijn vertex
     * @param end_v Vertex being pointed to
     */
    void add_edge(string start_v, string end_v){
        //if the adj_list is not empty, this implies the vertex is a branch point
        if(mVertices[start_v].adj_list_size() > 0){
            mVertices[start_v].set_branch(true);
            mBranchedVertices.push_back(start_v);
            vector<string>::iterator it;
            it = std::unique(mBranchedVertices.begin(), mBranchedVertices.end());
            mBranchedVertices.resize( std::distance(mBranchedVertices.begin(),it) );
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
        this -> depth_first_traversal( [&] (string vertex) { 
            if (!mVertices[vertex].get_empty_bool() && mVertices[vertex].adj_list_size() == 1){
                cout<<vertex<<" -> "<<mVertices[vertex].get_adj_list()[0];
            }
            else if (mVertices[vertex].adj_list_size() >= 1){
                cout<<vertex<<" -> ";
                for(auto i: mVertices[vertex].get_adj_list()){
                    cout<<i<<", ";
                }
            }
            if (mVertices[vertex].get_empty_bool()){
                cout << vertex << " (an endpoint)";
            }
            //else{ cout << vertex << "\n"; } } );
            /*if (mVertices[vertex].get_empty_bool()){
                cout << vertex << " (an endpoint)";
            }
            else{
                cout<<vertex<<" -> ";
                for(auto i: mVertices[vertex].get_adj_list()){
                    cout<<i<<", ";
                }*/
            cout<<"\n";
            });
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
        vector<string> all_vertices;
        for (auto & element : mVertices) {
            element.second.change_visitor_flag(0);
        }
    }

    template <typename FuncType>

///@todo have emily look at the general setup of this function, because I'm not sure if time and space complexity are just too much (probably are honestly)
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
        else{
            // because this is a directed graph, I want to make sure each path start is covered
            // therefore, I will put all the beginnings into my queue to start traversal
            vector<string> path = mStarts;
            string current = "";
            /*dfs_recursion(mStart, path, func);*/
            while(path.size() > 0){
                current = path.back();/*
                for (auto i:path){
                    cout<<i<<" ";
                }
                cout << "\n";*/
                path.pop_back();/*
                for (auto i:path){
                    cout<<i<<" ";
                }
                cout << "\n";*/
                // if the vertex has been visited fewer times than it appears in the graph, continue:
                if(mVertices[current].get_visitor_flag() <= int(mVertices[current].adj_list_size())){
                    /*
                    mVertices[current].change_visitor_flag(mVertices[current].get_visitor_flag()+1);*/
                    func(current);/*
                    for(int i = mVertices[current].adj_list_size() - mVertices[current].get_visitor_flag(); i > 0; i--){*/
                    // if this is the first time the vertex is being visited, we need to add it's adj_list into the queue
                    // otherwise, the adjacencies are already in there somewhere, so not needed
                    if(mVertices[current].get_visitor_flag() < 1){
                        for(int i = mVertices[current].adj_list_size(); i > 0; i--){
                            path.push_back(mVertices[current].get_adj_list()[i-1]);
                        }
                    }
                    mVertices[current].change_visitor_flag(mVertices[current].get_visitor_flag()+1);
                }   
            } 
            /*
            for(string i : mStarts){
                dfs_recursion(i, func);
            }*/
        }
        this->reset_vertex_flags();
    }


    template <typename FuncType>

    /**
     * Recursive function used in depth_first_traversal
     * @param vertex DeBruijn vertex with adjacencies to be traversed
     * @param func lambda function to use when visiting the current vertex
     */
    void dfs_recursion(string vertex, FuncType func){
        ////////////string current = path.pop();
        // if the number of times we've visited this vertex's adj_list has not covered all adjacencies:
        if(mVertices[vertex].get_visitor_flag() <= int(mVertices[vertex].adj_list_size())){
            // append to count of times we've visited this adj_list
            mVertices[vertex].change_visitor_flag(mVertices[vertex].get_visitor_flag()+1);
            func(vertex);
            // next vertex to visit will be at index based on number of times we've visited the adj_list
            // i.e. if we have 2 adjacencies, we will first visit index 0, then next time visit index 1
            // this works because the vector containing adjacencies is appended to in order of the sequence

            //is this breadth first instead of depth first?
            int next_index = mVertices[vertex].get_visitor_flag() - 1;
            string next_vertex = mVertices[vertex].get_adj_list()[next_index];
            //if( !mVertices[next_vertex].get_empty_bool() ){
            if(!mVertices[next_vertex].get_empty_bool()){
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
    vector<string> get_all_vertices() {
        vector<string> all_vertices;
        for (auto const& element : mVertices) {
            all_vertices.push_back(element.first);
        }
        return all_vertices; 
    }

    /**
     * Return vector containing vertices with more than one adjacency in graph
     * @return vector containing branched DeBruijn vertex objects
     */
    vector<string> get_branch_vertices() { return mBranchedVertices; }

    /**
     * Given a vertex, retrun true if the vertex branches
     * @param vertex to check
     * @return true if the vertex has more than 2 verticies in it's adjacency list
     */
    bool vertex_branch_check(string vertex) { return mVertices[vertex].get_branch(); }

    /**
     * Get the value associated with a vertex
     * @return Debruijn vertex value object
     */
    DBGraphValue get_value(string vertex) { return mVertices[vertex]; }

    /**
     * Add an entirely new possible sequence into the graph
     * Will take in a sequence much like the constructors do, but in this case, they will add a new node or a new path based on what's
     * different from the original path, and branch out from there.
     * (this will solve the multiple paths issue)
     * In actual expirimentation, we will choose a "fitter" section of the landscape to pull these combinations from, and then
     * traverse through and randomly do crossovers when we hit a branch
     * 
     */
    void add_sequence(int sequence){
        this->add_sequence(std::to_string(sequence));
    }

    void add_sequence(vector<int> sequence){
        string input = "";
        for(int i = 0; i < mKmerLength; ++i){
            input += std::to_string(sequence[i]);
        }
        this->add_sequence(input);
    }

    /* CASES:
    1) sequence overlaps in the middle (different start & end)
    2) sequence overlaps at beginning with same start
    3) sequence overlaps at beginning of new string, or vice versa (need new start in first case)
    4) sequence overlaps at end of new string, or vice versa
    5) pretty much lots of cases, so maybe start basic
    */
    void add_sequence(string sequence){
        // figure out how to adjust mStart here
        //string potential_mStart = sequence.substr(0, mKmerLength);

        // if the beginning string is not in the graph, add a new beginning vertex
        // maybe adjust the DB value for beginning if we keep that
        if(mVertices.count(sequence.substr(0, mKmerLength)) <= 0){
            mStarts.push_back(sequence.substr(0, mKmerLength));
        }

        // go through the entire new sequence and add edges:
        while(int(sequence.length()) >= mKmerLength+1){
            //if vertex is not already in the map, add to size
            if(mVertices.count(sequence.substr(0, mKmerLength)) <= 0){
                this->set_size( this->get_size() + 1 );
            }
            //add edge
            this->add_edge(sequence.substr(0, mKmerLength), sequence.substr(1, mKmerLength));
            //if future vertex is not already in map, set it as an empty vertex
            if(mVertices.count(sequence.substr(1, mKmerLength)) <= 0){
                this->set_empty_vertex(sequence.substr(1, mKmerLength));
            }
            sequence = sequence.substr(1, sequence.length()-1);
        }
/*
        void add_edge(string start_v, string end_v){
        //if the adj_list is not empty, this implies the vertex is a branch point
        if(mVertices[start_v].adj_list_size() > 0){
            mVertices[start_v].set_branch(true);
            mBranchedVertices.push_back(start_v);
            vector<string>::iterator it;
            //it = std::unique_copy (mBranchedVertices.begin(), mBranchedVertices.end(), mBranchedVertices.begin(), 
            //    [](string i, string j){return (i==j);});
            it = std::unique(mBranchedVertices.begin(), mBranchedVertices.end());
            mBranchedVertices.resize( std::distance(mBranchedVertices.begin(),it) );
        }
        mVertices[start_v].set_empty_bool(false);
        mVertices[start_v].add_to_adj_list(end_v);*/
    }

    void add_sequence(vector<string> sequence){
        string input = "";
        for(int i = 0; i < mKmerLength; ++i){
            input += sequence[i];
        }
        this->add_sequence(input);
    }

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNGRAPH_H
