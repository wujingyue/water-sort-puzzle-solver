#include "solver.h"

#include <algorithm>
#include <queue>

#include "state.h"

void Solver::Backtrack(const State& end,
                       const std::unordered_map<State, State>& parent,
                       std::vector<std::pair<int, int>>& solution) const {
  State x = end;
  while (true) {
    State p = parent.at(x);
    if (p == x) {
      break;
    }
    std::pair<int, int> from_to = p.HowTo(x);
    solution.push_back(from_to);
    x = p;
  }
  std::reverse(solution.begin(), solution.end());
}

bool Solver::Solve(const State& initial_state,
                   std::vector<std::pair<int, int>>& solution) const {
  std::queue<State> q;
  q.push(initial_state);

  std::unordered_map<State, State> parent;
  parent[initial_state] = initial_state;

  solution.clear();
  if (initial_state.Done()) {
    return true;
  }

  while (!q.empty()) {
    State x = q.front();
    q.pop();

    State old_x = x;
    for (int from = 0; from < x.NumTubes(); from++) {
      for (int to = 0; to < x.NumTubes(); to++) {
        if (from == to) {
          continue;
        }
        if (x.Pour(from, to)) {
          if (parent.count(x) == 0) {
            q.push(x);
            parent[x] = old_x;
            if (x.Done()) {
              Backtrack(x, parent, solution);
              return true;
            }
          }
          x = old_x;
        }
      }
    }
  }

  return false;
}
