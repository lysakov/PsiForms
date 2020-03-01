#ifndef ENUMERATOR
#define ENUMERATOR

#include <fplll.h>
#include <memory>
#include <iostream>
#include "utils.hpp"

class ISVPAlg
{

  public:
    virtual std::vector<Z_NR<mpz_t>> getShortestVector(ZZ_mat<mpz_t> &A) = 0;
    virtual std::vector<std::vector<Z_NR<mpz_t>>> getAllShortestVectors(ZZ_mat<mpz_t> &A) = 0;
    virtual Z_NR<mpz_t> getSquaredLength(ZZ_mat<mpz_t> &A) = 0;
    virtual unsigned long getShortestVectorsNum(ZZ_mat<mpz_t> &A) = 0;
    virtual std::shared_ptr<ISVPAlg> copy() const = 0;

};

class BasisSVP : public ISVPAlg
{

  public:
    BasisSVP() {}
    virtual std::vector<Z_NR<mpz_t>> getShortestVector(ZZ_mat<mpz_t> &A) override;
    virtual std::vector<std::vector<Z_NR<mpz_t>>> getAllShortestVectors(ZZ_mat<mpz_t> &A) override;
    virtual Z_NR<mpz_t> getSquaredLength(ZZ_mat<mpz_t> &A) override;
    virtual unsigned long getShortestVectorsNum(ZZ_mat<mpz_t> &A) override;
    virtual std::shared_ptr<ISVPAlg> copy() const override;

  private:
    std::vector<Z_NR<mpz_t>> _transformCoord(const std::vector<Z_NR<mpz_t>> &coord, const ZZ_mat<mpz_t> &A) noexcept;

};

class GramSVP : public ISVPAlg
{

  public:
    GramSVP() {}
    virtual std::vector<Z_NR<mpz_t>> getShortestVector(ZZ_mat<mpz_t> &A) override;
    virtual std::vector<std::vector<Z_NR<mpz_t>>> getAllShortestVectors(ZZ_mat<mpz_t> &A) override;
    virtual Z_NR<mpz_t> getSquaredLength(ZZ_mat<mpz_t> &A) override;
    virtual unsigned long getShortestVectorsNum(ZZ_mat<mpz_t> &A) override;
    virtual std::shared_ptr<ISVPAlg> copy() const override;

  private:
    ZZ_mat<mpz_t>& _LLL(ZZ_mat<mpz_t> &A);

};

class Enumerator
{

  public:
    Enumerator(const ZZ_mat<mpz_t> &A, std::shared_ptr<ISVPAlg> &&svpAlg);
    std::vector<Z_NR<mpz_t>> getShortestVector();
    std::vector<std::vector<Z_NR<mpz_t>>> getAllShortestVectors();
    Z_NR<mpz_t> getSquaredLength();
    unsigned long getShortestVectorsNum();

  private:
    ZZ_mat<mpz_t> _A;
    std::shared_ptr<ISVPAlg> _svpAlg;

};

#endif