#ifndef UTILS
#define UTILS

#include <fplll.h>

/* @brief addition of two integers
 * @param x - first integer
 * @param y - second integer
 * @returns - sum of x and y */
Z_NR<mpz_t> operator+(const Z_NR<mpz_t> &x, const Z_NR<mpz_t> &y);

/* @brief multiplication of unsigned long with integer
 * @param x - unsigned long integer
 * @param y - integer
 * @returns - product of x and y */
Z_NR<mpz_t> operator*(const unsigned long x, const Z_NR<mpz_t> &y);

/* @brief multiplication of unsigned long with integer
 * @param x - integer
 * @param y - unsigned long integer
 * @returns - product of x and y */
Z_NR<mpz_t> operator*(const Z_NR<mpz_t> &x, unsigned long y);

/* @brief multiplication of integers
 * @param x - first integer
 * @param y - second integer
 * @returns - product of x and y */
Z_NR<mpz_t> operator*(const Z_NR<mpz_t> &x, const Z_NR<mpz_t> &y);

/* @brief makes integer x negative
 * @param x - integer
 * @returns negative x */
Z_NR<mpz_t> operator-(const Z_NR<mpz_t> &x);

/* @brief addition of two matrices
 * @param A - first operand
 * @param B - second operand
 * @returns - sum of A and B */
ZZ_mat<mpz_t> operator+(const ZZ_mat<mpz_t> &A, const ZZ_mat<mpz_t> &B);

/* @brief dot product of two vectors
 * @param x - first vector
 * @param y - second vector
 * @returns - dot product of x and y */
Z_NR<mpz_t> dot(const std::vector<Z_NR<mpz_t>> &x, const std::vector<Z_NR<mpz_t>> &y);


/* @brief computes squared vector's norm
 * @param x - vector
 * @returns squared vector's norm */
Z_NR<mpz_t> squaredNorm(const std::vector<Z_NR<mpz_t>> &x);

#endif