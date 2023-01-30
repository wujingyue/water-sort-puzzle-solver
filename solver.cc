#include "solver.h"

#include <algorithm>
#include <chrono>
#include <queue>

#include "absl/log/log.h"
#include "absl/strings/str_cat.h"

#include "state.h"

bool Solver::DfsWithBound(const int bound, State& x,
                          std::vector<std::pair<int, int>>& solution) {
  if (x.Done()) {
    return true;
  }

  const int moves = solution.size();

  if (moves + x.EstimatedCost() > bound) {
    return false;
  }

  // To find the shortest solution, we revisit a state when its number of moves
  // gets improved.
  if (shortest_moves_.count(x) != 0 && shortest_moves_.at(x) <= moves) {
    return false;
  }
  shortest_moves_[x] = moves;
  num_shortest_moves_++;

  for (int from = 0; from < x.NumTubes(); from++) {
    for (int to = 0; to < x.NumTubes(); to++) {
      if (from == to) {
        continue;
      }
      if (const int water = x.Pour(from, to); water > 0) {
        solution.push_back({from, to});
        if (DfsWithBound(bound, x, solution)) {
          return true;
        }
        solution.pop_back();
        x.Pour(to, from, water);
      }
    }
  }

  return false;
}

absl::StatusOr<std::vector<std::pair<int, int>>> Solver::Solve(
    const std::vector<std::vector<int>>& tubes, const int volume) {
  absl::StatusOr<State> initial_state = State::Create(tubes, volume);
  if (!initial_state.ok()) {
    return initial_state.status();
  }

  // We chose to use the iterative deepening A* (IDA*) algorithm, because IDA*
  // 1. has a much smaller footprint than BFS and A*,
  // 2. allows to plug in a heuristic function that significantly speeds
  // up the search, and
  // 3. is guaranteed to find the shortest solution as long as the
  // heuristic function is admissible.
  std::vector<std::pair<int, int>> solution;
  for (int bound = 0; bound <= max_num_moves_; bound++) {
    LOG(INFO) << "Searching with bound " << bound << "...";
    const auto begin_time = std::chrono::steady_clock::now();
    shortest_moves_.clear();
    const bool succeeded = DfsWithBound(bound, *initial_state, solution);
    const auto end_time = std::chrono::steady_clock::now();
    LOG(INFO) << (succeeded ? "Succeeded" : "Failed") << " after "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     end_time - begin_time)
                     .count()
              << "ms. This search visited " << shortest_moves_.size()
              << " states.";
    if (succeeded) {
      return solution;
    }
  }
  return absl::NotFoundError(absl::StrCat("Failed to find a solution within ",
                                          max_num_moves_, " moves."));
}
