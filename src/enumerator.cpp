#include "enumerator.hpp"


/******************************
* BasisSVP method realization *
*******************************/



std::vector<Z_NR<mpz_t>> BasisSVP::getShortestVector(ZZ_mat<mpz_t> &A)
{

    decltype(getShortestVector(A)) res;
    shortest_vector(A, res, SVPM_PROVED);
    return _transformCoord(res, A);

}

std::vector<std::vector<Z_NR<mpz_t>>> BasisSVP::getAllShortestVectors(ZZ_mat<mpz_t> &A)
{

    decltype(getAllShortestVectors(A)) res;
    return res;

}

/* Compiler stub */
Z_NR<mpz_t> BasisSVP::getSquaredLength(ZZ_mat<mpz_t> &A)
{
    
    auto shortestVector = getShortestVector(A);

    return squaredNorm(shortestVector); 

}

/* Compiler stub */
unsigned long BasisSVP::getShortestVectorsNum(ZZ_mat<mpz_t> &A)
{

    A = A;

    return 0;

}

std::vector<Z_NR<mpz_t>> BasisSVP::_transformCoord(const std::vector<Z_NR<mpz_t>> &coord, const ZZ_mat<mpz_t> &A) noexcept
{

    decltype(_transformCoord(coord, A)) res;
    vector_matrix_product(res, coord, A);
    return res;

}

/*******************************
 * GramSVP methods realization *
 *******************************/

std::vector<Z_NR<mpz_t>> GramSVP::getShortestVector(ZZ_mat<mpz_t> &A)
{

    //_LLL(A);

    ZZ_mat<mpz_t> U;
    ZZ_mat<mpz_t> UT;
    MatGSOGram<Z_NR<mpz_t>, FP_NR<mpfr_t>> G(A, U, UT, 1);
    G.update_gso();

    //std::cout << G.get_r_matrix() << "\n!!!!!!\n" << std::endl;

    decltype(getShortestVector(A)) res;
    shortest_vector(G, res, SVPM_PROVED);
    return res;
}

std::vector<std::vector<Z_NR<mpz_t>>> GramSVP::getAllShortestVectors(ZZ_mat<mpz_t> &A)
{

    //_LLL(A);

    ZZ_mat<mpz_t> U;
    ZZ_mat<mpz_t> UT;
    MatGSOGram<Z_NR<mpz_t>, FP_NR<mpfr_t>> G(A, U, UT, 1);
    G.update_gso();

    decltype(getAllShortestVectors(A)) res;
    std::vector<double> solLength;
    shortest_vectors(G, res, solLength, 3000, SVPM_PROVED, 0);
    return res;

}

Z_NR<mpz_t> GramSVP::getSquaredLength(ZZ_mat<mpz_t> &A)
{
    
    auto shortestVector = getShortestVector(A);
    decltype(shortestVector) vectorMatrixProd;

    vector_matrix_product(vectorMatrixProd, shortestVector, A);

    return dot(shortestVector, vectorMatrixProd); 

}

unsigned long GramSVP::getShortestVectorsNum(ZZ_mat<mpz_t> &A)
{

    auto vectorList = getAllShortestVectors(A);

    return vectorList.size();

}

ZZ_mat<mpz_t>& GramSVP::_LLL(ZZ_mat<mpz_t> &A)
{

    ZZ_mat<mpz_t> U;
    ZZ_mat<mpz_t> UT;

    MatGSOGram<Z_NR<mpz_t>, FP_NR<mpfr_t>> G(A, U, UT, 1);
    G.update_gso();

    LLLReduction<Z_NR<mpz_t>, FP_NR<mpfr_t>> LLLObjgram(G, LLL_DEF_DELTA, LLL_DEF_ETA, 0);
    LLLObjgram.lll();

    return A;

}

/*********************************
 * Enumerator method realization *
 *********************************/

Enumerator::Enumerator(const ZZ_mat<mpz_t> &A, const std::shared_ptr<ISVPAlg> &svpAlg) : _A(A), _svpAlg(svpAlg)
{}

std::vector<Z_NR<mpz_t>> Enumerator::getShortestVector()
{

    return _svpAlg->getShortestVector(_A);

}

std::vector<std::vector<Z_NR<mpz_t>>> Enumerator::getAllShortestVectors()
{

    return _svpAlg->getAllShortestVectors(_A);

}

Z_NR<mpz_t> Enumerator::getSquaredLength()
{

    return _svpAlg->getSquaredLength(_A);

}

unsigned long Enumerator::getShortestVectorsNum()
{

    return _svpAlg->getShortestVectorsNum(_A);

}