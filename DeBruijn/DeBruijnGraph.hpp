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
#include <limits>
#include <set>

using std::string; using std::vector; using std::map;
using std::cout; using std::endl; using std::tuple;
using std::get; using std::make_tuple; using std::count;
using std::queue; using std::set;

class DeBruijnGraph {
public:
    using fun_t = void(std::ostream &);
    using fun_f = void(std::ofstream &);
private:

    /// Length of the k-mer IDs
    int mKmerLength = 3;

    /// Length of sequences (number of bits in BitsOrg)
    int mSequenceLength = 0;

    /// Map of Debruijn vertex objects to their values/data
    map<string, DeBruijnValue> mVertices;

    /// Vector of all beginning verticies
    set<string> mStarts;

    std::ofstream file;


    /**
     * Set a vertex with no value as a place-holder
     * @param v vertex object to add to the graph's list of vertices
     */
    void set_empty_vertex(string v) {
        mVertices[v];
    }

    /**
     * Set minimum and maximum possible path lengths from this node
     * @param v vertex object
     * @param path_length from this node to an endpoint
     * @param adj next node after v
     * @param new_node true if there's nothing in the adj list, and min and max length should both be set to this length
     */
    void set_path_length(string v, int path_length, string adj) {
        mVertices[v].append_path_len(path_length, adj);
    }

    /**
     * Add an edge between two vertices
     * @param start_v Starting Debruijn vertex
     * @param end_v Vertex being pointed to
     */
    void add_edge(string past_v, string start_v, string end_v){
        int initial_adj_size = mVertices[start_v].adj_list_size();
        mVertices[start_v].add_to_adj_list(end_v);
        if(past_v.length() > 0) { mVertices[start_v].set_in_head(past_v); }
        mVertices[start_v].set_in_tail(start_v);
        mVertices[start_v].set_out_head(start_v);
        mVertices[start_v].set_out_tail(end_v);
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
    void construct_from_string(string input, int kmer_length) {
        mSequenceLength = input.size();
        mKmerLength = kmer_length;
        mStarts.insert(input.substr(0, kmer_length));
        string past = "";
        //if the graph is one vertex long:
        if(int(input.length()) == kmer_length){
            set_empty_vertex(input);
        }
        // if the vertex is already in the graph, then skip this and don't add to size //uh???????????
        if(mVertices.count(input.substr(0, mKmerLength)) <= 0){
            set_empty_vertex(input.substr(0, mKmerLength));
            // min/max path length from this node
            int path_len = input.size() - 3;
            set_path_length(input.substr(0, kmer_length), path_len, input.substr(1, kmer_length));
        }
        // add to size and add an edge for each vertex, and an empty vertex for the end
        while(int(input.length()) >= kmer_length + 1){
            add_edge(past, input.substr(0, kmer_length), input.substr(1, kmer_length));
            mVertices[input.substr(0, kmer_length)].increment_kmer_occurrences();
            set_empty_vertex(input.substr(1, kmer_length));
            // min/max path length from this node
            int path_len = input.size() - 3;
            set_path_length(input.substr(0, kmer_length), path_len, input.substr(1, kmer_length));
            // take one character off the input, continue
            past = input.substr(0, mKmerLength);
            input = input.substr(1, input.length()-1);
        }
        mVertices[input].increment_endpoint();
        mVertices[input].increment_kmer_occurrences();
        mVertices[input].set_out_head(input);
        mVertices[input].set_in_head(past);
        mVertices[input].set_in_tail(input);
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
     * Using visitor flags, detect and mark all loops in the graph
     */
    void update_loops() {
        reset_vertex_flags();   // resets DeBruijnValue visitor flags
        reset_edge_flags();     // reset edge visitor flags in case we hit the infinity_length function and use them
        reset_loops();          // completely reset loop flags in case anything has changed
        /*for (auto & element : mVertices) {
            std::cout<<"loop flag = "<<element.second.get_loop_flag()<<" max path = "<<get<0>(element.second.get_max_length())<<"\n";
            
        }*/
        loop_detection();
        reset_edge_flags();
        reset_vertex_flags();
    }

    /**
     * Recursive function for loop detection in a cyclic graph
     */
    void loop_detection() {
        if(mVertices.size() >= 1) {
            set<string> path = mStarts;
            //std::cout<<"\nstart loop detectyion ";
            string current = "";
            while(path.size() > 0) {
                // pop off next kmer
                current = *path.begin();
                path.erase(path.begin());
                //std::cout<<"current = "<<current<<" path = ";
                //for (auto i : path) { std::cout<< i<<", ";} std::cout<<"\n";

                // if we have seen this kmer before, we found a loop!
                if (mVertices[current].get_visitor_flag() == 1) {
                    if (current == "111"){ //display();
                    }
                    mVertices[current].set_loop_flag(1);
                    infinity_length(current);
                    reset_edge_flags();
                }

                // if this is the first time the vertex is being visited, we need to add it's adj_list into the queue
                if (mVertices[current].get_visitor_flag() == 0) {
                    for (string it : mVertices[current].get_adj_list()) {
                        path.insert(it);
                    }
                }

                mVertices[current].increment_visitor_flag();
            }
        }
    }

    /**
     * If we have a node that we know is in a loop, we'll mark every node that leads to it as a maximum possible path
     * length of infinity, so that we don't end up in an endless loop when we try to randomly generate a new genome.
     * @param node kmer that we KNOW is part of a loop
     */
    void infinity_length(string node) {
        //std::cout<<"inf length staring node = "<<node<<"\n";
        queue<pair<string, string>> Q_parent;
        DeBruijnEdge & start_edge = mVertices[node].get_in_edge();
        //std::cout<<"initial heads ";
        for (auto i : start_edge.get_head() ){
            //std::cout<<i<<", ";
            Q_parent.push(make_pair(i, node));
        } //std::cout<<"\n";
        string current, parent;
        while(!Q_parent.empty()) {
            parent = Q_parent.front().second;
            current = Q_parent.front().first;
            //std::cout<<"parent "<<parent<<" child "<<current<<"\n";
            Q_parent.pop();
            DeBruijnEdge & edge = mVertices[current].get_in_edge(); //next edge
            mVertices[current].append_path_len(std::numeric_limits<int>::max(), parent);
            if(edge.get_visits() == 0) {
                for (auto i : edge.get_head() ){
                    Q_parent.push(make_pair(i, current));
                }
            }
            edge.increment_edge_visitor_flag();
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
        //std::cout<<"hi\n";
        mSequenceLength = sequence.size();
        string past = "";
        // if the beginning string is not in the graph, add a new beginning vertex
        if(mVertices.count(sequence.substr(0, mKmerLength)) <= 0){
            mStarts.insert(sequence.substr(0, mKmerLength));
            set_empty_vertex(sequence.substr(0, mKmerLength));
        }
        // go through the entire new sequence and add edges:
        while(int(sequence.length()) >= mKmerLength+1){
            //std::cout<<"\n"<<sequence<<"    ";
            if(mVertices.count(sequence.substr(1, mKmerLength)) <= 0){
                //std::cout<<mVertices.count(sequence.substr(1, mKmerLength))<<"    ";
            }
            // min/max path length from this node
            int path_len = sequence.size() - 3;
            set_path_length(sequence.substr(0, mKmerLength), path_len, sequence.substr(1, mKmerLength));
            //std::cout<<"path len set to "<<path_len<<"    ";
            add_edge(past, sequence.substr(0, mKmerLength), sequence.substr(1, mKmerLength));
            //std::cout<<"edge btwn "<<sequence.substr(0, mKmerLength)<<" and "<<sequence.substr(1, mKmerLength)<<"\n";
            mVertices[sequence.substr(0, mKmerLength)].increment_kmer_occurrences(); //increment number of times we've seen this kmer in the pangenome
            //if future vertex is not already in map, set it as an empty vertex
            if(mVertices.count(sequence.substr(1, mKmerLength)) <= 0 && sequence.length() != (mKmerLength)){
                set_empty_vertex(sequence.substr(1, mKmerLength));
            }
            past = sequence.substr(0, mKmerLength);
            sequence = sequence.substr(1, sequence.length()-1); //update our kmer and repeat!
        }
        mVertices[sequence].increment_endpoint(); //increment number of times this kmer is an endpoint of a seq in the pangenome
        mVertices[sequence].increment_kmer_occurrences(); //increment number of times we've seen this kmer in the pangenome
        mVertices[sequence].set_out_head(sequence);
        mVertices[sequence].set_in_head(past);
        mVertices[sequence].set_in_tail(sequence);
        update_loops();
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
            if (auto search = mVertices.find(current); search != mVertices.end()) {
                if(!mVertices[current].valid_adj(next)){
                    return false;
                }
            }
            else return false;
            sequence = sequence.substr(1, sequence.length()-1);
        }
        return true;
    }

    private:
    /**
     * remove_sequence helper function to make sure sequence is removed from all containers
     * @param sequence to remove
     */
    void remove(string current, string next){
        mVertices.erase(current);
        auto iter = mStarts.find(current);
        if(iter != mStarts.end()) { mStarts.erase(iter); }
        //for(auto i : mVertices) {
        //    i.second.remove_path_len(current); // do we still need this if we have line 414?
        //}
    }

    public:
    /**
     * Remove a sequence from the graph
     * To be used in BeforeDeath in MABE
     * @param sequence to remove
     */
    void remove_sequence(string sequence){
        if(is_valid(sequence)){
            string current, next;
            bool current_appears_once;
            bool next_appears_once;
            //std::cout<<"remove "<<sequence<<"... ";

            // while we still have sequence left:
            while(int(sequence.size()) > mKmerLength){
                current = sequence.substr(0,mKmerLength);
                next = sequence.substr(1,mKmerLength);
                mVertices[current].decrement_kmer_occurrences();
                mVertices[current].remove_path_len(next);
                current_appears_once = mVertices[current].get_kmer_occurrences() <= 0;
                next_appears_once = mVertices[next].get_kmer_occurrences() <= 1;
                std::cout<<"Current (bool = "<<current_appears_once<<") "<<current<<" = "<<mVertices[current].get_kmer_occurrences()<<" Next (bool = "<<next_appears_once<<") "<<next<<" = "<<mVertices[next].get_kmer_occurrences()<<"\n";

                if (current_appears_once  || next_appears_once) {
                    mVertices[current].remove_from_adj_list(next); //we should be removing all copies of this kmer if it's the only occurance in the graph, but we may be removing only one string in the vector
                    mVertices[current].get_out_edge().remove_tail(next);
                    mVertices[next].get_in_edge().remove_head(current);
                    //mVertices[current].remove_path_len(next);
                }
                if (current_appears_once ){ remove(current, next); } //if kmer only appears once in entire graph, when we delete this instance, we delete it from all objects
                
                sequence = sequence.substr(1, sequence.length()-1);
            }

            mVertices[sequence].decrement_kmer_occurrences(); // sequence is reduced to its last kmer
            mVertices[sequence].decrement_endpoint();
            if (mVertices[sequence].get_kmer_occurrences() <= 0){
                mVertices.erase(sequence);
                auto iter = mStarts.find(sequence);
                if(iter != mStarts.end()) { mStarts.erase(iter); }
                for(auto i : mVertices) {
                    i.second.remove_path_len(sequence);
                }
            }

            if(mVertices.size() > 0) { // update loop flags for sequences still in graph
                update_loops(); //std::cout<<"got here\n";
            }
        }
        //else{ throw std::invalid_argument( "input sequence to DeBruijn remove_sequence() is invalid" ); }
    }

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
            std::set<string>::iterator it = mVertices[current].get_adj_list().begin();
            advance(it, random.GetUInt(mVertices[current].adj_list_size()-1));
            current = *it; ///random seed here?!
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
    string modify_org(emp::Random & random, std::string organism, double probability = 1, bool seq_count = 1, bool variable_length = 0){ 
        string path = organism.substr(0, mKmerLength);          // initialize string variables we use to change and go down the path
        string current = organism.substr(0, mKmerLength);
        string next = "";
        mVertices[current].increment_visitor_flag(); // mark 1st kmer as visited
        int index;

        // If P() then we will modify this genome, else do nothing
        if( random.P( probability ) ) {
            while ( int(path.size()) < mSequenceLength) { // while our path hasn't reached the sequence length

                if(mVertices[current].get_visitor_flag() == 1) {         
                    mVertices[current].make_all_adj_availible(); // available choices = full adj_list if this is our first time seeing it
                }

                if(variable_length && mVertices[current].get_endpoint() > 0) { // if genome can be variable length and current kmer is an availible endpoint
                    index = random.GetUInt(mVertices[current].adj_availible_size() + 1);  // index will be randomly generated number
                    if( index == mVertices[current].adj_availible_size() ) { // if we have randomly chosen to keep this kmer as an endpoint
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
     * Out of all the adjacencies, possible paths we could take through the graph, 
     * which path will generate the most reasonably lengthed genome?
     * current + min > parent is unreasonable and current + max < parent is unreasonable
     * @param node node value that we're currently on. last node value added to our new genome that is currently being generated.
     * @param current_len current length of the new genome we are generating
     * @param parent_len length of genome of parent organism, this is our general target of a length to generate
     * @return 1 if there is for sure no endpoint yet
     *         0 if there is a possible endpoint
    */
    int make_adj_availible(DeBruijnValue & node, int current_len, int parent_len, int threshold = 0){
        node.clear_adj_availible();
        if ( current_len < parent_len ) { // if we're underneath the target length, and the path is not too short, add it
            node.not_too_short(current_len, parent_len, 0); // only the not too short ones
            //std::cout<<"OPTION 1 \n";
            if(node.adj_availible_size() == 0) {
                //std::cout<<"OPTION 3 \n";
                node.make_all_adj_availible();
                return 0;
            }
            return 1;
        }
        else { //if we are already at the target length, choose endpoint or lowest min path and break out of while loop
            node.append_adj_availible(get<1>(node.get_min_length())); // put minimum path length adjs into the list of availible adjs
            /**std::cout<<"OPTION 2 "<<get<0>(node.get_min_length())<<" from ";
            for(auto i : node.get_all_adj_availible()){
                std::cout<<i<<", ";
            }std::cout<<"\n";*/
            return 0;
        }
    }

    /**
     * @brief coding for one specific base case
     * Availible adj list used to track which adj can lead to reasonable length
     * May try with weights, and because of that, will keep track of visitor flag.
     * 
     * @param random 
     * @param organism 
     * @param probability
     * @return string 
     */
    string modify_org_variable_len(emp::Random & random, std::string organism, double probability = 1){
        string path = organism.substr(0, mKmerLength);
        string current = organism.substr(0, mKmerLength);
        string next = "";
        mVertices[current].increment_visitor_flag(); // Mark 1st kmer as visited
        int index;
        if ( random.P ( probability ) ) { // If P() then we will modify this genome, else do nothing
            while ( true ) {
                if ( mVertices[current].adj_list_size() < 1 ){ // if we have no possible adjacencies, we are at an endpoint with no options (besides turning around)
                    // if path length is 0, return original genome. maybe take this out later when there are no more errors?
                    //std::cout<<"Path length is "<<path.length()<<" but adj list "<<mVertices[current].adj_list_size()<<"\n";
                    if (path.length() == 0) {  
                        //std::cout<<"Path Length = 0, so returning original organism. No crossover today.\n";
                        path = organism;
                    }
                    break;
                }
                int flag = make_adj_availible(mVertices[current], path.length(), organism.length());
                if ( flag == 0 && mVertices[current].get_endpoint() > 0 && path.length() > 0){ // we are traveling down the shortest path possible
                    if (path.length() == 0) {  
                        //std::cout<<"Path Length = 0, so returning original organism. No crossover today.\n";
                        path = organism;
                    }
                    break;
                }
                //std::cout<<"CURRENT:";  info(current);
                index = random.GetUInt ( mVertices[current].adj_availible_size() );  // Choose random path down graph
                next = mVertices[current].get_adj_availible(index);
                //std::cout<<"NEXT "<<next<<"\n";
                path += next.substr(2,1);
                current = next;
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

///@remark DISPLAY AND TRAVERSAL /////////////////////////////////////////////////////////////

    template <typename FuncType>
    /**
     * Traversal function that currently prints each vertex ID
     * Visits kmers only one time each
     * @param func lambda function to use when visiting the current vertex
     * @note Depending on what we're traversing for, like if we need to compare adjacent 
     *      verticies, can make a 2-vertex parameter on the lambda, or if we need the function
     *      to return something (or a template of something)
     */
    void traversal(FuncType func){
        reset_vertex_flags();
        // edge case--this traversal did not work for size of 1 without it
        if(mVertices.size() == 1){
            func(*mStarts.begin()); 
        }
        else{
            // because this is a directed graph, I want to make sure each path start is covered
            // therefore, I will put all the beginnings into my queue to start traversal
            set<string> path = mStarts;
            string current = "";
            while(path.size() > 0){
                current = *path.begin();
                path.erase(path.begin());
                if(mVertices[current].get_visitor_flag() < 1){
                    func(current);
                    // if this is the first time the vertex is being visited, we need to add it's adj_list into the queue
                    // otherwise, the adjacencies are already in there somewhere, so not needed
                    for (std::string it : mVertices[current].get_adj_list()) {
                        path.insert(it);
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
     * Reset all edge flags to show they are Unvisited
     * To be used in update_loops functions
     */
    void reset_edge_flags() {
        for (auto & element : mVertices) {
            element.second.get_in_edge().clear_edge_visitor_flag();
            element.second.get_out_edge().clear_edge_visitor_flag();
        }
    }

    /**
     * Reset all loop flags to 0 or False
     */
    void reset_loops() {
        for (auto & element : mVertices) {
            element.second.set_loop_flag(0);
            element.second.remove_inf_path();
        }
    }

    /**
     * Print all vertices on the map to disply them
     * @todo Would like to eventually use Julia to display the graph as a whole
     */
    void display(){
        //std::cout<<"a     ";
        traversal( [&] (string vertex) { 
            cout<<vertex<<" ";
            // if there is one, non-empty vertex in the list, print it
            if (mVertices[vertex].adj_list_size() == 1){
                cout<<" -> "<< *mVertices[vertex].get_adj_list().begin();
            }
            // if the adj_list has more than one node in it, print them
            else if (mVertices[vertex].adj_list_size() >= 1){
                cout<<" -> ";
                for(auto i: mVertices[vertex].get_adj_list()){
                    cout<<i<<", ";
                }
            }
            //std::cout<<"c     ";
            // if the adj_list contains an endpoint/empty vertex, show that
            if (mVertices[vertex].get_endpoint()>0){
                cout << " (an endpoint)";
            }
            //std::cout<<"d     ";
            // if the vertex is a loop, show that
            if (mVertices[vertex].get_loop_flag()>0){
                cout << " (a loop at " << vertex << " = " << mVertices[vertex].get_loop_flag() << ")";
            }
            //std::cout<<"e     ";
            std::cout<<"\n";
            //std::cout<<"f     ";
            });
            //std::cout<<"g     ";
    }

    /**
     * Print attribute information about a certain node in the graph
     * @param current node to display
     */
    void info (string current) {
        std::cout<<"\nstring = "<<current;
        std::cout<<"\nin edge head ("<<mVertices[current].get_in_edge().get_head().size()<<") = ";
        for (auto i : mVertices[current].get_in_edge().get_head()){
            std::cout<<i<<", ";
        }
        std::cout<<"\nin edge tail ("<<mVertices[current].get_in_edge().get_tail().size()<<") = ";
        for (auto i : mVertices[current].get_in_edge().get_tail()){
            std::cout<<i<<", ";
        }
        std::cout<<"\nout edge head ("<<mVertices[current].get_out_edge().get_head().size()<<") = ";
        for (auto i : mVertices[current].get_out_edge().get_head()){
            std::cout<<i<<", ";
        }
        std::cout<<"\nout edge tail ("<<mVertices[current].get_out_edge().get_tail().size()<<") = ";
        for (auto i : mVertices[current].get_out_edge().get_tail()){
            std::cout<<i<<", ";
        }
        std::cout<<"\nAVAIL ADJ SZ "<<mVertices[current].adj_availible_size()<<": ";
        set<string> all_results = mVertices[current].get_all_adj_availible();
        for (auto i : all_results){
            std::cout<<i<<", ";
        }
        std::cout<<"\nALL ADJ SZ "<<mVertices[current].adj_list_size()<<": ";
        all_results.clear();
        all_results = mVertices[current].get_adj_list();
        for (auto i : all_results){
            std::cout<<i<<", ";
        }
        std::cout<<"\nendpoint flag = "<<mVertices[current].get_endpoint();
        std::cout<<"\nkmer occurances = "<<mVertices[current].get_kmer_occurrences();
        std::cout<<"\navail adj list size = "<<mVertices[current].adj_availible_size();
        std::cout<<"\nmax len = "<<get<0>(mVertices[current].get_max_length());
        std::cout<<" min len = "<<get<0>(mVertices[current].get_min_length());
        std::cout<<"\nloop = "<<mVertices[current].get_loop_flag();
        std::cout<<" visits = "<<mVertices[current].get_visitor_flag();
        std::cout<<std::endl;
        std::cout<<std::endl;
        //display();
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
    /*tuple<int, int> kmer_count(int count, string from, string to){
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
    }*/
    tuple<int, int> kmer_count(int count, string from, string to){
        return std::make_tuple(mVertices[from].get_kmer_occurrences(), mVertices[to].get_kmer_occurrences());
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
        std::set<string>::iterator it = mVertices[from].get_adj_list().begin();
        advance(it, mVertices[from].get_visitor_flag());
        return std::make_tuple(to, *it);
    }

///@remark GETTERS AND SETTERS /////////////////////////////////////////////////////////////

    /**
     * Return size of graph (NOT the total number of sequences in the pangenome)
     * @return number of vertices the graph contains
     */
    int get_size() { return mVertices.size(); }

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
     * Get all beginning vertices in the graph
     * @return set of strings representing kmers of the starting vertices
     */
    set<string> get_starts() { return mStarts; }

    /**
     * Get all vertices in the graph
     * @return map representing the DeBruijn graph
     */
    map<string, DeBruijnValue> get_graph() { return mVertices; }

    /**
     * Get the value associated with a vertex
     * @return Debruijn vertex value object
     */
    DeBruijnValue get_value(string vertex) { return mVertices[vertex]; }

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNGRAPH_H
