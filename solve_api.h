#ifndef SOLVE_API_H_
#define SOLVE_API_H_

#include <utility>
#include <vector>

#include "absl/status/statusor.h"

enum class Algorithm {
  kAStar,
  kIterativeDeepeningAStar,
};

// By default, we chose to use the iterative deepening A* (IDA*) algorithm,
// which is exact and fastest. It has a much smaller footprint than BFS and A*.
// It allows to plug in a heuristic function that significantly speeds up the
// search. It is guaranteed to find the shortest solution as long as the
// heuristic function is admissible.
absl::StatusOr<std::vector<std::pair<int, int>>> Solve(
    const std::vector<std::vector<int>>& tubes, int volume,
    Algorithm algorithm = Algorithm::kIterativeDeepeningAStar);

#endif  // SOLVE_API_H_
