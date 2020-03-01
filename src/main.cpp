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
