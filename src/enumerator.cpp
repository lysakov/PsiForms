#include "enumerator.hpp"

std::vector<Z_NR<mpz_t>> BasisSVP::getShortestVector(ZZ_mat<mpz_t> &A)
{
    decltype(getShortestVector(A)) res;
    shortest_vector(A, res, SVPM_PROVED);
    return transformCoord(res, A);
}

std::vector<std::vector<Z_NR<mpz_t>>> BasisSVP::getAllShortestVectors(ZZ_mat<mpz_t> &A)
{
    decltype(getAllShortestVectors(A)) res;
    return res;
}

std::vector<Z_NR<mpz_t>> BasisSVP::transformCoord(const std::vector<Z_NR<mpz_t>> &coord, const ZZ_mat<mpz_t> &A) noexcept
{
    decltype(transformCoord(coord, A)) res;
    vector_matrix_product(res, coord, A);
    return res;
}

std::vector<Z_NR<mpz_t>> GramSVP::getShortestVector(ZZ_mat<mpz_t> &A)
{
    ZZ_mat<mpz_t> U;
    ZZ_mat<mpz_t> UT;
    MatGSOGram<Z_NR<mpz_t>, FP_NR<mpfr_t>> G(A, U, UT, 1);
    G.update_gso();

    decltype(getShortestVector(A)) res;
    shortest_vector(G, res, SVPM_PROVED);
    return res;
}

std::vector<std::vector<Z_NR<mpz_t>>> GramSVP::getAllShortestVectors(ZZ_mat<mpz_t> &A)
{
    ZZ_mat<mpz_t> U;
    ZZ_mat<mpz_t> UT;
    MatGSOGram<Z_NR<mpz_t>, FP_NR<mpfr_t>> G(A, U, UT, 1);
    G.update_gso();

    decltype(getAllShortestVectors(A)) res;
    std::vector<double> solLength;
    shortest_vectors(G, res, solLength, 3000, SVPM_PROVED, 0);
    return res;
}

Enumerator::Enumerator(const ZZ_mat<mpz_t> &A, ISVPAlg &&svpAlg) : A(A), svpAlg(&svpAlg)
{}

std::vector<Z_NR<mpz_t>> Enumerator::getShortestVector()
{
    return svpAlg->getShortestVector(A);
}

std::vector<std::vector<Z_NR<mpz_t>>> Enumerator::getAllShortestVectors()
{
    return svpAlg->getAllShortestVectors(A);
}