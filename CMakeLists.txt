cmake_minimum_required(VERSION 3.13)
project(Subset_Sum_Solver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-fsanitize=address")

add_executable(Subset_Sum_Solver main.cpp PrimeDecider.cpp PrimeDecider.h Util.h SubsetSumSolver.cpp SubsetSumSolver.h SubsetSumImpl.cpp SubsetSumImpl.h DummyImplementation.cpp DummyImplementation.h ConstantsAndTypes.h FFTSolver.cpp FFTSolver.h FormalPowerSeries.cpp FormalPowerSeries.h)