#include <iostream>
#include <fplll.h>

#include "PsiForms.hpp"
#include "generator.hpp"
#include "enumerator.hpp"
#include "Logger.hpp"
#include "utils.hpp"

int main()
{

    PsiFormGenerator gen(15);

    Logger::init();

    while (!gen.empty()) {
        Enumerator enumerator(gen.getForm(), std::make_shared<GramSVP>());
        std::cout << enumerator.getShortestVectorsNum() << std::endl;
    }

    Logger::writeLine("**************************");
    Logger::writeLine("Total: " + std::to_string(gen.count()) + " psi forms");
    Logger::writeLine("**************************");

    return 0;

}