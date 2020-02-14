#include "utils.hpp"

Z_NR<mpz_t> operator+(const Z_NR<mpz_t> &x, const Z_NR<mpz_t> &y)
{

    Z_NR<mpz_t> sum;
    sum.add(x, y);

    return sum;

}

Z_NR<mpz_t> operator*(const unsigned long x, const Z_NR<mpz_t> &y)
{

    Z_NR<mpz_t> product;
    product.mul_ui(y, x);

    return product;

}

Z_NR<mpz_t> operator*(const Z_NR<mpz_t> &x, const unsigned long y)
{

    return y * x;

}

Z_NR<mpz_t> operator*(const Z_NR<mpz_t> &x, const Z_NR<mpz_t> &y)
{

    Z_NR<mpz_t> product;
    product.mul(x, y);

    return product;

}

Z_NR<mpz_t> operator-(const Z_NR<mpz_t> &x)
{

    Z_NR<mpz_t> result;
    result.neg(x);

    return result;

}

ZZ_mat<mpz_t> operator+(const ZZ_mat<mpz_t> &A, const ZZ_mat<mpz_t> &B)
{

    if (A.get_cols() != B.get_cols() || A.get_rows() != B.get_rows()) {
        throw std::runtime_error("Matrix addition: dimension mesmatch");
    }

    decltype(A + B) result;
    result.resize(A.get_rows(), A.get_cols());

    for (int i = 0; i < A.get_rows(); ++i) {
        for (int j = 0; j < A.get_cols(); ++j) {
            result[i][j] = A[i][j] + B[i][j];
        }
    }

    return result;

}

Z_NR<mpz_t> dot(const std::vector<Z_NR<mpz_t>> &x, const std::vector<Z_NR<mpz_t>> &y)
{

    decltype(dot(x, y)) result;

    if (x.size() != y.size()) {
        throw std::runtime_error("dot: dimension mismatch");
    }

    for (unsigned long i = 0; i < x.size(); ++i) {
        result = result + x[i] * y[i];
    }

    return result;

}

Z_NR<mpz_t> squaredNorm(const std::vector<Z_NR<mpz_t>> &x)
{

    return dot(x, x);

}