#ifndef PARALLEL_WRAPPER
#define PARALLEL_WRAPPER

#include <memory>
#include <thread>
#include <mutex>
#include "generator.hpp"
#include "enumerator.hpp"
#include "Logger.hpp"

class ParallelWrapper
{

  public:
    /* @param generator - pointer to form generator
     * @param SVPAlg - SVP algorithm (basis/Gram)
     * @param threadNum - number of threads */
    ParallelWrapper(std::shared_ptr<IGenerator> &generator, std::shared_ptr<ISVPAlg> &SVPAlg, 
    unsigned long threadNum = std::thread::hardware_concurrency());

    /* @param generator - pointer to form generator
     * @param SVPAlg - SVP algorithm (basis/Gram)
     * @param threadNum - number of threads */
    ParallelWrapper(std::shared_ptr<IGenerator> &&generator, std::shared_ptr<ISVPAlg> &&SVPAlg,
    unsigned long threadNum = std::thread::hardware_concurrency());

    /* @brief counts maximum quantity of shortest vectors
     * @returns maximum quantity of shortest vectors between forms, created by generator */
    unsigned long execute();

  private:
    /* Parallilizer */
    void _run(unsigned long &maxVal);

    /* Pointer to generator */
    std::shared_ptr<IGenerator> _generator;

    /* Pointer to SVP algorithm */
    std::shared_ptr<ISVPAlg> _SVPAlg;

    /* Thread safety */
    std::mutex _mtx;

    /* Number of threads */
    unsigned long _threadNum;

};

#endif