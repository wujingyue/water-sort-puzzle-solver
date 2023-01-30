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
  bool DfsWithBound(const int bound, State& x,
                    std::vector<std::pair<int, int>>& solution);

  // A configurable upper bound of the depth of the search.
  const int max_num_moves_;

  // Must be reset before each call of DfsWithBound. IDA* typically doesn't
  // deduplicate. However, for this particular problem, we found deduplication
  // speeds up the search a lot (e.g. from a minute down to a second).
  std::unordered_map<State, int> shortest_moves_;
};

#endif  // _SOLVER_H
