#ifndef _IDASTAR_H
#define _IDASTAR_H

#include <utility>
#include <vector>

#include "state.h"

class IterativeDeepeningAStar {
 public:
  bool Solve(const State& initial_state,
             std::vector<std::pair<int, int>>& solution);
};

#endif  // _IDASTAR_H
