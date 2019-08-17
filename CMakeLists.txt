cmake_minimum_required(VERSION 3.10)
project(Subset_Sum_Solver)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Werror -fsanitize=address -ftest-coverage")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -Wall -Werror")
set(CMAKE_EXE_LINKER_FLAGS "-pg -lm -fext-numeric-literals -lquadmath -lgcov")
#set(CMAKE_VERBOSE_MAKEFILE OFF)

include_directories(. GSL/include)

add_executable(Subset_Sum_Solver
        main.cpp
        SupplementaryAlgorithms/PrimeDecider.cpp
        SupplementaryAlgorithms/PrimeDecider.h
        Common/Util.h
        SubsetSumSolver.cpp
        SubsetSumSolver.h
        Implementations/SubsetSumImpl.cpp
        Implementations/SubsetSumImpl.h
        Implementations/MainImpl.cpp
        Implementations/MainImpl.h
        Common/ConstantsAndTypes.h
        SupplementaryAlgorithms/FFTCalculator.cpp
        SupplementaryAlgorithms/FFTCalculator.h
        DataModel/FormalPowerSeries.cpp
        DataModel/FormalPowerSeries.h
        Implementations/DynamicProgrammingImpl.cpp
        Implementations/DynamicProgrammingImpl.h
        DataModel/Zp.cpp
        DataModel/Zp.h
        SupplementaryAlgorithms/NTTCalculator.cpp
        SupplementaryAlgorithms/NTTCalculator.h)
