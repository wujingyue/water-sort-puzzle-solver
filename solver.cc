#include "solver.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <queue>

#include "absl/strings/str_cat.h"

#include "state.h"

bool Solver::DfsWithBound(State x, const int bound,
                          std::vector<std::pair<int, int>>& solution) {
  if (x.Done()) {
    return true;
  }

  if ((int)solution.size() + x.EstimatedCost() > bound) {
    return false;
  }

  if (visited_.count(x) != 0) {
    return false;
  }
  visited_.insert(x);

  for (int from = 0; from < x.NumTubes(); from++) {
    for (int to = 0; to < x.NumTubes(); to++) {
      if (from == to) {
        continue;
      }
      if (const int water = x.Pour(from, to); water > 0) {
        solution.push_back({from, to});
        if (DfsWithBound(x, bound, solution)) {
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
    const State& initial_state) {
  std::vector<std::pair<int, int>> solution;
  for (int bound = 0; bound <= max_num_moves_; bound++) {
    std::cerr << "Searching with bound " << bound << "..." << std::endl;
    const auto begin_time = std::chrono::steady_clock::now();
    visited_.clear();
    const bool succeeded = DfsWithBound(initial_state, bound, solution);
    const auto end_time = std::chrono::steady_clock::now();
    std::cerr << (succeeded ? "Succeeded" : "Failed") << " after "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     end_time - begin_time)
                     .count()
              << "ms. This search visited " << visited_.size() << " states."
              << std::endl;
    if (succeeded) {
      return solution;
    }
  }
  return absl::NotFoundError(absl::StrCat("Failed to find a solution within ",
                                          max_num_moves_, " moves."));
}
