#ifndef SOLVER_H_
#define SOLVER_H_

#include <vector>

#include "state.h"

struct Step {
  int from;
  int to;
  int water;
  int color_id;
};

using Solution = std::vector<Step>;

class Solver {
 public:
  virtual ~Solver() {}

  virtual bool Solve(const State& initial_state, Solution& solution) = 0;
};

#endif  // SOLVER_H_
