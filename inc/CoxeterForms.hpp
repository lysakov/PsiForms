#ifndef COXETER_FORMS
#define COXETER_FORMS

#include <memory>
#include <fplll.h>
#include "generator.hpp"

enum CoxeterFormCode {
    FORM_NULL = 0,
    FORM_A = 1,
    FORM_A_ASTR = 2,
    FORM_D = 4,
    FORM_D_ASTR = 5,
    FORM_E = 6,
    FORM_E7_ASTR = 7,
    FORM_E8_ASTR = 8,
    FORM_E9_ASTR = 9
};

/* @brief increment operator for CoxeterFormCode
 * @param code - object of class CoxeterFormCode to be increased */
CoxeterFormCode operator++(CoxeterFormCode &code) noexcept;

/* @brief prints code of Coxeter form to output stream str
 * @returns output stream str */
std::ostream& operator<<(std::ostream &str, const CoxeterFormCode &code);

/* Factory, which produces Coxeter forms */
class CoxeterFormGenerator : public IGenerator
{

  public:
    /* @brief Creates factory, which produces Coxeter forms.
     * @param genList - list of Coxeter forms codes to be generated
     * @param dim - list of dimensions in which new forms will be generated */
    CoxeterFormGenerator(const std::vector<CoxeterFormCode> &genList, const std::vector<int> &dim) noexcept;

    /* @brief Method getForm() returns pointer to the next Coxeter form. */
    virtual ZZ_mat<mpz_t> getForm() override;

    /* @brief Method checks whether all Coxeter forms are enumerated */
    bool empty() const noexcept override;

    /* @brief method counts the value of delta function
     * @param form - code of the Coxeter form
     * @param dim - form dimension
     * @returns value of delta function */
    static unsigned long delta(CoxeterFormCode form, unsigned long dim) noexcept;

  private:
    /* List of Coxeter forms codes to be generated. */
    std::vector<CoxeterFormCode> _genList;

    /* List of dimensions in which new forms will be generated. */
    std::vector<int> _dim;

    /* _extend(...) adds one colum and row to matrix A, all set to zero. */
    void _extend(ZZ_mat<mpz_t> &A) const;

    /* Next set of methods creates given Coxeter form by it's dimension */
    ZZ_mat<mpz_t> _getA(unsigned long n) const noexcept;

    ZZ_mat<mpz_t> _getAAstr(unsigned long n) const noexcept;

    ZZ_mat<mpz_t> _getD(unsigned long n) const noexcept;

    ZZ_mat<mpz_t> _getDAstr(unsigned long n) const noexcept;

    ZZ_mat<mpz_t> _getE(unsigned long n) const noexcept;

    ZZ_mat<mpz_t> _getE7(unsigned long n) const noexcept;

    ZZ_mat<mpz_t> _getE8(unsigned long n) const noexcept;

    ZZ_mat<mpz_t> _getE9(unsigned long n) const noexcept;

}; 

#endif