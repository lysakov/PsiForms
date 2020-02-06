#include <iostream>
#include <fplll.h>

#include "PsiForms.hpp"
#include "generator.hpp"
#include "enumerator.hpp"

int main()
{
    PsiFormGenerator gen(15);
    gen.test();
    return 0;
}