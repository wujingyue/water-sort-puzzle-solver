#ifndef _SOLVER_H
#define _SOLVER_H

#include <unordered_set>
#include <utility>
#include <vector>

#include "absl/status/statusor.h"

#include "state.h"

class Solver {
 public:
  Solver(int max_num_moves): max_num_moves_(max_num_moves) {}

  absl::StatusOr<std::vector<std::pair<int, int>>> Solve(
      const State& initial_state);

 private:
  bool DfsWithBound(State x, const int bound,
                    std::vector<std::pair<int, int>>& solution);

  const int max_num_moves_;

  // Must be reset before each call of DfsWithBound.
  std::unordered_set<State> visited_;
};

#endif  // _SOLVER_H
