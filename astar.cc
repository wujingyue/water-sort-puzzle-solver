#include "astar.h"

#include <algorithm>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "absl/log/log.h"

struct StateAndFScore {
  State state;
  int f_score;

  bool operator<(const StateAndFScore& other) const {
    return f_score > other.f_score;
  }
};

bool AStar::Solve(
    const State& initial_state, std::vector<std::pair<int, int>>& solution) {
  std::priority_queue<StateAndFScore> pq;
  std::unordered_map<State, int> shortest_moves;
  std::unordered_map<State, Operation> reaching_op;

  pq.push({initial_state, initial_state.EstimatedMovesToEnd()});
  shortest_moves[initial_state] = 0;
  reaching_op[initial_state] = {-1, -1, 0};

  while (!pq.empty()) {
    State x = pq.top().state;
    pq.pop();

    auto i_x = shortest_moves.find(x);
    const int moves_from_start = i_x->second;
    if (moves_from_start == std::numeric_limits<int>::min()) {
      continue;
    }
    i_x->second = std::numeric_limits<int>::min();

    if (x.Done()) {
      LOG(INFO) << "Visited " << shortest_moves.size() << " states.";
      ReconstructPath(x, reaching_op, solution);
      return true;
    }

    for (int from = 0; from < x.NumTubes(); from++) {
      for (int to = 0; to < x.NumTubes(); to++) {
        if (from == to) {
          continue;
        }
        if (const int water = x.Pour(from, to); water > 0) {
          if (shortest_moves.count(x) == 0 ||
              shortest_moves.at(x) > moves_from_start + 1) {
            pq.push({x, moves_from_start + 1 + x.EstimatedMovesToEnd()});
            shortest_moves[x] = moves_from_start + 1;
            reaching_op[x] = {from, to, water};
          }
          x.Pour(to, from, water);
        }
      }
    }
  }

  return false;
}

void AStar::ReconstructPath(
    State x, const std::unordered_map<State, Operation>& reaching_op,
    std::vector<std::pair<int, int>>& solution) {
  while (true) {
    const auto& op = reaching_op.at(x);
    if (op.from == -1 && op.to == -1) {
      break;
    }
    solution.push_back({op.from, op.to});
    x.Pour(op.to, op.from, op.water);
  }

  std::reverse(solution.begin(), solution.end());
}
