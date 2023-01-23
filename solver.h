#ifndef _SOLVER_H
#define _SOLVER_H

#include <unordered_map>
#include <utility>
#include <vector>

#include "state.h"

class Solver {
 public:
  bool Solve(const State& initial_state,
             std::vector<std::pair<int, int>>& solution) const;

 private:
  void Backtrack(const State& end,
                 const std::unordered_map<State, State>& parent,
                 std::vector<std::pair<int, int>>& solution) const;
};

#endif  // _SOLVER_H
