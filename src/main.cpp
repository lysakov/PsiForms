#include <iostream>
#include <fplll.h>
#include <mutex>
#include <chrono>

#include "PsiForms.hpp"
#include "generator.hpp"
#include "enumerator.hpp"
#include "Logger.hpp"
#include "utils.hpp"
#include "ParallelWrapper.hpp"

int main()
{

    //Logger::setLogFile("stdout");
    Logger::setLevel(LOG_INFO);

    CoxeterFormGenerator generator({FORM_D, FORM_D, FORM_D, FORM_D, FORM_D, FORM_D}, {18, 19, 20, 21, 22, 23});

    while (!generator.empty()) {
        Enumerator enumerator(generator.getForm(), std::make_shared<GramSVP>());
        auto svpSols = enumerator.getAllShortestVectors();
        //for (const auto &sol : svpSols) {
            //std::cout << sol << std::endl;
            //std::cout << "*********************" << std::endl;
        //}
        auto weight = createWeightSet(svpSols);

        for (const auto &pair : *weight) {
            std::cout << pair.first << " - " << pair.second << std::endl;
        }
        std::cout << "*********************" << std::endl;
    }

    //CoxeterFormGenerator generator({FORM_A}, {8});
    //Enumerator enumerator(PsiFormGenerator::excludeVar(generator.getForm()), std::make_shared<GramSVP>());
    //auto svpSols = enumerator.getAllShortestVectors();
    //int cnt = 1;
    //for (const auto &sol : svpSols) {
        //std::cout << cnt << ") " << sol << std::endl;
        //++cnt;
        //std::cout << "*********************" << std::endl;
    //}
    

    for (int i = 3; i < 11; ++i) {
        auto start = std::chrono::high_resolution_clock::now();

        Logger::info("*************" + std::string(i > 8 ? "*" : ""));
        Logger::info("* n + 1 = " + std::to_string(i + 1) + " *");
        Logger::info("*************" + std::string(i > 8 ? "*" : ""));

        ParallelWrapper wrapper(std::make_shared<PsiFormGenerator>(i), std::make_shared<GramSVP>(), 1);
        std::cout << i << ")" << wrapper.execute() << std::endl;
        Logger::writeLine();

        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        std::cout << elapsed.count() << std::endl;
    }

    return 0;

}
