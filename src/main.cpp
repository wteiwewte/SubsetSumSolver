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

  const double numberOfInputs = z;

  std::cout << "Formal series multiplication policy " << multPolicyName[FORMAL_SERIES_MULT_POLICY]
            << '\n';
  if constexpr(FORMAL_SERIES_MULT_POLICY == NTT && NTT_CHECK_OVERFLOW_CHECK)
      std::cerr << "Checking NTT's overflows, may affect performance\n";

  std::size_t correctAnswers = 0, wrongAnswers = 0;
  double newtonSumOfTimes = 0.0, divideAndConquerSumOfTimes = 0.0, dpSumOfTimes = 0.0;

  while (z--)
  {
    SubsetSumSolver<Int> newtonSolver(
        std::make_unique<MainImpl<Int, ExponentCalculationPolicy::NEWTONS_ITERATIVE_METHOD>>());

    std::cin >> newtonSolver;

    SubsetSumSolver<Int> divideAndConquerSolver(
        std::make_unique<MainImpl<Int, ExponentCalculationPolicy::DIVIDE_AND_CONQUER>>(newtonSolver.getImpl()));

    SubsetSumSolver<Int> dpSolver(
        std::make_unique<DynamicProgrammingImpl<Int>>(newtonSolver.getImpl()));

    const auto newtonsTime =
        executeAndMeasureTime([&newtonSolver] { newtonSolver.solve(); });
    const auto divideAndConquerTime =
        executeAndMeasureTime([&divideAndConquerSolver] { divideAndConquerSolver.solve(); });
    const auto dpTime =
        executeAndMeasureTime([&dpSolver] { dpSolver.solve(); });

    if constexpr (OUTPUT_TIME)
    {
      std::cout << "Dynamic programming time - " << dpTime.count()
                << " seconds, " << dpSolver.result() << " result\n";
      std::cout << "Divide and conquer time - " << divideAndConquerTime.count()
                << " seconds, " << divideAndConquerSolver.result() << " result\n";
      std::cout << "Newton's iterative method time - " << newtonsTime.count() << " seconds, "
                << newtonSolver.result() << " result\n";
    }

    dpSumOfTimes += dpTime.count();
    divideAndConquerSumOfTimes += divideAndConquerTime.count();
    newtonSumOfTimes += newtonsTime.count();

    (newtonSolver.result() == dpSolver.result() && dpSolver.result() == divideAndConquerSolver.result()) ? correctAnswers++ : wrongAnswers++;
  }

  std::cout << "Average times\n";
  std::cout << "DP|DIV&CONQ|NEWTON\n";
  std::cout << dpSumOfTimes / numberOfInputs <<
      ',' << divideAndConquerSumOfTimes / numberOfInputs <<
      ',' << newtonSumOfTimes / numberOfInputs << '\n';


  std::cout << "Correct answers " << correctAnswers << '\n';
  std::cout << "Wrong answers " << wrongAnswers << '\n';

  return 0;
}
