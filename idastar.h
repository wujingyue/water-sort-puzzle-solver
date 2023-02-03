#ifndef _IDASTAR_H
#define _IDASTAR_H

#include <utility>
#include <vector>

#include "solver.h"
#include "state.h"

class IterativeDeepeningAStar : public Solver {
 public:
  bool Solve(const State& initial_state,
             std::vector<std::pair<int, int>>& solution) override;
};

#endif  // _IDASTAR_H
