#ifndef _ASTAR_H
#define _ASTAR_H

#include <unordered_map>
#include <utility>
#include <vector>

#include "state.h"

class AStar {
 public:
  struct Operation {
    int from;
    int to;
    int water;
  };

  bool Solve(const State& initial_state,
             std::vector<std::pair<int, int>>& solution);

 private:
  void ReconstructPath(State x,
                       const std::unordered_map<State, Operation>& reaching_op,
                       std::vector<std::pair<int, int>>& solution);
};

#endif  // _ASTAR_H
