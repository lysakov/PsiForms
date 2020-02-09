#include "CoxeterForms.hpp"



/*******************************************
 * CoxeterFormCode's operators realization *
 *******************************************/



CoxeterFormCode operator++(CoxeterFormCode &code) noexcept
{

    switch (code) {
    case FORM_A:
        code = FORM_A_ASTR;
        break;

    case FORM_A_ASTR:
        code = FORM_D;
        break;

    case FORM_D:
        code = FORM_D_ASTR;
        break;

    case FORM_D_ASTR:
        code = FORM_E;
        break;

    case FORM_E:
        code = FORM_E7_ASTR;
        break;

    case FORM_E7_ASTR:
        code = FORM_E8_ASTR;
        break;

    case FORM_E8_ASTR:
        code = FORM_E9_ASTR;
        break;

    case FORM_E9_ASTR:
        code = FORM_A;
        break;

    default:
        code = FORM_E;
        break;
    }

    return code;

}

std::ostream& operator<<(std::ostream &str, const CoxeterFormCode &code)
{

    switch (code) {
    case FORM_A:
        str << "A";
        break;

    case FORM_A_ASTR:
        str << "A*";
        break;

    case FORM_D:
        str << "D";
        break;

    case FORM_D_ASTR:
        str << "D*";
        break;

    case FORM_E:
        str << "E";
        break;

    case FORM_E7_ASTR:
        str << "E*_7";
        break;

    case FORM_E8_ASTR:
        str << "E*_8";
        break;

    case FORM_E9_ASTR:
        str << "E*_9";
        break;

    default:
        str << "NULL";
        break;
    }

    return str;

}



/**********************************************
 * CoxeterFormGenerator's methods realization *
 **********************************************/ 



CoxeterFormGenerator::CoxeterFormGenerator(const std::vector<CoxeterFormCode> &genList, 
const std::vector<int> &dim) noexcept: _genList(genList), _dim(dim)
{}

ZZ_mat<mpz_t> CoxeterFormGenerator::getForm()
{

    if (_genList.empty()) {
        return _getA(1);
    }

    auto code = _genList.front();
    _genList.erase(_genList.begin());

    auto dim = _dim.front();
    _dim.erase(_dim.begin());

    switch(code) {
    case FORM_A:
        return _getA(dim);
    
    case FORM_A_ASTR:
        return _getAAstr(dim);

    case FORM_D:
        return _getD(dim);

    case FORM_D_ASTR:
        return _getDAstr(dim);

    case FORM_E:
        return _getE(dim);

    case FORM_E7_ASTR:
        return _getE7(dim);

    case FORM_E8_ASTR:
        return _getE8(dim);

    case FORM_E9_ASTR:
        return _getE9(dim);

    default:
        return _getA(1);
    }

}

bool CoxeterFormGenerator::empty() const noexcept
{

    return _genList.empty();

}

void CoxeterFormGenerator::_extend(ZZ_mat<mpz_t> &A) const
{

    A.resize(A.get_rows() + 1, A.get_cols() + 1);

    unsigned long dim = A.get_cols();

    for (unsigned long i = 0; i < dim; ++i) {

        A[i][dim - 1] = 0;
        A[dim - 1][i] = 0;

    }

}

/* A_1(x) = x^2,
 * A_k(x_1,..., x_k) = A_{k - 1}(x_1,...,x_{k - 1}) - x_{k - 1} * x_k + (x_k)^2, k >= 2 */
ZZ_mat<mpz_t> CoxeterFormGenerator::_getA(unsigned long n) const noexcept
{

    ZZ_mat<mpz_t> result;
    result.resize(n, n);

    for (unsigned long i = 0; i < n; ++i) {
        for (unsigned long j = 0; j < n; ++j) {
            switch (i - j) {
            case -1:
                result[i][j] = -1;
                break;

            case 0:
                result[i][j] = 2;
                break;

            case 1:
                result[i][j] = -1;
                break;

            default:
                result[i][j] = 0;
                break;
            }
        }
    }

    return result;

}


/* A_{k}^{*}(x_1,..., x_k) = A_{k - 1}(x_1,...,x_{k - 1}) - x_1 * x_k, k >= 2 */
ZZ_mat<mpz_t> CoxeterFormGenerator::_getAAstr(unsigned long n) const noexcept
{

    auto result = _getA(n - 1);
    _extend(result);

    result[0][n - 1] = -1;
    result[n - 1][0] = -1;

    return result;

}

/* D_k(x_1,..., x_k) = A_{k - 1}(x_1,..., x_{k - 1}) - x_2 * x_k + (x_k)^2, k >= 4 */
ZZ_mat<mpz_t> CoxeterFormGenerator::_getD(unsigned long n) const noexcept
{

    auto result = _getA(n - 1);
    _extend(result);

    result[n - 1][n - 1] = 2;
    result[1][n - 1] = -1;
    result[n - 1][1] = -1;

    return result;

}

/* D_{k}^{*}(x_1,..., x_k) = D_{k - 1}(x_1,..., x_{k - 1}) - x_{k - 2} * x_k + (x_k)^2, k >= 5 */
ZZ_mat<mpz_t> CoxeterFormGenerator::_getDAstr(unsigned long n) const noexcept
{

    auto result = _getD(n - 1);
    _extend(result);

    result[n - 1][n - 1] = 2;
    result[n - 3][n - 1] = -1;
    result[n - 1][n - 3] = -1;

    return result;

}

/* E_k(x_1,..., x_k) = A_{k - 1}(x_1,..., x_{k - 1}) - x_3 * x_k + (x_k)^2, k = 6, 7, 8 */
ZZ_mat<mpz_t> CoxeterFormGenerator::_getE(unsigned long n) const noexcept
{

    auto result = _getA(n - 1);
    _extend(result);

    result[n - 1][n - 1] = 2;
    result[2][n - 1] = -1;
    result[n - 1][2] = -1;

    return result;

}

/* E_{7}^{*}(x_1,..., x_7) = E_6(x_1,..., x_6) - x_6 * x_7 + (x_7)^2, k = 7 */
ZZ_mat<mpz_t> CoxeterFormGenerator::_getE7(unsigned long n) const noexcept
{

    n = n; // Compiler stub

    auto result = _getE(6);
    _extend(result);

    result[6][6] = 2;
    result[5][6] = -1;
    result[6][5] = -1;

    return result;

}

/* E_{8}^{*}(x_1,..., x_8) = E_7(x_1,..., x_7) - x_1 * x_8 + (x_8)^2, k = 8 */
ZZ_mat<mpz_t> CoxeterFormGenerator::_getE8(unsigned long n) const noexcept
{

    n = n; // Compiler stub

    auto result = _getE(7);
    _extend(result);

    result[7][7] = 2;
    result[0][7] = -1;
    result[7][0] = -1;

    return result;

}

/* E_{9}^{*}(x_1,..., x_9) = E_8(x_1,..., x_8) - x_8 * x_9 + (x_9)^2, k = 9 */
ZZ_mat<mpz_t> CoxeterFormGenerator::_getE9(unsigned long n) const noexcept
{

    n = n; // Compiler stub

    auto result = _getE(8);
    _extend(result);

    result[8][8] = 2;
    result[7][8] = -1;
    result[8][7] = -1;

    return result;

}