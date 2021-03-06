set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Werror -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -Wall -Werror")
#set(CMAKE_VERBOSE_MAKEFILE OFF)

include_directories(. GSL/include)

add_library(Subset_Sum_Solver
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
add_executable (Main main.cpp)
target_link_libraries (Main Subset_Sum_Solver)    
