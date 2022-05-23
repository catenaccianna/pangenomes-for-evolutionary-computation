/**
 * @file DeBruijnGraph.h
 * @author Anna Catenacci
 *
 * A class that constructs and describes a De Bruijn Graph
 * 
 * @todo (optional) create a display function that more clearly shows the links between vertices.
 * @todo agenda is AddSequence function > AddSequenceTest > TestMultipleEnds > take out some unnecessary stuff
 * @todo also test AddSequence in relevant tests (like unique verticies test)
 * @todo instead of 4 functions for constructors and add_sequence, have 2 with templates?
 * 
 * @todo !!! test for the case where it's an endpoint and also still has the adj_list !!!
 */

#ifndef PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNGRAPH_H
#define PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNGRAPH_H

#include "DeBruijnValue.h"
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

    /// Number of sequences the graph contains
    int mNumSequences = 0;

    /// Length of the k-mer IDs
    int mKmerLength = 3;

    /// Length of sequences (number of bits in BitsOrg)
    int mSequenceLength;

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
        mSequenceLength = input.size();
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
            //change the set_empty_bool here so we don't run into endpoint troubles later.
            mVertices[input.substr(0, kmer_length)].set_empty_bool(0);
            this->set_empty_vertex(input.substr(1, kmer_length));
            //take one character off the input, continue
            input = input.substr(1, input.length()-1);
        }
        mVertices[input].set_empty_bool(1);
        this->set_size( this->get_size() + 1 );
        mNumSequences++;
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
        mVertices[start_v].add_to_adj_list(end_v);
        
    }

    /**
     * Print all vertices on the map to disply them
     * @todo Would like to eventually use Julia to display the graph as a whole
     */
    void display(){
        this -> depth_first_traversal( [&] (string vertex) { 
            cout<<vertex;
            // if there is one, non-empty vertex in the list, print it
            if (mVertices[vertex].get_empty_bool()==0 && mVertices[vertex].adj_list_size() == 1){
                cout<<" -> "<<mVertices[vertex].get_adj_list()[0];
            }
            // if the adj_list has more than one node in it, print them
            else if (mVertices[vertex].adj_list_size() >= 1){
                cout<<" -> ";
                for(auto i: mVertices[vertex].get_adj_list()){
                    cout<<i<<", ";
                }
            }
            // if the adj_list contains an endpoint/empty vertex, show that
            if (mVertices[vertex].get_empty_bool()==1){
                cout << "(an endpoint)";
            }
            cout<<"\n";
            });
    }

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
            while(path.size() > 0){
                current = path.back();
                path.pop_back();
                // if the vertex has been visited fewer times than it appears in the graph, continue:
                if(mVertices[current].get_visitor_flag() <= int(mVertices[current].adj_list_size())){
                    func(current);
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
        }
        this->reset_vertex_flags();
    }
    
    /**
     * Recursive function to traverse through a single path in the graph, with branches chosen at random
     * To be used in selecting the genetic information for the next generation organism
     * @param seed random seed to be used when choosing a branching path
     * @param organism whose genome we are modifying--here I am just going to insert a 3-char string to represent the start
     */
    string next_genome_logic(unsigned int seed, string organism){
        string path = organism;
        string current = organism;
        // this will work while all sequences are the same length (looks like this is the case in MABE)
        while (int(path.size()) < mSequenceLength){
            // generate index using the empirical random library when we have empirical hooked up
            current = mVertices[current].get_adj_list()[(mVertices[current].adj_list_size()-1)%seed];
            path+= current.substr(2,1);
        }
        return path;
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
     * @param sequence to add to the graph
     */
    void add_sequence(int sequence){
        this->add_sequence(std::to_string(sequence));
    }

    /**
     * Add an entirely new possible sequence into the graph
     * @param sequence to add to the graph
     */
    void add_sequence(vector<int> sequence){
        string input = "";
        for(int i = 0; i < mKmerLength; ++i){
            input += std::to_string(sequence[i]);
        }
        this->add_sequence(input);
    }

    /**
     * Add an entirely new possible sequence into the graph
     * @param sequence to add to the graph
     */
    void add_sequence(vector<string> sequence){
        string input = "";
        for(int i = 0; i < mKmerLength; ++i){
            input += sequence[i];
        }
        this->add_sequence(input);
    }

    /**
     * Add an entirely new possible sequence into the graph
     * Will take in a sequence much like the constructors do, but in this case, they will add a new node or a new path based on what's
     * different from the original path, and branch out from there.
     * (this will solve the multiple paths issue)
     * In actual expirimentation, we will choose a "fitter" section of the landscape to pull these combinations from, and then
     * traverse through and randomly do crossovers when we hit a branch
     * @param sequence to add to the graph
     */
    void add_sequence(string sequence){
        mNumSequences++;
        mSequenceLength = sequence.size();
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
            mVertices[sequence.substr(0, mKmerLength)].set_empty_bool(0);
            //if future vertex is not already in map, set it as an empty vertex
            if(mVertices.count(sequence.substr(1, mKmerLength)) <= 0){
                this->set_empty_vertex(sequence.substr(1, mKmerLength));
            }
            //turn empty bool true for all but last guy. However, check, because if there is a 
            //sequence already in here, then the flag will be off for that node already and we
            //do not want to change it
            sequence = sequence.substr(1, sequence.length()-1);
        }
        mVertices[sequence].set_empty_bool(1);
    }

    /**
     * Remove a sequence from the graph
     * To be used in BeforeDeath in MABE
     * @param sequence to remove
     */
    void remove_sequence(string sequence){
        mNumSequences--;
        string current = sequence.substr(0,mKmerLength);
        while(int(sequence.size()) > mKmerLength){
            //get/set adjlist to remove this element from it if it's not used for a duplicate string
            //remove element from mVerticies if it's not used for a duplicate string
            sequence = sequence.substr(1, sequence.length()-1);
        }
    }

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNGRAPH_H
