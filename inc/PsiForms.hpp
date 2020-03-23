#ifndef PSIFORMS
#define PSIFORMS

#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <mutex>

#include "generator.hpp"
#include "CoxeterForms.hpp"
#include "iterator.hpp"
#include "Logger.hpp"
#include "utils.hpp"

/* Factory, which produces psi forms. */
class PsiFormGenerator : public IGenerator
{

  public:
    /* @brief Creates factory, which produses psi forms in n - dimensional space. 
     * @param n - space dimension */
    explicit PsiFormGenerator(int n);

    /* @brief Method getForm() returns next psi form.
     * @returns psi form. */
    virtual ZZ_mat<mpz_t> getForm() override;

    /** 
     * @brief Method returns perfect psi form.
     * @param state - vector of Coxeter forms
     * dim - dimension of the given Coxeter forms
     * @returns psi form. 
     */
    static ZZ_mat<mpz_t> getPerfectForm(const std::vector<CoxeterFormCode> &state, const std::vector<int> &dim);

    /** 
     * @brief Method returns psi form.
     * @param state - vector of Coxeter forms
     * dim - dimension of the given Coxeter forms
     * @returns psi form. 
     */
    static ZZ_mat<mpz_t> getForm(const std::vector<CoxeterFormCode> &state, const std::vector<int> &dim);

    /* @brief Method checks whether all psi forms are enumerated. *
     * @returns true, if all psi forms in given dimension are not enumerated. */
    bool empty() const noexcept override;

    /* @brief method counts number of generated psi forms
     * @returns number of created psi forms */
    unsigned long long count() const noexcept;

    /* _isValid(...) checks whether psi form is valid*/
    static bool _isValid(const std::vector<CoxeterFormCode> &state, const std::vector<int> &dim) noexcept;

    /* _sufficiency(...) checks the sufficiency condition of psi form */
    static bool _sufficiency(const std::vector<CoxeterFormCode> &state, const std::vector<int> &dim) noexcept;

    /* _necessity(...) checks the necessity condition of psi form */
    static bool _necessity(const std::vector<CoxeterFormCode> &state, const std::vector<int> &dim) noexcept;

    /**
     * @brief excludes last varialble from the form
     * @param A - matrix of the form
     * @return modified matrix
     */
    static ZZ_mat<mpz_t> excludeVar(ZZ_mat<mpz_t> A) noexcept;

    void test();

  private:
    /* TODO: make PsiFormIterator inner class of PsiFormStore */
    /* Class PsiFormIterator is used for iterating over PsiFormStore. */
    class PsiFormIterator
    {

      public:
        explicit PsiFormIterator(const std::vector<int> *partition) noexcept;

        PsiFormIterator(const std::vector<int> *partition, 
        const std::vector<CoxeterFormCode> &state) noexcept;

        ZZ_mat<mpz_t> operator*() const noexcept;

        PsiFormIterator& operator++() noexcept;
        
        bool operator==(const PsiFormIterator &iter) const noexcept;

        bool operator!=(const PsiFormIterator &iter) const noexcept;

        /* excludeVar(...) - excludes n - th  variable from form */
        static ZZ_mat<mpz_t>& excludeVar(ZZ_mat<mpz_t> &A, unsigned long n) noexcept;

        /* _glew(...) - creates block diagonal matrix, where first block corresponds to A and
        * second - to B. */
        static ZZ_mat<mpz_t>& glew(ZZ_mat<mpz_t> &A, const ZZ_mat<mpz_t> &B) noexcept;



      private:

        std::vector<CoxeterFormCode> _state;

        const std::vector<int> *_partition;

        /* Method declared friend for logging purpouses */
        friend ZZ_mat<mpz_t> PsiFormGenerator::getForm();

        /* _end = true, if iterator points to the end of store */
        bool _end = false;

        /* Increases i-th component of vector _state */
        void _increaseComponet(unsigned long i);

        /* _isPositive(...) checks whether matrix A is positive defined. */
        bool _isPositive(ZZ_mat<mpz_t> A) const noexcept;

    };

    /* Class PsiFormStore stores all psi forms, coresponding to the given partition. */
    class PsiFormStore : public IIterable<PsiFormIterator>
    {

      public:
        explicit PsiFormStore(const std::vector<int> &partition) noexcept;

        virtual PsiFormIterator begin() const noexcept override;

        virtual PsiFormIterator end() const noexcept override;

      private:
        std::vector<int> _partition;

    };

    /* Space dimension */
    int _n = 0;

    /* _store stores all psi forms corresponding to the given partition */
    PsiFormStore _store;

    /* _iter points to the current psi form in _store */
    PsiFormIterator _iter;

    /* Psi form counter */
    unsigned long long _cnt = 1;

    /* Thread safety */
    mutable std::mutex _mtx;

    /* Partition stack */
    std::vector<std::vector<int>> _partitions;

    /* _findMin(...) - returns the position of the smallest element in vector partition,
     * not counting the last one. This method is used for generating the partition stack. */
    unsigned long _findMin(const std::vector<int> &partition) const noexcept;

    /* _decompose(...) - decomposes the sum of all elements on positions after the minPos 
     * position into the sum of 1. This method is used for generating the partition stack. */
    void _decompose(std::vector<int> &partition, unsigned long minPos) const noexcept;

    /* _initStack() - initializes the partittion stack. */
    void _initStack() noexcept;

    /* _load() loads next partition in _store and sets _iter */
    void _load();

};



#endif