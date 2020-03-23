#include "Parser.hpp"

Parser::Parser(const std::string &str)
{

    std::regex psiForm("([adeADE]\\d+)(\\s[adeADE]\\d+)*");

    if (!regex_match(str, psiForm)) {
        throw std::invalid_argument("Psi-form corresponding to input doesn't exist.");
    }

    std::regex coxeterForm("[adeADE]\\d+");
    for (auto iter = std::sregex_iterator(str.begin(), str.end(), coxeterForm); iter != std::sregex_iterator(); ++iter) {
        int formDim = std::stoi(iter->str().substr(1));
        _dim.push_back(formDim);

        switch (iter->str()[0]) {
        case 'a':
            _state.push_back(FORM_A);
            break;
        
        case 'A':
            _state.push_back(FORM_A_ASTR);
            break;
        
        case 'd':
            _state.push_back(FORM_D);
            break;
        
        case 'D':
            _state.push_back(FORM_D_ASTR);
            break;
        
        case 'e':
            _state.push_back(FORM_E);
            break;
        
        case 'E':
            if (formDim == 7) {
                _state.push_back(FORM_E7_ASTR);
            }
            else if (formDim == 8) {
                _state.push_back(FORM_E8_ASTR);
            }
            else {
                _state.push_back(FORM_E9_ASTR);
            }

            break;
        
        default:
            break;
        }

    }

}

const std::vector<CoxeterFormCode>& Parser::getState() const noexcept
{

    return _state;

}

const std::vector<int>& Parser::getPartition() const noexcept
{

    return _dim;

}