#include "Common/ConstantsAndTypes.h"
#include "Implementations/DynamicProgrammingImpl.h"
#include "Implementations/MainImpl.h"
#include "SupplementaryAlgorithms/PrimeDecider.h"
#include "SubsetSumSolver.h"
#include "Common/Util.h"

#include <iostream>
#include <functional>

int main() {
    std::iostream::sync_with_stdio(false);
//    std::cin.tie(nullptr);
//    std::cout << PrimeDecider<__int128>::isPrime(9999999) << '\n';
//    std::cout << PrimeDecider<__int128>::isPrime(179426369) << '\n';

    int z;
    std::cin >> z;

    std::size_t correctAnswers = 0, wrongAnswers = 0;


    while( z-- )
    {
//        SubsetSumSolver<Int> ssSolverMainAlgorithm(std::make_unique<MainImpl<Int, ExponentCalculationPolicy::DIVIDE_AND_CONQUER>>());
        SubsetSumSolver<Int> ssSolverMainAlgorithm(std::make_unique<MainImpl<Int, ExponentCalculationPolicy::NEWTONS_ITERATIVE_METHOD>>());

        std::cin >> ssSolverMainAlgorithm;

        SubsetSumSolver<Int> ssSolverDpAlgorithm(std::make_unique<DynamicProgrammingImpl<Int>>(ssSolverMainAlgorithm.getImpl()));

        auto mainAlgorithmTime = executeAndMeasureTime([&ssSolverMainAlgorithm]{ ssSolverMainAlgorithm.solve(); });
        auto dpAlgorithmTime = executeAndMeasureTime([&ssSolverDpAlgorithm]{ ssSolverDpAlgorithm.solve(); });

        if(OUTPUT_TIME)
        {
            std::cout << "Main algorithm time - " << mainAlgorithmTime.count() << " seconds\n";
            std::cout << "Dynamic programming algorithm time - " << dpAlgorithmTime.count() << " seconds\n";
        }

        (ssSolverMainAlgorithm.result() == ssSolverDpAlgorithm.result()) ? correctAnswers++ : wrongAnswers++;
    }

    std::cout << "Correct answers " << correctAnswers << '\n';
    std::cout << "Wrong answers " << wrongAnswers << '\n';

    return 0;
}
