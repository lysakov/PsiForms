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

/* TODO: REFACTOR THIS FUNCTION */
ZZ_mat<mpz_t> PsiFormGenerator::getForm()
{

    decltype(getForm()) result;

    auto log = [&]() {

        Logger::writeLine(*_iter._partition);
        Logger::writeLine(_iter._state);
        Logger::writeLine();

    };

    auto getNextForm = [&]() {

        bool isPositive = false;

        while (!isPositive) {
            Logger::writeLine("==========================");
            Logger::writeLine("Form № " + std::to_string(_cnt));
            Logger::writeLine("==========================");
    
            if (_iter == _store.end()) {
                _load();
            }

            result = *_iter;
            ++_cnt;
            log();
            ++_iter;
            isPositive = _isPositive(result);

            if (!isPositive) {
                Logger::writeLine("FORM IS NOT POSITIVE DEFINED");
                Logger::writeLine();
            }

            Logger::writeLine(result);

        }

    };

    if (_iter != _store.end()) {
        getNextForm();
    }
    else {
        if (_partitions.empty()) {
            return result;
        }

        _load();
        getNextForm();
    }

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
    }

}

bool PsiFormGenerator::_isValid(const std::vector<CoxeterFormCode> &state,
const std::vector<int> &dim) noexcept
{

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

    /*if (dim.size() == 1 && isExt(state[0])) {
        return false;
    }*/

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

/* I have no idea what is rMatrix and why it works */
bool PsiFormGenerator::_isPositive(ZZ_mat<mpz_t> A) const noexcept
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

    _excludeVar(result);

    return result;

}

PsiFormGenerator::PsiFormIterator& PsiFormGenerator::PsiFormIterator::operator++() noexcept
{

    do {
        _increaseComponet(0);
    } while (!_isValid(_state, *_partition));

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
ZZ_mat<mpz_t>& PsiFormGenerator::PsiFormIterator::_excludeVar(ZZ_mat<mpz_t> &A) const noexcept
{

    int n = A.get_rows();

    ZZ_mat<mpz_t> mat1;
    mat1.resize(n - 1, n - 1);

    for (int i = 0; i < mat1.get_rows(); ++i) {
        for (int j = 0; j < mat1.get_cols(); ++j) {
            mat1[i][j] = -A[i][n - 1] + (-A[j][n - 1]);
        }
    }

    ZZ_mat<mpz_t> mat2;
    mat2.resize(n - 1, n - 1);

    for (int i = 0; i < mat2.get_rows(); ++i) {
        for (int j = 0; j < mat2.get_cols(); ++j) {
            if (i == j) {
                mat2[i][j] = A[n - 1][n - 1];
            }
            else {
                mat2[i][j] = A[n - 1][n - 1];
            }
        }
    }

    A.resize(n - 1, n - 1);
    A = A + mat1 + mat2;

    return A;

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