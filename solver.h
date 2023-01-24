#ifndef _SOLVER_H
#define _SOLVER_H

#include <unordered_set>
#include <utility>
#include <vector>

#include "state.h"

class Solver {
 public:
  bool Solve(const State& initial_state,
             std::vector<std::pair<int, int>>& solution);

 private:
  bool DfsWithBound(State x, const int bound,
                    std::vector<std::pair<int, int>>& solution);

  // Must be reset before each call of DfsWithBound.
  std::unordered_set<State> visited_;
};

#endif  // _SOLVER_H
