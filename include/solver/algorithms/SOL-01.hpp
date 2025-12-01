#ifndef SOL_01_H
#define SOL_01_H
#include "solver\solver.hpp"

struct SOL_01Solver {
  static std::vector<std::vector<bool>> Solve(SLDEProblem& problem);
};

#endif
