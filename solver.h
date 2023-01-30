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
      const std::vector<std::vector<int>>& tubes, int volume);

 private:
  // A configurable upper bound of the depth of the search.
  const int max_num_moves_;
};

#endif  // _SOLVER_H
