#include "Common/ConstantsAndTypes.h"
#include "Common/Util.h"
#include "DataModel/Zp.h"
#include "Implementations/DynamicProgrammingImpl.h"
#include "Implementations/MainImpl.h"
#include "SubsetSumSolver.h"
#include "SupplementaryAlgorithms/PrimeDecider.h"


#include <iostream>
#include <functional>
#include <map>

int main() {
//    std::iostream::sync_with_stdio(false);
//    std::cin.tie(nullptr);

    int z;
    std::cin >> z;

    std::size_t correctAnswers = 0, wrongAnswers = 0;

    while( z-- )
    {
//        SubsetSumSolver<Int> ssSolverMainAlgorithm(std::make_unique<MainImpl<Int, ExponentCalculationPolicy::DIVIDE_AND_CONQUER, FormalSeriesMultiplicationPolicy::NTT>>());
//        SubsetSumSolver<Int> ssSolverMainAlgorithm(std::make_unique<MainImpl<Int,
//                ExponentCalculationPolicy::DIVIDE_AND_CONQUER,
//                FormalSeriesMultiplicationPolicy::TRIVIAL>>());
        SubsetSumSolver<Int> ssSolverMainAlgorithm(std::make_unique<MainImpl<Int,
                ExponentCalculationPolicy::NEWTONS_ITERATIVE_METHOD,
                FormalSeriesMultiplicationPolicy::NTT>>());

        std::cin >> ssSolverMainAlgorithm;

        SubsetSumSolver<Int> ssSolverDpAlgorithm(std::make_unique<DynamicProgrammingImpl<Int>>(ssSolverMainAlgorithm.getImpl()));

        auto mainAlgorithmTime = executeAndMeasureTime([&ssSolverMainAlgorithm]{ ssSolverMainAlgorithm.solve(); });
        auto dpAlgorithmTime = executeAndMeasureTime([&ssSolverDpAlgorithm]{ ssSolverDpAlgorithm.solve(); });

        if(OUTPUT_TIME)
        {
            std::cout << "Main algorithm time - " << mainAlgorithmTime.count() << " seconds, "
                    << ssSolverMainAlgorithm.result() << " result\n";
            std::cout << "Dynamic programming algorithm time - " << dpAlgorithmTime.count() << " seconds, "
                    << ssSolverDpAlgorithm.result() << " result\n";
        }

        (ssSolverMainAlgorithm.result() == ssSolverDpAlgorithm.result()) ? correctAnswers++ : wrongAnswers++;
    }

    std::cout << "Correct answers " << correctAnswers << '\n';
    std::cout << "Wrong answers " << wrongAnswers << '\n';

    return 0;
}
