/**
 * @file DeBruijnGraph.h
 * @author Anna Catenacci
 *
 * A class that constructs and describes a De Bruijn Graph
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

using std::string; using std::vector; using std::map;
using std::cout;

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
        if(initial_adj_size > 0){ //if the adj_list is not empty, AND new adj_list size > old_adj_list.size, then
            if(initial_adj_size < mVertices[start_v].adj_list_size()){ //this implies the vertex is a branch point
                mVertices[start_v].set_branch(true);
                mBranchedVertices.push_back(start_v);
                vector<string>::iterator it;
                mBranchedVertices.erase( std::unique( mBranchedVertices.begin(), mBranchedVertices.end() ), mBranchedVertices.end() );
            }
        }
        mVertices[start_v].increment_sequence_count();
        
    }

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
        mSeqSize += 1;
        mSequenceLength = input.size();
        mKmerLength = kmer_length;
        mStarts.push_back(input.substr(0, kmer_length));
        //if the graph is one vertex long:
        if(int(input.length()) == kmer_length){
            this->set_empty_vertex(input);
        }
        // if the vertex is already in the graph, then skip this and don't add to size
        if(mVertices.count(input.substr(0, mKmerLength)) <= 0){
            mSize++;
        }
        //add to size and add an edge for each vertex, and an empty vertex for the end
        while(int(input.length()) >= kmer_length + 1){
            if(mVertices.count(input.substr(1, mKmerLength)) <= 0){
                mSize++;
            }
            this->add_edge(input.substr(0, kmer_length), input.substr(1, kmer_length));
            //change the set_empty_bool here so we don't run into endpoint troubles later.
            mVertices[input.substr(0, kmer_length)].set_empty_bool(0);
            this->set_empty_vertex(input.substr(1, kmer_length));
            //take one character off the input, continue
            input = input.substr(1, input.length()-1);
        }
        mVertices[input].set_empty_bool(1);
        mVertices[input].increment_endpoint();
        mVertices[input].increment_sequence_count();
    }

public:
    DeBruijnGraph()=default;
    ~DeBruijnGraph()=default;

///@remark CONSTRUCTORS /////////////////////////////////////////////////////////////

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

    void check_for_endpoint(string current){
        vector<string> adj = mVertices[current].get_adj_list();
        for(auto i : adj){
            cout<<i<<" ";
            if(mVertices[i].get_sequence_count() == mVertices[i].get_endpoint()){
                mVertices[current].remove_adj_availible(i, 1);
                cout<<"<removed ";
            }
        }
        cout<<std::endl;

    }

    /**
     * Given the genome of an organism, do crossover (if probability allows) by randomly choosing branches of existing 
     * genomes in the graph to pursue
     * @param random Empirical random number generator
     * @param organism whose genome we are modifying--here I am just going to insert a 3-char string to represent the start
     * 
     * @note should a sequence be availible as many times as we see it in all sequences in the graph or only 
     *       as many times as it appears in the sequence it appears most in?
     * so I either need to somehow make sure random does not land on an index that has reached it's max visitor count
     * or if it is going to be a premature endpoint without any adj to choose from
     */
    string modify_org(emp::Random & random, std::string organism, double probability = 1){
        organism = organism.substr(0,mKmerLength); //make the string the first kmer if it is not already
        string path = organism;                    //initialize string variables we use to change and go down the path
        string current = organism;
        string next;
        mVertices[current].change_visitor_flag(mVertices[current].get_visitor_flag()+1); //mark 1st kmer as visited
        int index;
        
        //If P() then we will modify this genome, else do nothing
        if(random.P(probability)){
            while (int(path.size()) < mSequenceLength){                 //while our path hasn't reached the sequence length
                if(mVertices[current].get_visitor_flag() == 1){         
                    mVertices[current].set_adj_availible();      //available choices = full adj_list if this is our first time seeing it
                }
                index = random.GetUInt(mVertices[current].adj_availible_size());  //index will be randomly generated number
                next = mVertices[current].get_adj_availible(index);                 //record next kmer using index
                path+= next.substr(2,1);                                            //add it to path

                mVertices[next].change_visitor_flag(mVertices[next].get_visitor_flag()+1);  //mark next as visited
                if(mVertices[next].get_visitor_flag() == mVertices[current].get_sequence_count()){
                    mVertices[current].remove_adj_availible(next);  //remove kmer from availible seq.s if it has been visited as many times
                }                                                   //as it appears in all sequences in graph
                current = next;
            }
        }
        this->reset_vertex_flags();
        return path;
    }

    /**
     * Same as modify_org, but "No Sequence Count" (NSC). Kmers are no longer labeled unavailible if they have been used the same number of
     * times that they have appeared in entire living genome.
     */
    string modify_org_NSC(emp::Random & random, std::string organism, double probability = 1){
        organism = organism.substr(0,mKmerLength); //make the string the first kmer if it is not already
        string path = organism;                    //initialize string variables we use to change and go down the path
        string current = organism;
        string next;
        mVertices[current].change_visitor_flag(mVertices[current].get_visitor_flag()+1); //mark 1st kmer as visited
        int index;
        
        //If P() then we will modify this genome, else do nothing
        if(random.P(probability)){
            while (int(path.size()) < mSequenceLength){                 //while our path hasn't reached the sequence length
                if(mVertices[current].get_visitor_flag() == 1){         
                    mVertices[current].set_adj_availible();      //available choices = full adj_list if this is our first time seeing it
                }
                index = random.GetUInt(mVertices[current].adj_availible_size());  //index will be randomly generated number
                next = mVertices[current].get_adj_availible(index);                 //record next kmer using index
                path+= next.substr(2,1);                                            //add it to path

                mVertices[next].change_visitor_flag(mVertices[next].get_visitor_flag()+1);  //mark next as visited
                current = next;
            }
        }
        this->reset_vertex_flags();
        return path;
    }

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
        mSeqSize += 1;
        mSequenceLength = sequence.size();
        // if the beginning string is not in the graph, add a new beginning vertex
        if(mVertices.count(sequence.substr(0, mKmerLength)) <= 0){
            mStarts.push_back(sequence.substr(0, mKmerLength));
            mSize++;
        }
        // go through the entire new sequence and add edges:
        while(int(sequence.length()) >= mKmerLength+1){
            if(mVertices.count(sequence.substr(1, mKmerLength)) <= 0){
                mSize++;
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
        mVertices[sequence].increment_endpoint();
        mVertices[sequence].increment_sequence_count();
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
        mSeqSize -= 1;
        /*if(!this->is_valid(sequence)){
            throw std::invalid_argument( "input sequence to DeBruijn remove_sequence() is invalid" );
        }*/
        if(this->is_valid(sequence)){
            //mSeqSize -= 1;
            string current, next;
            bool current_duplicated, next_duplicated;
            // while we still have sequence left:
            while(int(sequence.size()) > mKmerLength){

                current = sequence.substr(0,mKmerLength);
                next = sequence.substr(1,mKmerLength);
                current_duplicated = mVertices[current].get_sequence_count() > 1;
                next_duplicated = mVertices[next].get_sequence_count() > 1;

                mVertices[current].decrement_sequence_count();
                //if the entire sequence is not exactly a duplicate, remove adjacency
                if(!current_duplicated || !next_duplicated){
                    mVertices[current].remove_from_adj_list(next);
                }
                //if current is not a duplicate, delete it from mverticies
                if (!current_duplicated){
                    this->remove(current);
                }
                sequence = sequence.substr(1, sequence.length()-1);


            }
            mVertices[sequence].decrement_sequence_count();
            mVertices[sequence].decrement_endpoint();
            if (mVertices[sequence].get_sequence_count() == 0){
                this->remove(sequence);
            }
        }
    }

    /**
     * Iterate through graph along sequence to make sure the sequence is in the graph
     * @note Currently in removal of a sequence, I don't check to make sure the wntire sequence is valid beforehand, 
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
                    mVertices[current].change_visitor_flag(mVertices[current].get_visitor_flag()+1);
                }   
            } 
        }
        this->reset_vertex_flags();
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

///@remark GETTERS AND SETTERS /////////////////////////////////////////////////////////////

    /**
     * Set the size object
     * @param s size of graph
     */
    void set_size(int s) { mSize = s; }

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
    
    std::function<fun_t> csv_multiple_ofstream(){
        std::function<fun_t> from_fun, to_fun;
        /*for(auto vertex : this->get_all_vertices()){
            
        }

        std::function<fun_t> from_fun =
        [&](std::ostream & os){
        };

        std::function<fun_t> to_fun =
        [&](std::ostream & os){
          //os << node.GetHistCount(bin_id);
          depth_first_traversal([&] (string vertex) {
             os<<vertex;
                os<<" -> ";
                for(int i = 0; i < int(mVertices[vertex].get_adj_list().size()-1); i++){
                    os<<mVertices[vertex].get_adj_list()[i];
                    os<<", ";
                }
                os<<mVertices[vertex].get_adj_list()[mVertices[vertex].get_adj_list().size()-1];
                os<<" | ";
        });
        };*/
        return from_fun, to_fun;
    }

/*
1) put a counter for rows in the first row so it doesn't look weird
2) make sure it's ok
3) push
4) run things and see how it goes
5) be done with csv and start r things
*/
    void csv(string time){
        file.open("dbg_"+time+".csv");
        file<<"Time,Count,From,To"<<"\n";

        string traits;
        for(auto vertex : this->get_all_vertices()){
            for(auto adj : this->get_value(vertex).get_adj_list()){
                traits+=time+",";
                traits+=std::to_string(this->get_value(vertex).get_sequence_count())+",";
                traits+=vertex+",";
                traits+=adj+",";
                file << traits << "\n";
                traits = "";
            }
        }
    }

    size_t seq_count(){

    }

    /**
     * @return string respresenting next sequence in graph
     */
    int from(){
        for(auto vertex : this->get_all_vertices()){
            if(mVertices[vertex].get_visitor_flag() <= mVertices[vertex].get_sequence_count()){
                mVertices[vertex].change_visitor_flag(mVertices[vertex].get_visitor_flag()+1);
                return std::stoi(vertex);
            }
        }
    }


    string to(){
        for(auto vertex : this->get_all_vertices()){
            if(mVertices[vertex].get_visitor_flag() < mVertices[vertex].get_sequence_count()){
                mVertices[vertex].change_visitor_flag(mVertices[vertex].get_visitor_flag()+1);
                return vertex;
            }
        }
    }

};

#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNGRAPH_H