#ifndef ENUMERATOR
#define ENUMERATOR

#include <fplll.h>
#include <memory>
#include <iostream>

class ISVPAlg
{

  public:
    virtual std::vector<Z_NR<mpz_t>> getShortestVector(ZZ_mat<mpz_t> &A) = 0;
    virtual std::vector<std::vector<Z_NR<mpz_t>>> getAllShortestVectors(ZZ_mat<mpz_t> &A) = 0;

};

class BasisSVP : public ISVPAlg
{

  public:
    BasisSVP() {}
    virtual std::vector<Z_NR<mpz_t>> getShortestVector(ZZ_mat<mpz_t> &A) override;
    virtual std::vector<std::vector<Z_NR<mpz_t>>> getAllShortestVectors(ZZ_mat<mpz_t> &A) override;
  private:
    std::vector<Z_NR<mpz_t>> transformCoord(const std::vector<Z_NR<mpz_t>> &coord, const ZZ_mat<mpz_t> &A) noexcept;

};

class GramSVP : public ISVPAlg
{

  public:
    GramSVP() {}
    virtual std::vector<Z_NR<mpz_t>> getShortestVector(ZZ_mat<mpz_t> &A) override;
    virtual std::vector<std::vector<Z_NR<mpz_t>>> getAllShortestVectors(ZZ_mat<mpz_t> &A) override;

};

class Enumerator
{

  public:
    Enumerator(const ZZ_mat<mpz_t> &A, ISVPAlg &&svpAlg);
    std::vector<Z_NR<mpz_t>> getShortestVector();
    std::vector<std::vector<Z_NR<mpz_t>>> getAllShortestVectors();
  private:
    ZZ_mat<mpz_t> A;
    ISVPAlg *svpAlg;

};

#endif