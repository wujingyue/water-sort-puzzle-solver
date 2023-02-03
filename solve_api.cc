#include "solve_api.h"

#include <utility>
#include <vector>

#include "absl/status/statusor.h"

#include "astar.h"
#include "idastar.h"
#include "solver.h"
#include "state.h"

static absl::Status Verify(State state,
                           const std::vector<std::pair<int, int>>& solution) {
  for (const auto& [from, to] : solution) {
    const int water = state.Pour(from, to);
    if (water <= 0) {
      return absl::InvalidArgumentError(absl::StrCat(
          "Failed to pour from tube ", from + 1, " to tube ", to + 1, "."));
    }
  }
  if (!state.Done()) {
    return absl::InvalidArgumentError(
        absl::StrCat("The game has not finished: \n", state.DebugString()));
  }
  return absl::OkStatus();
}

absl::StatusOr<std::vector<std::pair<int, int>>> Solve(
    const std::vector<std::vector<int>>& tubes, const int volume,
    const Algorithm algorithm) {
  absl::StatusOr<State> initial_state = State::Create(tubes, volume);
  if (!initial_state.ok()) {
    return initial_state.status();
  }

  std::unique_ptr<Solver> solver;
  switch (algorithm) {
    case Algorithm::kAStar:
      solver.reset(new AStar());
      break;
    case Algorithm::kIterativeDeepeningAStar:
      solver.reset(new IterativeDeepeningAStar());
      break;
  }

  std::vector<std::pair<int, int>> solution;
  if (!solver->Solve(*initial_state, solution)) {
    return absl::NotFoundError("Failed to find a solution.");
  }

  absl::Status solution_is_valid = Verify(*initial_state, solution);
  if (!solution_is_valid.ok()) {
    return solution_is_valid;
  }

  return solution;
}
