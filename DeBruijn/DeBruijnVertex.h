/**
 * @file DeBruijnVertex.h
 * @author Anna Catenacci
 *
 *
 */

#ifndef PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNVERTEX_H
#define PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNVERTEX_H

#include <string>
#include <vector>

using std::string; using std::vector;

class DeBruijnVertex {
private:
    string mKmer;

public:
    DeBruijnVertex()=default;
    DeBruijnVertex(string k) : mKmer(k) {};
    ~DeBruijnVertex()=default;

    ///getters
    string get_kmer() const {return mKmer; } ;

    bool operator<(const DeBruijnVertex& vertex) const noexcept
    {
        return this->mKmer < vertex.get_kmer();
    }

    bool operator==(const DeBruijnVertex& vertex) const noexcept
    {
        return this->mKmer == vertex.get_kmer();
    }
};


#endif //PANGENOMES_FOR_EVOLUTIONARY_COMPUTATION_DEBRUIJNVERTEX_H
