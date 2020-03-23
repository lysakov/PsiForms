cmake_minimum_required(VERSION 3.5)

#Target name
project(PsiForms)
project(util)

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
${SOURCE_DIR}/PsiForms.cpp
${SOURCE_DIR}/enumerator.cpp
${SOURCE_DIR}/generator.cpp
${SOURCE_DIR}/CoxeterForms.cpp
${SOURCE_DIR}/Logger.cpp
${SOURCE_DIR}/utils.cpp)

set(MAIN_SRC
${SOURCE_DIR}/main.cpp
${SOURCE_DIR}/ParallelWrapper.cpp)

set(UTIL_SRC
${SOURCE_DIR}/PsiFormUtil.cpp
${SOURCE_DIR}/Parser.cpp)

#Searching for libraries
find_library(FPLLL_LIB fplll)
find_library(GMP_LIB gmp)
find_library(MPFR_LIB mpfr)

#Searching for pthread lib
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(${INCLUDE_DIR})
add_executable(PsiForms ${SOURCE_FILES} ${MAIN_SRC})

add_executable(util ${SOURCE_FILES} ${UTIL_SRC})

target_link_libraries(PsiForms
${FPLLL_LIB}
${GMP_LIB}
${MPFR_LIB}
${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(util
${FPLLL_LIB}
${GMP_LIB}
${MPFR_LIB}
${CMAKE_THREAD_LIBS_INIT})