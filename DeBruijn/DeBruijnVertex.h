/**
 * @file DeBruijnVertex.h
 * @author Anna Catenacci
 *
 * A class that describes the DeBruijn vertex object and the
 * genetic ID string associated with it.
 */

#ifndef PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNVERTEX_H
#define PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNVERTEX_H

#include <string>
#include <vector>

using std::string; using std::vector;

class DeBruijnVertex {
private:

    /// Identifying vertex ID
    string mKmer;

    /// Sequential value of when the vertex was added to the graph
    int mOrder;

public:
    DeBruijnVertex()=default;
    DeBruijnVertex(string k) : mKmer(k) {};
    ~DeBruijnVertex()=default;

    /**
     * Get the kmer ID
     * @return string representing the vertex ID
     */
    string get_kmer() const {return mKmer; } ;

    /**
     * Get the sequential order value
     * @return True if this vertex is the beginning of the graph
     */
    int get_order() const {return mOrder; } ;

    /**
     * Set the sequential order value to true
     */
    void set_start(int num) {mOrder = num; } ;

    bool operator<(const DeBruijnVertex& vertex) const noexcept
    {
        return this->mOrder < vertex.get_order();
    }

    bool operator==(const DeBruijnVertex& vertex) const noexcept
    {
        return this->mKmer == vertex.get_kmer();
    }
};


#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNVERTEX_H
