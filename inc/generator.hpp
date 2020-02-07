#ifndef GENERATOR
#define GENERATOR

#include <fplll.h>
#include <memory>

class IGenerator
{

  public:
    virtual ZZ_mat<mpz_t> getForm() = 0;

};

/*class KZFormGenerator : public IGenerator
{

  public:
    KZFormGenerator(int n) noexcept;
    virtual std::shared_ptr<ZZ_mat<mpz_t>> getForm() override;
    void setSize() noexcept;

};*/

#endif