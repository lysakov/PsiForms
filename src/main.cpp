#include <iostream>
#include <fplll.h>

#include "PsiForms.hpp"
#include "generator.hpp"
#include "enumerator.hpp"
#include "Logger.hpp"

int main()
{

    PsiFormGenerator gen(15);

    Logger::init();

    while (!gen.empty()) {
        gen.getForm();
    }

    Logger::writeLine("==========================");
    Logger::writeLine("Total: " + std::to_string(gen.count()) + " psi forms");
    Logger::writeLine("==========================");

    return 0;

}