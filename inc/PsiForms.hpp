#ifndef PSIFORMS
#define PSIFORMS

#include <iostream>
#include <stack>
#include <vector>
#include "generator.hpp"
#include "iterator.hpp"

/* Factory, which produces psi forms. */
class PsiFormGenerator : public IGenerator
{

  public:
    /* Creates factory, which produses psi forms in n - dimensional space. */
    explicit PsiFormGenerator(int n);

    /* Method getForm()returns pointer to the next psi form. */
    virtual std::shared_ptr<ZZ_mat<mpz_t>> getForm() override;

    void test();

  private:
    /* Space dimension */
    int _n = 0;

    /* Partition stack */
    std::vector<std::vector<int>> _partitions;

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

    friend CoxeterFormCode operator++(CoxeterFormCode &code) noexcept;

    /* _findMin(...) - returns the position of the smallest element in vector partition,
     * not counting the last one. This method is used for generating the partition stack. */
    unsigned long _findMin(const std::vector<int> &partition) const noexcept;

    /* _decompose(...) - decomposes the sum of all elements on positions after the minPos 
     * position into the sum of 1. This method is used for generating the partition stack. */
    void _decompose(std::vector<int> &partition, unsigned long minPos) const noexcept;

    /* _initStack() - initializes the partittion stack. */
    void _initStack() noexcept;

    /* Class PsiFormStore stores all psi forms, coresponding to the given partition. */
    class PsiFormStore;

    /* Class PsiFormIterator is used for iterating over PsiFormStore. */
    class PsiFormIterator;

};

class PsiFormGenerator::PsiFormIterator
{

  public:
    explicit PsiFormIterator(const std::vector<int> *partition) noexcept;

    PsiFormIterator(const std::vector<int> *partition, 
    const std::vector<PsiFormGenerator::CoxeterFormCode> &state) noexcept;

    ZZ_mat<mpz_t> operator*() const noexcept;

    PsiFormIterator& operator++() noexcept;
    
    bool operator==(const PsiFormIterator &iter) const noexcept;

    bool operator!=(const PsiFormIterator &iter) const noexcept;

    /* Increases i-th component of vector _state */
    void _increaseComponet(unsigned long i);

    std::vector<PsiFormGenerator::CoxeterFormCode> _state;

  private:

    const std::vector<int> *_partition;

};

class PsiFormGenerator::PsiFormStore : public IIterable<PsiFormGenerator::PsiFormIterator>
{

  public:
    PsiFormStore(const std::vector<int> *partition) noexcept;

    PsiFormIterator begin() noexcept;

    PsiFormIterator end() noexcept;

  private:
    const std::vector<int> *_partition;

};

#endif