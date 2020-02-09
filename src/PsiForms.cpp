#include "PsiForms.hpp"



/*****************************************
 * PsiFormGenerator's method realization *
 *****************************************/



PsiFormGenerator::PsiFormGenerator(int n) : _n(n), _store({}), _iter(nullptr)
{

    if (n <= 0) {
        throw std::runtime_error("Impossible to generate psi forms in space with this dimension.");
    }

    _initStack();
    _load();

}

ZZ_mat<mpz_t> PsiFormGenerator::getForm()
{

    decltype(getForm()) result;

    Logger::writeLine("**************************");
    
    if (_iter != _store.end()) {
        result = *_iter;
        ++_cnt;

        Logger::writeLine("--------------------------");
        Logger::writeLine(*_iter._partition);
        Logger::writeLine(_iter._state);
        Logger::writeLine("--------------------------");

        ++_iter;
    }
    else {
        if (_partitions.empty()) {
            return result;
        }

        _load();

        result = *_iter;
        ++_cnt;

        Logger::writeLine("--------------------------");
        Logger::writeLine(*_iter._partition);
        Logger::writeLine(_iter._state);
        Logger::writeLine("--------------------------");

        ++_iter;
    }

    Logger::writeLine(result);
    Logger::writeLine("**************************");

    return result;

}

bool PsiFormGenerator::empty() const noexcept
{

    return _partitions.empty() && _iter == _store.end();

}

unsigned long long PsiFormGenerator::count() const noexcept
{

    return _cnt;

}

unsigned long PsiFormGenerator::_findMin(const std::vector<int> &partition) const noexcept
{

    if (partition.size() <= 1) {
        return 0;
    }

    unsigned long minPos = 0;
    for(unsigned long i = 0; i < partition.size() - 1; ++i) {
        if (partition[i] < partition[minPos]) {
            minPos = i;
        }
    }

    return minPos;

}

void PsiFormGenerator::_decompose(std::vector<int> &partition, unsigned long minPos) const noexcept
{

    unsigned long sum = 0;
    for (unsigned long i = minPos + 1; i < partition.size(); ++i) {
        sum += partition[i];
    }

    partition.resize(minPos + 1);

    for (unsigned long i = 0; i < sum; ++i) {
        partition.push_back(1);
    }

}

void PsiFormGenerator::_initStack() noexcept
{

    std::vector<int> partition;

    for(int i = 0; i < _n; ++i) {
        partition.push_back(1);
    }

    _partitions.push_back(partition);

    while(partition[0] != _n) {

        auto minPos = _findMin(partition);
        ++partition[minPos];
        --partition.back();

        if (partition.back() == 0) {
            partition.pop_back();
        }

        _decompose(partition, minPos);
        _partitions.push_back(partition);

    }

}

void PsiFormGenerator::_load()
{

    _store = PsiFormStore(_partitions.back());
    _partitions.pop_back();

    _iter = _store.begin();

}

void PsiFormGenerator::test()
{

    std::vector<int> partition = {4, 3, 5};
    PsiFormStore store(partition);

    for (auto psiForm : store) {
        std::cout << psiForm << std::endl;
        //std::cin.get();
    }

}



/****************************************
 * PsiFormIterator's method realization *
 ****************************************/



PsiFormGenerator::PsiFormIterator::PsiFormIterator(const std::vector<int> *partition) noexcept :
_partition(partition)
{

    if (partition == nullptr) {
        return;
    }
    
    for (unsigned long i = 0; i < partition->size(); ++i) {
        _state.push_back(FORM_A);
    }

}

PsiFormGenerator::PsiFormIterator::PsiFormIterator(const std::vector<int> *partition, 
const std::vector<CoxeterFormCode> &state) noexcept :
_state(state), _partition(partition)
{}

ZZ_mat<mpz_t> PsiFormGenerator::PsiFormIterator::operator*() const noexcept
{

    CoxeterFormGenerator generator(_state, *_partition);

    ZZ_mat<mpz_t> result;

    while (!generator.empty()) {
        _glew(result, generator.getForm());
    }

    return result;

}

PsiFormGenerator::PsiFormIterator& PsiFormGenerator::PsiFormIterator::operator++() noexcept
{

    _increaseComponet(0);

    if (_end) {
        for (auto &code : _state) {
            code = FORM_NULL;
        }
    }

    return *this;

}

bool PsiFormGenerator::PsiFormIterator::operator==(const PsiFormGenerator::PsiFormIterator &iter) const noexcept
{

    for (size_t i = 0; i < iter._state.size(); ++i) {
        if (iter._state[i] != this->_state[i]) {
            return false;
        }
    }

    return true;

}

bool PsiFormGenerator::PsiFormIterator::operator!=(const PsiFormGenerator::PsiFormIterator &iter) const noexcept
{

    return !((*this) == iter);

}

void PsiFormGenerator::PsiFormIterator::_increaseComponet(unsigned long i)
{

    ++_state[i];

    if (_partition->at(i) < _state[i]) {
        _state[i] = FORM_A;
    }

    /* E_{7}^{*}, E_{8}^{*} and E_{9}^{*} don't exist for dimension 6 */
    if (_partition->at(i) == 6 && _state[i] > FORM_E) {
        _state[i] = FORM_A;
    }

    /* E_{8}^{*} and E_{9}^{*} don't exist for dimension 7 */
    if (_partition->at(i) == 7 && _state[i] > FORM_E7_ASTR) {
        _state[i] = FORM_A;
    }

    /* E_{7}^{*} and E_{9}^{*} don't exist for dimension 8 */
    if (_partition->at(i) == 8) {
        if (_state[i] > FORM_E8_ASTR) {
            _state[i] = FORM_A;
        }

        if (_state[i] == FORM_E7_ASTR) {
            _state[i] = FORM_E8_ASTR;
        }
    }

    /* E, E_{7}^{*} and E_{8}^{*} don't exist for dimension 9 */
    if (_partition->at(i) == 9 && (_state[i] == FORM_E || 
        _state[i] == FORM_E7_ASTR || _state[i] == FORM_E8_ASTR)) {
            _state[i] = FORM_E9_ASTR;
    }
    
    /* E, E_{7}^{*}, E_{8}^{*} and E_{9}^{*} don't exist for dimensions bigger then 9 */
    if (_partition->at(i) > 9 && _state[i] > FORM_D_ASTR) {
        _state[i] = FORM_A;
    }

    if (_state[i] == FORM_A) {
        if  (i < _state.size() - 1) {
            _increaseComponet(i + 1);
        }
        else {
            _end = true;
        }
    }

}

void PsiFormGenerator::PsiFormIterator::_glew(ZZ_mat<mpz_t> &A, const ZZ_mat<mpz_t> &B) const noexcept
{

    unsigned long oldRowsNum = A.get_rows();
    unsigned long oldColsNum = A.get_cols();

    A.resize(oldRowsNum + B.get_rows(), oldColsNum + B.get_cols());

    /* TODO: fill elements outside blocks A and B with zeroes */

    for (unsigned long i = 0; i < static_cast<unsigned long>(B.get_rows()); ++i) {
        for (unsigned long j = 0; j < static_cast<unsigned long>(B.get_cols()); ++j) {
            A[oldRowsNum + i][oldColsNum + j] = B[i][j];
        }
    }

}



/*********************************
 * PsiForm's methods realization *
 *********************************/


PsiFormGenerator::PsiFormStore::PsiFormStore(const std::vector<int> &partition) noexcept :
_partition(partition)
{}

PsiFormGenerator::PsiFormIterator PsiFormGenerator::PsiFormStore::begin() const noexcept
{

    return PsiFormIterator(const_cast<const std::vector<int>*>(&_partition));

}

PsiFormGenerator::PsiFormIterator PsiFormGenerator::PsiFormStore::end() const noexcept
{

    std::vector<CoxeterFormCode> state(_partition.size());

    for (auto &code : state) {
        code = FORM_NULL;
    }

    return PsiFormIterator(const_cast<const std::vector<int>*>(&_partition), state);

}