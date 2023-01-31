#include <cassert>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "solver.h"

int main(int argc, char* argv[]) {
  int num_tubes;
  int volume;
  std::cin >> num_tubes >> volume;
  std::string line;
  getline(std::cin, line);

  std::vector<std::vector<int>> tubes(num_tubes, std::vector<int>());
  std::unordered_map<std::string, int> color_id;
  for (int i = 0; i < num_tubes; i++) {
    tubes[i].reserve(volume);

    if (!getline(std::cin, line)) {
      break;
    }

    std::istringstream iss(line);
    std::string color;
    while (iss >> color) {
      int& id = color_id[color];
      if (id == 0) {
        id = color_id.size();
      }
      tubes[i].push_back(id);
    }
  }

  Solver solver;
  const absl::StatusOr<std::vector<std::pair<int, int>>> solution =
      solver.Solve(tubes, volume);
  if (!solution.ok()) {
    std::cout << "Color -> ID:" << std::endl;
    for (const auto& [color, id] : color_id) {
      std::cout << "  " << color << " -> " << id << std::endl;
    }
    std::cout << solution.status().message() << std::endl;
    return 1;
  }

  int move = 0;
  for (const auto& [from, to]: *solution) {
    move++;
    std::cout << "Move " << move << ": " << from + 1 << " " << to + 1
              << std::endl;
  }

  return 0;
}
