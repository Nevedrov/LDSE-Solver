#ifndef TSS_BUILDER_H
#define TSS_BUILDER_H
#include "solver\solver.hpp"

struct TSSBuilder {
  static basis_vectors_t Solve(SLDEProblem& problem);
};

#endif
