#include "PsiForms.hpp"



/*****************************************
 * PsiFormGenerator's method realization *
 *****************************************/



PsiFormGenerator::CoxeterFormCode operator++(PsiFormGenerator::CoxeterFormCode &code) noexcept
{

    switch (code) {
    case PsiFormGenerator::FORM_A:
        code = PsiFormGenerator::FORM_A_ASTR;
        break;

    case PsiFormGenerator::FORM_A_ASTR:
        code = PsiFormGenerator::FORM_D;
        break;

    case PsiFormGenerator::FORM_D:
        code = PsiFormGenerator::FORM_D_ASTR;
        break;

    case PsiFormGenerator::FORM_D_ASTR:
        code = PsiFormGenerator::FORM_E;
        break;

    case PsiFormGenerator::FORM_E:
        code = PsiFormGenerator::FORM_E7_ASTR;
        break;

    case PsiFormGenerator::FORM_E7_ASTR:
        code = PsiFormGenerator::FORM_E8_ASTR;
        break;

    case PsiFormGenerator::FORM_E8_ASTR:
        code = PsiFormGenerator::FORM_E9_ASTR;
        break;

    case PsiFormGenerator::FORM_E9_ASTR:
        code = PsiFormGenerator::FORM_A;
        break;

    default:
        code = PsiFormGenerator::FORM_E;
        break;
    }

    return code;

}

PsiFormGenerator::PsiFormGenerator(int n) : _n(n)
{

    _initStack();

}

std::shared_ptr<ZZ_mat<mpz_t>> PsiFormGenerator::getForm()
{

    return decltype(getForm())();

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

void PsiFormGenerator::test()
{

    std::cout << _partitions.size() << std::endl;

    for(const auto &vec : _partitions) {
        std::cout << vec << std::endl;
    }

    CoxeterFormCode code = FORM_A;
    for (int i = 0; i < 9; ++i) {
        ++code;
        std::cout << code << std::endl;
    }

    std::vector<int> partition = {15, 9};
    PsiFormStore substate(&partition);

    for (auto i = substate.begin(); i != substate.end(); ++i) {
        std::cout << i._state << std::endl;
    }

}



/****************************************
 * PsiFormIterator's method realization *
 ****************************************/



PsiFormGenerator::PsiFormIterator::PsiFormIterator(const std::vector<int> *partition) noexcept :
_partition(partition)
{
    
    for (unsigned long i = 0; i < partition->size(); ++i) {
        _state.push_back(FORM_A);
    }

}

PsiFormGenerator::PsiFormIterator::PsiFormIterator(const std::vector<int> *partition, 
const std::vector<PsiFormGenerator::CoxeterFormCode> &state) noexcept :
_state(state), _partition(partition)
{}

PsiFormGenerator::PsiFormIterator& PsiFormGenerator::PsiFormIterator::operator++() noexcept
{

    CoxeterFormCode oldLast = _state.back();
    _increaseComponet(0);
    CoxeterFormCode newLast = _state.back();

    if (oldLast != newLast && newLast == FORM_A) {
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

    if (i < _state.size() - 1 && _state[i] == FORM_A) {
        _increaseComponet(i + 1);
    }

}



/*********************************
 * PsiForm's methods realization *
 *********************************/


PsiFormGenerator::PsiFormStore::PsiFormStore(const std::vector<int> *partition) noexcept :
_partition(partition)
{}

PsiFormGenerator::PsiFormIterator PsiFormGenerator::PsiFormStore::begin() noexcept
{

    return PsiFormIterator(_partition);

}

PsiFormGenerator::PsiFormIterator PsiFormGenerator::PsiFormStore::end() noexcept
{

    std::vector<PsiFormGenerator::CoxeterFormCode> state(_partition->size());

    for (auto &code : state) {
        code = FORM_NULL;
    }

    return PsiFormIterator(_partition, state);

}