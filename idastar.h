#ifndef IDASTAR_H_
#define IDASTAR_H_

#include <utility>
#include <vector>

#include "solver.h"
#include "state.h"

class IterativeDeepeningAStar : public Solver {
 public:
  bool Solve(const State& initial_state,
             std::vector<std::pair<int, int>>& solution) override;
};

#endif  // IDASTAR_H_
