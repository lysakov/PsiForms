#ifndef PARSER
#define PARSER

#include <regex>
#include "CoxeterForms.hpp"

/**
 * Paser parses command line
 */
class Parser
{

  public:
    /**
     * @param str - string to be parsed
     */
    Parser(const std::string &str);

    /**
     * @brief creates state vector, corresponding to the parsed line
     * @returns state vector
     */
    const std::vector<CoxeterFormCode>& getState() const noexcept;

    /**
     * @brief creates partition vector, corresponding to the parsed line
     * @returns partition vector
     */
    const std::vector<int>& getPartition() const noexcept;

    /**
     * Command line keys
     */
    class Options
    {

      public:
        Options() {}

        /**
         * Print shortest vectors
         */
        bool svp = false;

        /**
         * Number of shortest vectors to be printed.
         * Works only if svp = true.
         * By default equals to 0 and prints all solutions.
         */
        int solNum = 0;

        /**
         * True if there is a need to exclude var
         */
        bool excludeVar = true;

    };

  private:
    /* State vector */
    std::vector<CoxeterFormCode> _state;

    /* Partition vector */
    std::vector<int> _dim;

};

#endif