#include "PsiForms.hpp"



/*****************************************
 * PsiFormGenerator's method realization *
 *****************************************/



PsiFormGenerator::PsiFormGenerator(int n) : _n(n + 1), _store({}), _iter(nullptr)
{

    if (n <= 0) {
        throw std::runtime_error("Impossible to generate psi forms in space with this dimension.");
    }

    _initStack();
    _load();

}

ZZ_mat<mpz_t> PsiFormGenerator::getForm()
{

    std::lock_guard<std::mutex> lock(_mtx);

    decltype(getForm()) result;

    auto getNextForm = [&]() {

        Logger::debug("==========================");
        Logger::debug("Form № " + std::to_string(_cnt));
        Logger::debug("==========================");
    
        result = *_iter;

        Logger::debug(result);

        ++_iter;
        ++_cnt;

    };

    if (_iter != _store.end()) {
        getNextForm();
    }
    else {
        while (_iter == _store.end() && !_partitions.empty()) {
            _load();
        }

        if (_partitions.empty() && _iter == _store.end()) {
            return result;
        }
        else {
            getNextForm();
        }
    }

    return result;

}

ZZ_mat<mpz_t> PsiFormGenerator::getForm(const std::vector<CoxeterFormCode> &state, const std::vector<int> &dim)
{

    decltype(getForm()) result;
    PsiFormStore store(dim);
    auto iter = store.begin();

    while (iter != PsiFormIterator(&dim, state)) {
        ++iter;
    }

    return *iter;

}

bool PsiFormGenerator::empty() const noexcept
{

    std::lock_guard<std::mutex> lock(_mtx);

    return _partitions.empty() && _iter == _store.end();

}

unsigned long long PsiFormGenerator::count() const noexcept
{

    std::lock_guard<std::mutex> lock(_mtx);

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

/* TODO: use std::min_element */
void PsiFormGenerator::_initStack() noexcept
{

    std::vector<int> partition;

    for(int i = 0; i < _n; ++i) {
        partition.push_back(1);
    }

    _partitions.push_back(partition);

    while(partition[0] != _n) {

        auto minPos = _findMin(partition);
        //auto minPos = std::min_element(partition.begin(), --partition.end());
        ++partition[minPos];
        //++(*minPos);
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
    }

}

bool PsiFormGenerator::_necessity(const std::vector<CoxeterFormCode> &state,
const std::vector<int> &dim) noexcept
{

    /****************** => *******************/

    unsigned long maxDelta = 0;
    unsigned long extDelta = 0;
    unsigned long curPos = 0;
    bool extFound = false;

    auto isExt = [](CoxeterFormCode code) -> bool {

        if (code == FORM_A_ASTR || code == FORM_D_ASTR || code > FORM_E) {
            return true;
        }

        return false;

    };

    auto pred = [&](CoxeterFormCode code) -> bool {

        if (!isExt(code)) {
            auto curDelta = CoxeterFormGenerator::delta(code, dim[curPos]);
            if (curDelta > maxDelta) {
                maxDelta = curDelta;
            }

            if (extFound && maxDelta >= extDelta) {
                ++curPos;
                return false;
            }
        }
        else {
            if (extFound) {
                ++curPos;
                return false;
            }

            extFound = true;
            extDelta = CoxeterFormGenerator::delta(code, dim[curPos]);

            if (extDelta <= maxDelta) {
                ++curPos;
                return false;
            }
        }

        ++curPos;

        return true;

    };

    return std::all_of(state.begin(), state.end(), pred);


}

/* WARNING: SHITCODE */
/* TODO: REFACTOR THIS FUNCTION */
bool PsiFormGenerator::_sufficiency(const std::vector<CoxeterFormCode> &state, 
const std::vector<int> &dim) noexcept
{

    /****************** <= *******************/

    unsigned long curPos = 0;

    /* Position of forms, which are different from A1 */
    std::vector<unsigned long> positions;    
    bool valid = false;

    auto findSpecForms = [&](const CoxeterFormCode &code) {

        if (code != FORM_A || dim[curPos] != 1) {
            positions.push_back(curPos);
        }

        ++curPos;

    };

    std::for_each(state.begin(), state.end(), findSpecForms);

    /* Consition 1 */
    if (positions.size() >= 3) {
        valid = true;
    }

    /* Condition 2 */
    auto cond2A = [&](CoxeterFormCode code, int dimLowerBound) {

        if (!valid && 
            ((state[positions[0]] == code && dim[positions[0]] >= dimLowerBound) ||
            (state[positions[1]] == code && dim[positions[1]] >= dimLowerBound))) {
                valid = true;
        }

    };

    auto cond2B = [&](CoxeterFormCode code1, CoxeterFormCode code2,
    int dim1, int dim2) {

        if (!valid && 
        ((state[positions[0]] == code1 && dim[positions[0]] == dim1 && 
        state[positions[1]] == code2 && dim[positions[1]] == dim2) ||
        (state[positions[1]] == code1 && dim[positions[1]] == dim1 && 
        state[positions[0]] == code2 && dim[positions[0]] == dim2))) {
            valid = true;
        }

    };

    if (!valid && positions.size() == 2) {
        cond2A(FORM_A, 5);
        cond2A(FORM_D, 6);
        cond2A(FORM_E, 6);
        cond2A(FORM_A_ASTR, 6);
        cond2A(FORM_D_ASTR, 7);
        cond2A(FORM_E7_ASTR, 7);
        cond2A(FORM_E8_ASTR, 8);
        cond2A(FORM_E9_ASTR, 9);

        if (!valid) {
            cond2B(FORM_D, FORM_D, 5, 4);
            cond2B(FORM_D, FORM_D, 5, 5);
            cond2B(FORM_D_ASTR, FORM_D, 5, 4);
            cond2B(FORM_D_ASTR, FORM_D, 6, 4);
            cond2B(FORM_D_ASTR, FORM_D, 6, 5);
        }

        if (!valid && state.size() >= 3) {
            cond2B(FORM_A, FORM_A_ASTR, 2, 3);
            cond2B(FORM_A, FORM_A_ASTR, 3, 4);
            cond2B(FORM_A, FORM_A_ASTR, 4, 5);
        }
    }

    /* Condition 3 */
    auto cond3A = [&](CoxeterFormCode code, int dimLowerBound) {

        if (!valid && state[positions[0]] == code && dim[positions[0]] >= dimLowerBound) {
            valid = true;
        }

    };

    auto cond3B = [&](CoxeterFormCode code, int dimBound) {

        if (!valid && state[positions[0]] == code && dim[positions[0]] == dimBound) {
            valid = true;
        }

    };

    if (!valid && positions.size() == 1) {

        cond3A(FORM_A, 8);
        cond3A(FORM_A_ASTR, 7);
        cond3A(FORM_D, 9);
        cond3A(FORM_D_ASTR, 10);

        cond3B(FORM_A_ASTR, 2);
        cond3B(FORM_E8_ASTR, 8);
        cond3B(FORM_E9_ASTR, 9);
    }

    /* Condition 4 */
    if (!valid && positions.empty() && state.size() >= 2) {
        valid = true;
    }

    return valid;

}

bool PsiFormGenerator::_isValid(const std::vector<CoxeterFormCode> &state, 
const std::vector<int> &dim) noexcept {

    if (!_necessity(state, dim)) {
        return false;
    }

    return _sufficiency(state, dim);

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

    while (!_isValid(_state, *_partition) && !_end) {
        ++(*this);
    }

}

PsiFormGenerator::PsiFormIterator::PsiFormIterator(const std::vector<int> *partition, 
const std::vector<CoxeterFormCode> &state) noexcept :
_state(state), _partition(partition)
{}

ZZ_mat<mpz_t> PsiFormGenerator::PsiFormIterator::operator*() const noexcept
{

    CoxeterFormGenerator generator(_state, *_partition);

    ZZ_mat<mpz_t> tmpResult;

    while (!generator.empty()) {
        _glew(tmpResult, generator.getForm());
    }

    Logger &logger = Logger::getInstance();
    if (Logger::getLevel() <= LOG_INFO) {
        for (unsigned long i = 0; i < _state.size(); ++i) {
            logger << _state[i] << (*_partition)[i] << ' ';
        }
    }

    Logger::writeLine(LOG_DEBUG);
    Logger::writeLine(LOG_DEBUG);
    Logger::debug(tmpResult);
    Logger::debug("||");
    Logger::debug("\\/");

    //unsigned long varToBeExcl = tmpResult.get_rows();
    //ZZ_mat<mpz_t> result;

    /*do {
        Logger::writeLine(varToBeExcl);
        result = tmpResult;
        _excludeVar(result, tmpResult.get_rows());
        --varToBeExcl;
    } while(!_isPositive(tmpResult) && varToBeExcl > 0);*/

    _excludeVar(tmpResult, tmpResult.get_rows());

    return tmpResult;

}

PsiFormGenerator::PsiFormIterator& PsiFormGenerator::PsiFormIterator::operator++() noexcept
{

    do {
        _increaseComponet(0);
        /*if (!_isValid(_state, *_partition) && !_end) {
            Logger::writeLine("--------------------------");
            Logger::writeLine(*_partition);
            Logger::writeLine(_state);
            Logger::writeLine("NOT VALID :");
            if (!_necessity(_state, *_partition)) {
                Logger::writeLine("NECESSITY NOT MET");
            }
            else {
                Logger::writeLine("SUFFICIENCY NOT MET");
            }
        }*/
    } while (!_isValid(_state, *_partition) && !_end);

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

ZZ_mat<mpz_t>& PsiFormGenerator::PsiFormIterator::_glew(ZZ_mat<mpz_t> &A, const ZZ_mat<mpz_t> &B) const noexcept
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

    return A;

}

/* x_{n + 1} = -x_1 - x_2 - ... - x_n */
ZZ_mat<mpz_t>& PsiFormGenerator::PsiFormIterator::_excludeVar(ZZ_mat<mpz_t> &A,
unsigned long n) const noexcept
{

    unsigned long dim = A.get_rows();

    n = n;

    /*if (n != dim) {
        A.swap_rows(n - 1, dim - 1);

        //No swap_columns? WTF?
        for (unsigned long i = 0; i < dim; ++i) {
            A[i][dim - 1].swap(A[i][n - 1]);
        }
    }*/

    ZZ_mat<mpz_t> mat1;
    mat1.resize(dim - 1, dim - 1);

    for (int i = 0; i < mat1.get_rows(); ++i) {
        for (int j = 0; j < mat1.get_cols(); ++j) {
            mat1[i][j] = -A[i][dim - 1] + (-A[dim - 1][j]);
        }
    }

    ZZ_mat<mpz_t> mat2;
    mat2.resize(dim - 1, dim - 1);

    for (int i = 0; i < mat2.get_rows(); ++i) {
        for (int j = 0; j < mat2.get_cols(); ++j) {
            mat2[i][j] = A[dim - 1][dim - 1];
        }
    }

    A.resize(dim - 1, dim - 1);
    A = A + mat1 + mat2;

    return A;

}

/* I have no idea what is rMatrix and why it works */
bool PsiFormGenerator::PsiFormIterator::_isPositive(ZZ_mat<mpz_t> A) const noexcept
{

    ZZ_mat<mpz_t> U;
    ZZ_mat<mpz_t> UT;
    MatGSOGram<Z_NR<mpz_t>, FP_NR<mpfr_t>> G(A, U, UT, 1);
    G.update_gso();
    auto rMatrix = G.get_r_matrix();

    for (int i = 0; i < rMatrix.get_rows(); ++i) {
        if (rMatrix[i][i] <= 0) {
            return false;
        }
    }

    return true;

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