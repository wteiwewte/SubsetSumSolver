#include "Common/ConstantsAndTypes.h"
#include "Common/Util.h"
#include "DataModel/Zp.h"
#include "Implementations/DynamicProgrammingImpl.h"
#include "Implementations/MainImpl.h"
#include "SubsetSumSolver.h"
#include "SupplementaryAlgorithms/PrimeDecider.h"

#include <functional>
#include <iostream>
#include <map>

int
main()
{
  int z;
  std::cin >> z;

  std::cout << "Formal series multiplication policy " << multPolicyName[FORMAL_SERIES_MULT_POLICY]
            << '\n';

  std::size_t correctAnswers = 0, wrongAnswers = 0;

  while (z--)
  {
    SubsetSumSolver<Int> ssSolverMainAlgorithm(
        std::make_unique<MainImpl<Int, ExponentCalculationPolicy::NEWTONS_ITERATIVE_METHOD>>());

    std::cin >> ssSolverMainAlgorithm;

    SubsetSumSolver<Int> ssSolverDpAlgorithm(
        std::make_unique<DynamicProgrammingImpl<Int>>(ssSolverMainAlgorithm.getImpl()));

    auto mainAlgorithmTime =
        executeAndMeasureTime([&ssSolverMainAlgorithm] { ssSolverMainAlgorithm.solve(); });
    auto dpAlgorithmTime =
        executeAndMeasureTime([&ssSolverDpAlgorithm] { ssSolverDpAlgorithm.solve(); });

    if (OUTPUT_TIME)
    {
      std::cout << "Main algorithm time - " << mainAlgorithmTime.count() << " seconds, "
                << ssSolverMainAlgorithm.result() << " result\n";
      std::cout << "Dynamic programming algorithm time - " << dpAlgorithmTime.count()
                << " seconds, " << ssSolverDpAlgorithm.result() << " result\n";
    }

    (ssSolverMainAlgorithm.result() == ssSolverDpAlgorithm.result()) ? correctAnswers++
                                                                     : wrongAnswers++;
  }

  std::cout << "Correct answers " << correctAnswers << '\n';
  std::cout << "Wrong answers " << wrongAnswers << '\n';

  return 0;
}