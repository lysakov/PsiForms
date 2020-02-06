cmake_minimum_required(VERSION 3.5)

#Target name
project(PsiForms)

#C++ standart
set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDART_REQUIRED True)

#Compiler options
set(CMAKE_CXX_FLAGS 
"-std=c++17 -Wall -Werror -pedantic-errors -Wextra -ftrapv -O2 -g")

#Setting up include directory
set(INCLUDE_DIR
inc)

#Setting up source directory
set(SOURCE_DIR
src)

#Setting up source files for target
set(SOURCE_FILES
${SOURCE_DIR}/main.cpp
${SOURCE_DIR}/PsiForms.cpp
${SOURCE_DIR}/enumerator.cpp
${SOURCE_DIR}/generator.cpp
${SOURCE_DIR}/iterator.cpp)

#Searching for libraries
find_library(FPLLL_LIB fplll)
find_library(GMP_LIB gmp)
find_library(MPFR_LIB mpfr)

include_directories(${INCLUDE_DIR})
add_executable(PsiForms ${SOURCE_FILES})
target_link_libraries(PsiForms
${FPLLL_LIB}
${GMP_LIB}
${MPFR_LIB})