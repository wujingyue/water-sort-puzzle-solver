#ifndef _SOLVER_H
#define _SOLVER_H

#include <unordered_set>
#include <utility>
#include <vector>

#include "absl/status/statusor.h"

#include "state.h"

class Solver {
 public:
  absl::StatusOr<std::vector<std::pair<int, int>>> Solve(
      const std::vector<std::vector<int>>& tubes, int volume);
};

#endif  // _SOLVER_H
