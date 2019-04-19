#include "ConstantsAndTypes.h"
#include "DummyImplementation.h"
#include "PrimeDecider.h"
#include "SubsetSumSolver.h"
#include "Util.h"

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
        SubsetSumSolver<Int> ssSolver(std::make_unique<DummyImplementation<Int>>());
        auto readingInputTime = executeAndMeasureTime([&ssSolver] { std::cin >> ssSolver; });

        auto solvingTime = executeAndMeasureTime([&ssSolver]{ ssSolver.solve(); });

        auto [isCorrect, verifyingTime] = executeAndMeasureTime([&ssSolver]{ return ssSolver.verify(); });
        if(OUTPUT_TIME)
        {
            std::cout << "Reading input time - " << readingInputTime.count() << " seconds\n";
            std::cout << "Solving time - " << solvingTime.count() << " seconds\n";
            std::cout << "Veryfying time - " << verifyingTime.count() << " seconds\n";
        }
        if(isCorrect)
            correctAnswers++;
    }

    std::cout << "Correct answers " << correctAnswers << '\n';
    std::cout << "Wrong answers " << wrongAnswers << '\n';

    return 0;
}
