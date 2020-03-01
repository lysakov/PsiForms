#include "ParallelWrapper.hpp"
#include "PsiForms.hpp"

ParallelWrapper::ParallelWrapper(std::shared_ptr<IGenerator> &generator, std::shared_ptr<ISVPAlg> &SVPAlg,
unsigned long threadNum) : _generator(generator), _SVPAlg(SVPAlg), _threadNum(threadNum)
{}

ParallelWrapper::ParallelWrapper(std::shared_ptr<IGenerator> &&generator, std::shared_ptr<ISVPAlg> &&SVPAlg,
unsigned long threadNum) : _generator(generator), _SVPAlg(SVPAlg), _threadNum(threadNum)
{}

unsigned long ParallelWrapper::execute()
{

    unsigned long maxVal = 0;

    std::vector<std::thread> threadsVector;
    //Logger::writeLine(std::to_string(_threadNum) + " threds were initiated.");

    auto wrapper = [&]() {
        _run(maxVal);
    };

    for (unsigned long i = 0; i < _threadNum; ++i) {
        threadsVector.push_back(std::thread(wrapper));
    }

    for (auto &thread : threadsVector) {
        thread.join();
    }

    return maxVal;

}

void ParallelWrapper::_run(unsigned long &maxVal)
{

    while (!_generator->empty()) {
        _mtx.lock();
        ZZ_mat<mpz_t> mat = _generator->getForm();
        //std::cout << mat << "\n *** \n";
        //std::cout << dynamic_cast<PsiFormGenerator*>(_generator.get())->count() << std::endl;
        Enumerator enumerator(mat, _SVPAlg->copy());
        //std::cout << form << std::endl;
        //std::cout << this_thread::get_id() << std::endl;
        _mtx.unlock();

        auto val = enumerator.getShortestVectorsNum();
        
        if (Logger::getLevel() != LOG_DEBUG) {
            Logger::info("- " + std::to_string(val));
        } else {
            Logger::debug("\nSolution number: " + std::to_string(val));
        }
        
        if (val > maxVal) {
            std::lock_guard<std::mutex> lock(_mtx);
            maxVal = val;
        }
    }

}
