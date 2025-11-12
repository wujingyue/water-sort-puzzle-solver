#include <cassert>
#include <iostream>
#include <ranges>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "absl/flags/parse.h"
#include "absl/log/initialize.h"
#include "solve_api.h"

int main(int argc, char *argv[]) {
  absl::ParseCommandLine(argc, argv);
  absl::InitializeLog();

  int num_tubes;
  int volume;
  std::cin >> num_tubes >> volume;
  std::string line;
  getline(std::cin, line);

  std::vector<std::vector<int>> tubes(num_tubes, std::vector<int>());
  std::vector<std::string> colors;
  colors.reserve(num_tubes);
  std::unordered_map<std::string, int> color_id;
  for (int i = 0; i < num_tubes; i++) {
    tubes[i].reserve(volume);

    if (!getline(std::cin, line)) {
      break;
    }

    std::istringstream iss(line);
    std::string color;
    while (iss >> color) {
      auto [iter, inserted] = color_id.try_emplace(color, std::ssize(colors));
      if (inserted) {
        colors.push_back(color);
      }
      tubes[i].push_back(iter->second);
    }
  }

  const absl::StatusOr<Solution> solution = Solve(tubes, volume);
  if (!solution.ok()) {
    std::cout << "ID -> color:" << std::endl;
    for (auto [id, color] : std::views::enumerate(colors)) {
      std::cout << "  " << id << " -> " << color << std::endl;
    }
    std::cout << solution.status().message() << std::endl;
    return 1;
  }

  for (const auto &[move, step] : std::views::enumerate(*solution)) {
    std::cout << "Move " << move + 1 << ": " << "Pouring `"
              << colors.at(step.color_id) << "` from tube " << step.from + 1
              << " to tube " << step.to + 1 << std::endl;
  }

  return 0;
}
