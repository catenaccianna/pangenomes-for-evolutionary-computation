# Pangenomes For Evolutionary Computation

This is a module created to be used with Version 2 of the Modular Agent-Based Evolver framework:

[MABE2](https://github.com/mercere99/MABE2.git)

## Documentation

**DeBruijn Graph**: This is a class that describes the framework of a DeBruijn Graph to store all living genomes in a population.  
**DeBruijn Value**: This is a class that describes the values and useful characteristics of each section of a piece of genetic information in the pangenome.

## Running Pangenomes
The Makefile for my code is in the pangenomes-for-evolutionary-computation directory. From here, you can run...  
**make clean** to clear the executable from your last run out of the way.  
**make test** to run the test.cpp file that I use to help me visualize what I want my graph to look like. In the test.cpp file, you can look through the cases and comment out whatever you don't want to look at.  
**make assert** to run the unit tests for DeBruijnGraph.hpp  
**make (as a default)** will run "make assert"  
**make assert_value** to run the unit tests for DeBruijnValue.hpp  
**make debug** [does not work yet]  
