#include "idastar.h"

#include <chrono>
#include <unordered_map>

#include "absl/log/log.h"
#include "state.h"

class DfsWithBound {
 public:
  DfsWithBound(const int bound): bound_(bound) {}

  // When `x` can reach an end state, this function returns true, modifies `x`
  // to be the reachable end state and stores the solution in `solution`. When
  // `x` cannot reach an end state, this function returns false and leaves `x`
  // and `solution` unmodified.
  bool Search(State& x, std::vector<std::pair<int, int>>& solution) {
    num_visits_++;

    // To find the shortest solution, we revisit a state when its number of
    // moves gets improved. In practice, we observed that the search visits a
    // state roughly twice on average.
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

  // We revisit a state when we find a shorter move sequence. So this number can
  // be significantly larger than `NumVisitedStates`.
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

bool IterativeDeepeningAStar::Solve(
    const State& initial_state, std::vector<std::pair<int, int>>& solution) {
  // Make a copy so DfsWithBound::Search can modify the state in place.
  State state(initial_state);

  int bound = 0;
  while (bound < std::numeric_limits<int>::max()) {
    LOG(INFO) << "Searching with bound " << bound << "...";
    const auto begin_time = std::chrono::steady_clock::now();
    DfsWithBound searcher(bound);
    const bool succeeded = searcher.Search(state, solution);
    const auto end_time = std::chrono::steady_clock::now();
    LOG(INFO) << (succeeded ? "Succeeded" : "Failed") << " after "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     end_time - begin_time)
                     .count()
              << "ms. This search visited " << searcher.NumVisitedStates()
              << " states " << searcher.NumVisits() << " times.";
    if (succeeded) {
      return true;
    }
    bound = searcher.NextBound();
  }

  return false;
}
