#ifndef ASTAR_H_
#define ASTAR_H_

#include <unordered_map>
#include <utility>
#include <vector>

#include "solver.h"
#include "state.h"

class AStar : public Solver {
 public:
  bool Solve(const State& initial_state, Solution& solution) override;

 private:
  void ReconstructPath(State x,
                       const std::unordered_map<State, Step>& reaching_op,
                       Solution& solution);
};

#endif  // ASTAR_H_
