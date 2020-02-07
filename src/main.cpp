#include <iostream>
#include <fplll.h>

#include "PsiForms.hpp"
#include "generator.hpp"
#include "enumerator.hpp"

int main()
{
    PsiFormGenerator gen(15);
    gen.test();
    CoxeterFormGenerator coxGen({FORM_A, FORM_D, FORM_E9_ASTR}, {1, 10, 9});

    for  (int i = 0; i < 3; ++i) {
        std::cout << coxGen.getForm() << std::endl;
    }

    return 0;
}