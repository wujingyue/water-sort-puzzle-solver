#ifndef ASTAR_H_
#define ASTAR_H_

#include <unordered_map>
#include <utility>
#include <vector>

#include "solver.h"
#include "state.h"

class AStar : public Solver {
 public:
  struct Operation {
    int from;
    int to;
    int water;
  };

  bool Solve(const State& initial_state,
             std::vector<std::pair<int, int>>& solution) override;

 private:
  void ReconstructPath(State x,
                       const std::unordered_map<State, Operation>& reaching_op,
                       std::vector<std::pair<int, int>>& solution);
};

#endif  // ASTAR_H_
