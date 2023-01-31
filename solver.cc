#include "solver.h"

#include <algorithm>
#include <chrono>
#include <queue>

#include "absl/log/log.h"
#include "absl/strings/str_cat.h"

#include "state.h"

class DfsWithBound {
 public:
  DfsWithBound(const int bound): bound_(bound) {}

  bool Search(State& x, std::vector<std::pair<int, int>>& solution) {
    num_visits_++;

    // To find the shortest solution, we revisit a state when its number of
    // moves gets improved.
    const int moves_from_start = solution.size();
    if (shortest_moves_.count(x) != 0 &&
        shortest_moves_.at(x) <= moves_from_start) {
      return false;
    }
    shortest_moves_[x] = moves_from_start;

    if (x.Done()) {
      return true;
    }

    const int estimated_moves = moves_from_start + x.EstimatedMovesToEnd();
    if (estimated_moves > bound_) {
      next_bound_ = std::min(next_bound_, estimated_moves);
      return false;
    }

    for (int from = 0; from < x.NumTubes(); from++) {
      for (int to = 0; to < x.NumTubes(); to++) {
        if (from == to) {
          continue;
        }
        if (const int water = x.Pour(from, to); water > 0) {
          solution.push_back({from, to});
          if (Search(x, solution)) {
            return true;
          }
          solution.pop_back();
          x.Pour(to, from, water);
        }
      }
    }

    return false;
  }

  int NumVisitedStates() const { return shortest_moves_.size(); }

  int NumVisits() const { return num_visits_; }

  int NextBound() const { return next_bound_; }

 private:
  // IDA* typically doesn't deduplicate. However, for this particular problem,
  // we found deduplication speeds up the search a lot (e.g. from a minute down
  // to a second).
  std::unordered_map<State, int> shortest_moves_;

  const int bound_;

  int num_visits_ = 0;

  int next_bound_ = std::numeric_limits<int>::max();
};


absl::StatusOr<std::vector<std::pair<int, int>>> Solver::Solve(
    const std::vector<std::vector<int>>& tubes, const int volume) {
  absl::StatusOr<State> initial_state = State::Create(tubes, volume);
  if (!initial_state.ok()) {
    return initial_state.status();
  }

  // We chose to use the iterative deepening A* (IDA*) algorithm, because IDA*
  // 1. has a much smaller footprint than BFS and A* and is therefore faster,
  // 2. allows to plug in a heuristic function that significantly speeds
  // up the search, and
  // 3. is guaranteed to find the shortest solution as long as the
  // heuristic function is admissible.
  std::vector<std::pair<int, int>> solution;
  int bound = 0;
  while (bound < std::numeric_limits<int>::max()) {
    LOG(INFO) << "Searching with bound " << bound << "...";
    const auto begin_time = std::chrono::steady_clock::now();
    DfsWithBound searcher(bound);
    const bool succeeded = searcher.Search(*initial_state, solution);
    const auto end_time = std::chrono::steady_clock::now();
    LOG(INFO) << (succeeded ? "Succeeded" : "Failed") << " after "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     end_time - begin_time)
                     .count()
              << "ms. This search visited " << searcher.NumVisitedStates()
              << " states " << searcher.NumVisits() << " times.";
    if (succeeded) {
      return solution;
    }
    bound = searcher.NextBound();
  }

  return absl::NotFoundError(absl::StrCat("Failed to find a solution."));
}
