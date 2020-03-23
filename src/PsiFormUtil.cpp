#include <iostream>
#include <map>
#include "enumerator.hpp"
#include "PsiForms.hpp"
#include "Parser.hpp"

int main(int argc, const char **argv)
{

    // std::vector<decltype(Enumerator(ZZ_mat<mpz_t>(), std::make_shared<GramSVP>()).getAllShortestVectors())> set;

    for (int i = 1; i < argc; ++i) {
        try {
            Parser parser(argv[i]);

            Enumerator enumerator(PsiFormGenerator::getForm(parser.getState(), parser.getPartition()),
            std::make_shared<GramSVP>());

            auto svpSol = enumerator.getAllShortestVectors();
            //set.push_back(svpSol);
            //auto weightPtr = createWeightSet(svpSol);

            std::cout << argv[i] << ": " << svpSol.size() << std::endl;
            /*for (const auto &weight : *weightPtr) {
                std::cout << weight.first << " - " << weight.second << std::endl;
            }*/

            /*int cnt = 1;
            for (auto &sol : svpSol) {
                std::cout << std::to_string(cnt) << ") " << sol << std::endl;
                ++cnt;
            }*/

        } catch(const std::exception &e) {
            std::cerr << "ERROR: " << argv[i]<< ": " << e.what() << std::endl;
        }
    }

    /* for (size_t i = 0; i < set.size(); ++i) {
        for (size_t j = 0; j < set.size(); ++j) {
            if (i != j) {
                for (const auto &elem : set[i]) {
                    if (!std::count(set[j].begin(), set[j].end(), elem)) {
                        std::cout << j << " not include " << i << ' ' << elem << std::endl;
                        break;
                    } 
                }
            }
        }
    } */

    return 0;

}