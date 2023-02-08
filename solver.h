#ifndef SOLVER_H_
#define SOLVER_H_

#include <utility>
#include <vector>

#include "state.h"

class Solver {
 public:
  virtual ~Solver() {}
  virtual bool Solve(const State& initial_state,
                     std::vector<std::pair<int, int>>& solution) = 0;
};

#endif  // SOLVER_H_
