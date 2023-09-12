/**
 * @file DeBruijnGraph.hpp
 * @author Anna Catenacci
 *
 * A class that constructs and describes a DeBruijn Graph
 */

#ifndef PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNGRAPH_H
#define PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNGRAPH_H

#include "../../../mabe/MABE2/source/third-party/empirical/include/emp/math/Random.hpp"

#include "DeBruijnValue.hpp"
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <functional>
#include <tuple>
#include <queue>

using std::string; using std::vector; using std::map;
using std::cout; using std::endl; using std::tuple;

class DeBruijnGraph {
public:
    using fun_t = void(std::ostream &);
    using fun_f = void(std::ofstream &);
private:

    /// Number of vertices the graph contains
    int mSize = 0;

    /// Number of sequences added to graph in total
    int mSeqSize = 0;

    /// Length of the k-mer IDs
    int mKmerLength = 3;

    /// Length of sequences (number of bits in BitsOrg)
    int mSequenceLength = 0;

    /// Map of Debruijn vertex objects to their values/data
    map<string, DBGraphValue> mVertices;

    /// Vector of all Vertices the map contains
    // (will this variable be necessary to keep around if I already have a flag attribute in DBValue?)
    vector<string> mBranchedVertices;

    /// Vector of all beginning verticies
    vector<string> mStarts;

    std::ofstream file;


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
        int initial_adj_size = mVertices[start_v].adj_list_size();
        mVertices[start_v].add_to_adj_list(end_v);
        if(initial_adj_size > 0){ //if the adj_list was not empty, AND new adj_list size > old_adj_list.size, then
            if(initial_adj_size < mVertices[start_v].adj_list_size()){ //this implies the vertex is a branch point
                mVertices[start_v].set_branch(true);
                mBranchedVertices.push_back(start_v);
                vector<string>::iterator it;
                mBranchedVertices.erase( std::unique( mBranchedVertices.begin(), mBranchedVertices.end() ), mBranchedVertices.end() );
            }
        }        
    }

    /**
     * Create a useable sequence from the input to construct the graph
     * @param num_input vector of numbers to create a graph from
     * @param kmer_length length of each vertex ID
     */
    void construct_from_sequence(vector<int> num_input, int kmer_length){
        string input = "";
        for(int i = 0; i < int(num_input.size()); ++i){
            input += std::to_string(num_input[i]);
        }
        construct_from_string(input, kmer_length);
    }

    /**
     * Construct a graph from a string of genetic information
     * This function is the common constructor
     * @param input string containing all genetic data sequentially
     */
    void construct_from_string(string input, int kmer_length){
        mSeqSize ++;
        mSequenceLength = input.size();
        mKmerLength = kmer_length;
        mStarts.push_back(input.substr(0, kmer_length));
        //if the graph is one vertex long:
        if(int(input.length()) == kmer_length){
            set_empty_vertex(input);
        }
        // if the vertex is already in the graph, then skip this and don't add to size
        if(mVertices.count(input.substr(0, mKmerLength)) <= 0){
            mSize++;
            set_empty_vertex(input.substr(0, mKmerLength));
        }
        //add to size and add an edge for each vertex, and an empty vertex for the end
        while(int(input.length()) >= kmer_length + 1){
            if(mVertices.count(input.substr(1, mKmerLength)) <= 0){
                mSize++;
            }
            add_edge(input.substr(0, kmer_length), input.substr(1, kmer_length));
            //change the set_empty_bool here so we don't run into endpoint troubles later.
            mVertices[input.substr(0, kmer_length)].set_empty_bool(0);
            set_empty_vertex(input.substr(1, kmer_length));
            //take one character off the input, continue
            input = input.substr(1, input.length()-1);
        }
        mVertices[input].set_empty_bool(1);
        mVertices[input].increment_endpoint();
        mVertices[input].increment_kmer_occurrences();
        update_loops();
    }

public:
    DeBruijnGraph()=default;
    ~DeBruijnGraph()=default;
    DeBruijnGraph(const DeBruijnGraph &) = default;

///@remark CONSTRUCTORS /////////////////////////////////////////////////////////////

    DeBruijnGraph(int kmer_length) : mKmerLength(kmer_length) {  };

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
        construct_from_string(result, kmer_length);
    }

    /**
     * Construct a De Bruijn Graph object from a vector of integers
     * @param input a vector containing ints to convert to a graph
     * @param kmer_length length of indecies to pull from input to create a kmer ID
     */
    DeBruijnGraph(vector<int> input, int kmer_length){
        construct_from_sequence(input, kmer_length);
    }

    /**
     * Construct a De Bruijn Graph object from an integer
     * @param input a integer to convert to a graph
     * @param kmer_length length of indecies to pull from input to create a kmer ID
     */
    DeBruijnGraph(int input, int kmer_length){
        construct_from_string(std::to_string(input), kmer_length);
    }

    /**
     * Construct a De Bruijn Graph object from an string
     * @param input a string to convert to a graph
     * @param kmer_length length of indecies to pull from input to create a kmer ID
     */
    DeBruijnGraph(string input, int kmer_length){
        construct_from_string(input, kmer_length);
    }

///@remark MABE FUNCTIONS (next genome, add sequence, remove sequence) /////////////////////////////////////////////////////////////

    /**
     * Recursive function to traverse through a single path in the graph, with branches chosen at random
     * To be used in selecting the genetic information for the next generation organism
     * @param seed random seed to be used when choosing a branching path
     * @param organism whose genome we are modifying--here I am just going to insert a 3-char string to represent the start
     */
    string next_genome_logic(emp::Random & random, string organism){
        string path = organism;
        string current = organism;
        // this will work while all sequences are the same length (looks like this is the case in MABE)
        while (int(path.size()) < mSequenceLength){
            // generate index using the empirical random library when we have empirical hooked up
            current = mVertices[current].get_adj_list()[random.GetUInt(mVertices[current].adj_list_size()-1)]; ///random seed here?!
            path+= current.substr(2,1);
        }
        return path;
    }

    /**
     * Given the genome of an organism, do crossover (if probability allows) by randomly choosing branches of existing 
     * genomes in the graph to pursue
     * @param random Empirical random number generator
     * @param organism whose genome we are modifying--here I am just going to insert a 3-char string to represent the start
     * @param probability that the modifcation will take place
     * @param seq_count "Sequence Counting" - if true, kmers are labeled unavailible if they have been used the same number 
     *                   of times that they have appeared in entire living genome.
     * @param variable_length false if the genome must be a fixed, standard length
     */
    string modify_org0(emp::Random & random, std::string organism, double probability = 1, bool seq_count = 1, bool variable_length = 0){ 
        string path = organism.substr(0, mKmerLength);          // initialize string variables we use to change and go down the path
        string current = organism.substr(0, mKmerLength);
        string next = "";
        mVertices[current].increment_visitor_flag(); // mark 1st kmer as visited
        int index;
        bool variable_length_end_reached = false;

        // If P() then we will modify this genome, else do nothing
        if( random.P( probability ) ) {
            while ( int(path.size()) < mSequenceLength) { // while our path hasn't reached the sequence length

                if(mVertices[current].get_visitor_flag() == 1) {         
                    mVertices[current].set_adj_availible(); // available choices = full adj_list if this is our first time seeing it
                }

                if(variable_length && mVertices[current].get_endpoint() > 0) { // if genome can be variable length and current kmer is an availible endpoint
                    index = random.GetUInt(mVertices[current].adj_availible_size() + 1);  // index will be randomly generated number
                    if( index == mVertices[current].adj_availible_size() ) { // if we have randomly chosen to keep this kmer as an endpoint
                        variable_length_end_reached = true;
                        break;
                    }
                }
                else { // if genome must be fixed length
                    index = random.GetUInt(mVertices[current].adj_availible_size());  // index will be randomly generated number
                }
                next = mVertices[current].get_adj_availible(index);                 // record next kmer using index
                path += next.substr(2,1);

                mVertices[next].increment_visitor_flag();  // mark next as visited
                if(seq_count){
                    if(mVertices[next].get_visitor_flag() == mVertices[current].get_kmer_occurrences()){
                        mVertices[current].remove_adj_availible(next);  // remove kmer from availible seq.s if it has been visited as many times
                    } // as it appears in all sequences in graph
                }
                current = next;
            }
            remove_sequence(organism);
            add_sequence(path);
            reset_vertex_flags();
            return path;
        }
        else {
            reset_vertex_flags();
            return organism;
        }
    }

    /**
     * @brief coding for one specific base case
     * Not using availible adj list. Just randomly select from adj_list. 
     * May try with weights, and because of that, will keep track of visitor flag.
     * 
     * @param random 
     * @param organism 
     * @param probability 
     * @param seq_count = 0
     * @param variable_length = 1
     * @return string 
     */
    string modify_org(emp::Random & random, std::string organism, double probability = 1, bool seq_count = 1, bool variable_length = 0){
        string path = organism.substr(0, mKmerLength);          // Initialize string variables we use to change and go down the path
        string current = organism.substr(0, mKmerLength);
        string next = "";
        mVertices[current].increment_visitor_flag();            // Mark 1st kmer as visited
        int index;

        if ( random.P ( probability ) ) {                         // If P() then we will modify this genome, else do nothing
            if ( variable_length ) {
                while ( true ) {
                    //if ( mVertices[current].get_endpoint() >= mVertices[current].get_kmer_occurrences() ) { // At a Dead End Node /// is this right??? should i be using like maybe visitor flag or something?
                    if ( mVertices[current].adj_list_size() < 1 ){
                        // pick ending kmer from current.adj's and append to path
                        //append endpoint i think 
                        break;
                    }

                    else if ( path.length() + std::get<0>(mVertices[current].get_max_len()) < organism.length() ) { //curr + max1 < parent and curr + max 2 >= parent choose 2 (and mirror)
                    //longest it can possibly be and it's still not long enough
                    }

                    else if ( path.length() + std::get<0>(mVertices[current].get_min_len()) > organism.length() ) { //curr + min 1 > parent and curr + min2 <= parent choose 2 (and mirror)
                    }

                    else if ( path.length() < organism.length() ) { //if current < parent len, (not long enough yet) take whichever fork has higher max
                    //also dont want min len + current len significantly > parent len
                    }

///@remark if we're looking at min and max length, that'll be recorded in the current node, but then we need some way of recording/selecting the path that actually matches that description. dict in value or edge class?

                    // if we are not in either of these cases, we may be theoretically capable of reaching a genome with a reasonable length, so choose a path randomly
                    // these two cases will only be gaurd rails
                    else { // if we are in this case, we have a chance at randomly choosing a genome of reasonable length
                        index = random.GetUInt ( mVertices[current].adj_list_size() + 1 );  // Choose random path down graph
                        next = mVertices[current].get_adjacency ( index );
                        path.append ( next.substr() );
                        // update current and next
                        //add kmer to graph, update visitor flag
                    }
                }
            }

            else { // if we are NOT USING VARIABLE LENGTH, could just do a loop until we reach length?
                    // feel like this might defeat the purpose of being able to have a genome that finds an intuitive end but good to have the option
            }
            
            remove_sequence(organism);                          // Remove old, unmodified sequence from graph
            add_sequence(path);                                 // Add new, modified sequence to graph
            reset_vertex_flags();
            return path;
        }
        else {                                                 // No modification
            reset_vertex_flags();
            return organism;
        }
    }

    /**
     * Using visitor flags, detect and mark all loops in the graph
     */
    void update_loops() {
        reset_vertex_flags();
        reset_loops();          // completely reset loop flags in case anything has changed
        loop_detection(mStarts[0]);
        reset_vertex_flags();
    }

    /**
     * Recursive function for loop detection in a cyclic graph
     * @param node to begin the recursion on
     */
    void loop_detection(string node) {
        depth_first_traversal( [&] (string node) {
            if (mVertices[node].get_visitor_flag()>0){
                mVertices[node].set_loop_flag(1); 
            }
            });
    }

    /**
     * Update minimum and maximum length attributes of each vertex within the graph
     */
    void update_weights() {
        update_loops();
        for(auto & vertex : mVertices) {
            if(vertex.second.get_endpoint() > 0) { // possible endpoint, min = 0
                vertex.second.set_min_len(0, vertex.first);
            }
            else {
                // min length of path is an endpoint vertex ahead of this point
                // maybe do a dfs/bfs search to find an endpoint, while keeping a count, 
                // dfs would probably have a better chance of keeping a count, 
                // but we'd have to set it back to 0 every once in a while on a new branch exploration
                // WAIT CAN WE USE DJIKSTRAS (pronounce dyke-stra's) ALGORITHM HERE AND SEARCH FOR SHORTEST PATH? 
                // for unweighted graphs, DA = bfs
                // idk because we'd need the shortest path to each endpoint and then compare them
                // could make a case where we set shortest and longest path this way (when we already have all dists) if both are needed i guess
                reset_vertex_flags();
                std::queue<std::string> node_q;
                vertex.second.increment_visitor_flag();
                node_q.push(vertex.first);
                int count = 0;
                while(!node_q.empty()) {
                    std::string current = node_q.front();
                    if ( mVertices[current].get_endpoint() > 0 ) {
                        count++;
                        vertex.second.set_min_len(count, current); /// @remark this might need to change because the string is going to be further down the line than the initial adj
                        break;
                    }
                    node_q.pop();
                    count++;
                    for ( auto & adj : mVertices[current].get_adj_list() ) {
                        if ( mVertices[adj].get_visitor_flag() == 0 ) {
                            mVertices[adj].increment_visitor_flag();
                            node_q.push(adj);
                        }
                    }
                }
                reset_vertex_flags();
            }
            if(vertex.second.get_loop_flag() == 1) { // possible loop, max = infinity
                vertex.second.set_max_len(std::numeric_limits<int>::max(), vertex.first); /// @remark this might need to change for the string to take to get to the loop
            }
            else {
                // so at this point, any mode we encounter on any path that has a loop is infinity
                // and otherwise we have the count numerical value
                reset_vertex_flags();
                std::queue<std::string> node_q;
                vertex.second.increment_visitor_flag();
                node_q.push(vertex.first);
                int count = 0;
                while(!node_q.empty()) {
                    std::string current = node_q.front();
                    if ( mVertices[current].get_loop_flag() > 0 ) { // found a loop, so max_len = infinity
                        count++;
                        vertex.second.set_max_len(std::numeric_limits<int>::max(), current);
                        break;
                    }
                    node_q.pop();
                    count++;
                    for ( auto & adj : mVertices[current].get_adj_list() ) {
                        if ( mVertices[adj].get_visitor_flag() == 0 ) {
                            mVertices[adj].increment_visitor_flag();
                            node_q.push(adj);
                        }
                    }
                    if ( node_q.empty() ) { // if the queue is empty and we still haven't broken yet, then max len is not infinity
                        vertex.second.set_max_len(count, vertex.first);     /// @note that this is not the right string for what path to take, but also this case says that there's a reasonable length to reach, so might not matter
                    }
                }
                reset_vertex_flags();
            }
        }
    }

    /**
     * Add an entirely new possible sequence into the graph
     * @param sequence to add to the graph
     */
    void add_sequence(int sequence) {
        add_sequence(std::to_string(sequence));
    }

    /**
     * Add an entirely new possible sequence into the graph
     * @param sequence to add to the graph
     */
    void add_sequence(vector<int> sequence) {
        string input = "";
        for(int i = 0; i < sequence.size(); ++i){
            input += std::to_string(sequence[i]);
        }
        add_sequence(input);
    }

    /**
     * Add an entirely new possible sequence into the graph
     * @param sequence to add to the graph
     */
    void add_sequence(vector<string> sequence) {
        string input = "";
        for(int i = 0; i < sequence.size(); ++i){
            input += sequence[i];
        }
        add_sequence(input);
    }

    /**
     * Add an entirely new possible sequence into the graph.
     * In actual expirimentation, we will choose a "fitter" section of the landscape to pull these combinations from, and then
     * traverse through and randomly do crossovers when we hit a branch.
     * @param sequence to add to the graph
     */
    void add_sequence(string sequence) {
        mSeqSize += 1;
        mSequenceLength = sequence.size();
        // if the beginning string is not in the graph, add a new beginning vertex
        if(mVertices.count(sequence.substr(0, mKmerLength)) <= 0){
            mStarts.push_back(sequence.substr(0, mKmerLength));
            set_empty_vertex(sequence.substr(0, mKmerLength));
            mSize++;
        }
        // go through the entire new sequence and add edges:
        while(int(sequence.length()) >= mKmerLength+1){
            if(mVertices.count(sequence.substr(1, mKmerLength)) <= 0){
                mSize++;
            }
            add_edge(sequence.substr(0, mKmerLength), sequence.substr(1, mKmerLength));
            mVertices[sequence.substr(0, mKmerLength)].set_empty_bool(0); //set that we know this adj_list has something in it
            mVertices[sequence.substr(0, mKmerLength)].increment_kmer_occurrences(); //increment number of times we've seen this kmer in the pangenome
            //if future vertex is not already in map, set it as an empty vertex
            if(mVertices.count(sequence.substr(1, mKmerLength)) <= 0){
                set_empty_vertex(sequence.substr(1, mKmerLength));
            }
            sequence = sequence.substr(1, sequence.length()-1); //update our kmer and repeat!
        }
        //flag that this node's adj_list as empty only if we haven't seen it before (value of 2)
        //if it has previously been flagged with a value of 0, we know it has something in the adj_list
        if(mVertices[sequence].get_empty_bool() == 2){
            mVertices[sequence].set_empty_bool(1);
        }
        mVertices[sequence].increment_endpoint(); //increment number of times this kmer is an endpoint of a seq in the pangenome
        mVertices[sequence].increment_kmer_occurrences(); //increment number of times we've seen this kmer in the pangenome
        update_loops();
    }

private:
    /**
     * remove_sequence helper function to make sure sequence is removed from all containers
     * @param sequence to remove
     */
    void remove(string sequence){
        mSize--;
        mVertices.erase(sequence);
        mStarts.erase(std::remove(mStarts.begin(), mStarts.end(), sequence), mStarts.end());
        mBranchedVertices.erase(std::remove(mBranchedVertices.begin(), mBranchedVertices.end(), sequence), mBranchedVertices.end());
    }

public:
    /**
     * Remove a sequence from the graph
     * To be used in BeforeDeath in MABE
     * @param sequence to remove
     */
    void remove_sequence(string sequence){
        if(is_valid(sequence)){
            mSeqSize--;
            string current, next;
            bool current_appears_once, next_appears_once;
            // while we still have sequence left:
            while(int(sequence.size()) > mKmerLength){

                current = sequence.substr(0,mKmerLength);
                next = sequence.substr(1,mKmerLength);
                current_appears_once = mVertices[current].get_kmer_occurrences() <= 1;
                next_appears_once = mVertices[next].get_kmer_occurrences() <= 1;

                mVertices[current].decrement_kmer_occurrences();
                //if current or next vertex only appeared once in pangenome, break adj & remove edge from graph
                if(current_appears_once || next_appears_once){
                    mVertices[current].remove_from_adj_list(next);
                }
                //if current kmer was only in 1 seq in the pangenome, delete it from mVerticies
                if (current_appears_once){
                    remove(current);
                }
                sequence = sequence.substr(1, sequence.length()-1);

            }
            mVertices[sequence].decrement_kmer_occurrences();
            mVertices[sequence].decrement_endpoint();
            if (mVertices[sequence].get_kmer_occurrences() <= 0){
                remove(sequence);
            }
        }
        //else{ throw std::invalid_argument( "input sequence to DeBruijn remove_sequence() is invalid" ); }
    }

    /**
     * Iterate through graph along sequence to make sure the sequence is in the graph
     * @note Currently in removal of a sequence, I don't check to make sure the entire sequence is valid beforehand, 
     * so if you were to remove a sequence where the first half of it is valid, it would remove those verticies before stopping
     * (I think). Could check like this beforehand--wasn't sure about runtime, but as long as it's not used nested, it should be ok
     * @param sequence to evaluate
     * @return true is the sequence is valid, false if it is not in the graph
     */
    bool is_valid(string sequence){
        string current, next;
        while(int(sequence.size()) > mKmerLength+1){
            current = sequence.substr(0,mKmerLength);
            next = sequence.substr(1,mKmerLength);
            // if the path from this vertex to it's adjacency is invalid, return false
            if(!mVertices[current].valid_adj(next)){
                return false;
            }
            sequence = sequence.substr(1, sequence.length()-1);
        }
        return true;
    }

///@remark DISPLAY AND TRAVERSAL /////////////////////////////////////////////////////////////

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
            func(mStarts[0]);
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
                    mVertices[current].increment_visitor_flag();

                }   
            } 
        }
        reset_vertex_flags();
    }
    
    /**
     * Reset all vertex flags to show they are Unvisited
     * To be used in traversals
     */
    void reset_vertex_flags() {
        for (auto & element : mVertices) {
            element.second.change_visitor_flag(0);
            element.second.clear_adj_availible();
        }
    }

    /**
     * Reset all loop flags to 0 or False
     */
    void reset_loops() {
        for (auto & element : mVertices) {
            element.second.set_loop_flag(0);
        }
    }

    /**
     * Print all vertices on the map to disply them
     * @todo Would like to eventually use Julia to display the graph as a whole
     */
    void display(){
        depth_first_traversal( [&] (string vertex) { 
            cout<<vertex<<" ";
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
            // if the vertex is a loop, show that
            if (mVertices[vertex].get_loop_flag()>0){
                cout << " (a loop at " << vertex << " = " << mVertices[vertex].get_loop_flag() << ")";
            }
            cout<<"\n";
            });
    }

///@remark CSV GENERATOR /////////////////////////////////////////////////////////////

    /**
     * Create a CSV file that has a column for the time/generation we are recording, 
     * the number of times each kmer appears in the graph, the kmer, and each adjacency
     * of the kmer (there may be multiple rows per node&it's seq. count if the node has
     * more than one adjacency)
     * @param time (generation) in string form that we are recording that graph at
     */
    void csv(string time){
        file.open("dbg_"+time+".csv");
        file<<"Time,Count,From,To"<<"\n";

        string traits;
        for(auto vertex : get_all_vertices()){
            for(auto adj : get_value(vertex).get_adj_list()){
                traits+=time+",";
                traits+=std::to_string(get_value(vertex).get_kmer_occurrences())+",";
                traits+=vertex+",";
                traits+=adj+",";
                file << traits << "\n";
                traits = "";
            }
        }
    }

    /**
     * This function is one that I used ONLY to express the pattern used in the CSV helper functions below.
     * The reason why those work is because we iterate through the graph in the same order in each function.
     */
    void example_iteration(){
        for(auto vertex : mVertices){
            for(auto adj : mVertices[vertex.first].get_adj_list()){
                cout<<vertex.first<<"->"<<adj<<endl;
            }
        }
    }

    /**
     * Helper function for MABE2 PangenomeAnalysis module.
     * Returns first values of iteration to begin creating a CSV file representing 
     * @return int sequence count, starting vertex, starting vertex's 1st adjacency
     */
    tuple<int,string,string> csv_start_values(){
        vector<string> temp = get_all_vertices();
        for(auto vertex : temp){
            for(auto adj : mVertices[vertex].get_adj_list()){
                return std::make_tuple(mVertices[vertex].get_kmer_occurrences(), vertex, adj);
            }
        } return std::make_tuple(-1, "", "");
    }

    /**
     * Helper function for MABE2 PangenomeAnalysis module. 
     * Added as a lambda to create a column in a Datafile that the module will produce.
     * @param count sequence count of current vertex
     * @param from kmer ID of current vertex
     * @param to current adjacency of current vertex
     * @return tuple<int, int> current and next sequence count
     */
    tuple<int, int> kmer_count(int count, string from, string to){
        int current = 0, next = 0;
        for(auto vertex : mVertices){
            for(auto adj : mVertices[vertex.first].get_adj_list()){
                if(current > 0){ // record the next count
                    next = mVertices[vertex.first].get_kmer_occurrences();
                    return std::make_tuple(current, next);
                }
                if(mVertices[vertex.first].get_kmer_occurrences()==count && mVertices[vertex.first].get_visitor_flag()<mVertices[vertex.first].adj_list_size() && vertex.first==from && adj==to){ // match current count
                    current = count;
                    mVertices[vertex.first].increment_visitor_flag();
                }
            }
        }
        if(current > 0){ // if we're on the last vertex of the graph, there will not be a next count
            return std::make_tuple(current, -1);
        }
        return std::make_tuple(-1, -1); // return invalid tuple if no other case is matched
    }

    /**
     * Helper function for MABE2 PangenomeAnalysis module. 
     * Added as a lambda to create a column in a Datafile that the module will produce.
     * @param count sequence count of current vertex
     * @param from kmer ID of current vertex
     * @param to current adjacency of current vertex
     * @return tuple<int, int> current and next sequence count
     * 
     * @note Do we need a count of edges as well as how frequently a kmer appears in the graph?
     *       Currently do not have a count for specific edges of From to To, only count of From
     *       How would we even represent this though? if From has multiple edges
     */
    tuple<int, int> edge_count(int count, string from, string to){
        int current = 0, next = 0;
        for(auto vertex : mVertices){
            for(auto adj : mVertices[vertex.first].get_adj_list()){
                if(current > 0){ // record the next count
                    //next = mVertices[vertex.first].get_kmer_occurrences();
                    return std::make_tuple(current, next);
                }
                if(mVertices[vertex.first].get_kmer_occurrences()==count && mVertices[vertex.first].get_visitor_flag()<mVertices[vertex.first].adj_list_size() && vertex.first==from && adj==to){ // match current count
                    //current = count;
                    //mVertices[vertex.first].increment_visitor_flag();
                }
            }
        }
        if(current > 0){ // if we're on the last vertex of the graph, there will not be a next count
            return std::make_tuple(current, -1);
        }
        return std::make_tuple(-1, -1); // return invalid tuple if no other case is matched
    }

    /**
     * Helper function for MABE2 PangenomeAnalysis module. 
     * Added as a lambda to create a column in a Datafile that the module will produce.
     * @param count sequence count of current vertex
     * @param from kmer ID of current vertex
     * @param to current adjacency of current vertex
     * @return tuple<string, string> current and next IDs for the kmer of the vertex
     */
    tuple<string, string> from(int count, string from, string to){
        string current = "", next = "";
        for(auto vertex : mVertices){
            for(auto adj : mVertices[vertex.first].get_adj_list()){
                if(!current.empty()){ //same pattern as above
                    next = vertex.first;
                    return std::make_tuple(current, next);
                }
                if(vertex.first==from && mVertices[vertex.first].get_visitor_flag()<=mVertices[vertex.first].adj_list_size() && adj==to){ //on 123
                    current = from;
                }
            }
        } 
        if(!current.empty()){ //same pattern as above
            return std::make_tuple(current, "");
        }
        return std::make_tuple("", "");
    }

    /**
     * Helper function for MABE2 PangenomeAnalysis module. 
     * Added as a lambda to create a column in a Datafile that the module will produce.
     * @param count sequence count of current vertex
     * @param from kmer ID of current vertex
     * @param to current adjacency of current vertex
     * @return tuple<string, string> current and next adjacencies to the current vertex
     */
    tuple<string, string> to(int count, string from, string to){
        if(count == -1 && from.empty()){ //if this is the last edge in the whole graph iteration
            return std::make_tuple(to, "");
        }
        return std::make_tuple(to, mVertices[from].get_adj_list()[mVertices[from].get_visitor_flag()]);
    }

///@remark GETTERS AND SETTERS /////////////////////////////////////////////////////////////

    /**
     * Return size of graph (NOT the total number of sequences in the pangenome)
     * @return number of vertices the graph contains
     */
    int get_size() { return mSize; }

    /**
     * Return sequence size of graph
     * @return number of sequences the graph has added into the total pangenome
     */
    int get_sequence_size() { return mSeqSize; }

    /**
     * Return sequence size of graph
     * @return number of sequences the graph has added into the total pangenome
     */
    int get_kmer_len() { return mKmerLength; }

    /**
     * @return int number of edges in the graph
     */
    int edge_count(){
        int count = 0;
        for(auto i : mVertices){
            count += i.second.adj_list_size();
        }
        return count;
    }

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
     * Return vector containing vertices that represent the beginning(s) of the graph
     * @return vector containing beginning DeBruijn vertex objects
     */
    vector<string> get_start_vertices() { return mStarts; }

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
     * Set the availible adj list object to have all adjacencies possible
     * @param kmer we are currently adding
     */
    void set_avail_adj_list(string kmer) { mVertices[kmer].set_adj_availible(); }

    /**
     * Remove an adjacency from availible adj list
     * @param kmer we are adjusting the value of
     * @param adj string we are removing
     */
    void remove_avail_adj_list(string kmer, int adj) { mVertices[kmer].remove_adj_availible(mVertices[kmer].get_adj_availible(0)); }

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNGRAPH_H
