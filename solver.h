#ifndef _SOLVER_H
#define _SOLVER_H

#include <utility>
#include <vector>

#include "state.h"

class Solver {
 public:
  virtual ~Solver() {}
  virtual bool Solve(const State& initial_state,
                     std::vector<std::pair<int, int>>& solution) = 0;
};

#endif  // _SOLVER_H
