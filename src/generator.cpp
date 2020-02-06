#include "generator.hpp"

/*ZZ_mat<mpz_t> Generator::KZForm(int k) noexcept
{

    ZZ_mat<mpz_t> res(k, k);

    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            if (i != j) {
                res[i][j] = 1;
            }
            else {
                res[i][j] = 2;
            }
        }
    }

    return res;

}

ZZ_mat<mpz_t> Generator::AnzinForm(int n) noexcept
{

    ZZ_mat<mpz_t> res(n, n);
    int k = n / 2;

    if (n % 2 == 0) {
        --k;
    }

    res[0][0] = 2 * k;
    res[0][1] = 1;
    res[1][0] = 1;
    res[1][1] = 2 * k;

    for (int i = 2; i < n; ++i) {
        res[0][i] = k;
        res[1][i] = k;
        res[i][0] = k;
        res[i][1] = k;
    }

    for (int i = 2; i < n; ++i) {
        for (int j = 2; j < n; ++j) {
            if (i != j) {
                res[i][j] = k - 1;
            }
            else {
                res[i][j] = 2 * k;
            }
        }
    }

    return res;

}*/