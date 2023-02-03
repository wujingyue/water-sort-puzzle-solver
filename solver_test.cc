#include <iostream>

#include "absl/flags/parse.h"
#include "absl/log/initialize.h"
#include "gtest/gtest.h"

#include "solve_api.h"

class SolverTest : public testing::TestWithParam<Algorithm> {
};

TEST_P(SolverTest, Small) {
  const absl::StatusOr<std::vector<std::pair<int, int>>> solution =
      Solve({{1, 1, 1, 2}, {2, 2, 2, 1}, {}}, 4, GetParam());
  ASSERT_TRUE(solution.ok());
  EXPECT_EQ(solution->size(), 3);
}

TEST_P(SolverTest, ZeroMoves) {
  const absl::StatusOr<std::vector<std::pair<int, int>>> solution =
      Solve({{1, 1, 1, 1}, {2, 2, 2, 2}, {}}, 4, GetParam());
  ASSERT_TRUE(solution.ok());
  EXPECT_EQ(solution->size(), 0);
}

TEST_P(SolverTest, NoSolutions) {
  const absl::StatusOr<std::vector<std::pair<int, int>>> solution =
      Solve({{1, 2, 3, 4}, {1, 2, 3, 4}, {1, 2, 3, 4}, {1, 2, 3, 4}, {}}, 4,
            GetParam());
  ASSERT_FALSE(solution.ok());
}

TEST_P(SolverTest, Medium) {
  const absl::StatusOr<std::vector<std::pair<int, int>>> solution = Solve(
      {
          {8, 1, 1, 5},
          {4, 9, 6, 7},
          {7, 8, 2, 3},
          {5, 4, 8, 9},
          {8, 6, 1, 6},
          {5, 3, 3, 4},
          {7, 2, 5, 9},
          {2, 1, 9, 4},
          {7, 3, 6, 2},
          {},
          {},
      },
      4, GetParam());
  ASSERT_TRUE(solution.ok());
  EXPECT_EQ(solution->size(), 29);
}

TEST_P(SolverTest, Large) {
  const absl::StatusOr<std::vector<std::pair<int, int>>> solution = Solve(
      {
          {7, 7, 4, 8},
          {11, 5, 10, 9},
          {12, 1, 9, 3},
          {3, 1, 10, 3},
          {8, 2, 1, 4},
          {4, 12, 4, 2},
          {6, 1, 9, 12},
          {6, 10, 9, 6},
          {8, 7, 11, 5},
          {5, 11, 2, 12},
          {8, 6, 2, 11},
          {3, 10, 7, 5},
          {},
          {},
      },
      4, GetParam());
  ASSERT_TRUE(solution.ok());
  EXPECT_EQ(solution->size(), 41);
}

INSTANTIATE_TEST_SUITE_P(
    Algorithms, SolverTest,
    testing::ValuesIn({Algorithm::kAStar, Algorithm::kIterativeDeepeningAStar}),
    [](const testing::TestParamInfo<Algorithm>& info) {
      switch (info.param) {
        case Algorithm::kAStar:
          return "AStar";
        case Algorithm::kIterativeDeepeningAStar:
          return "IDAStar";
      }
      return "";
    });

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  absl::ParseCommandLine(argc, argv);
  absl::InitializeLog();
  return RUN_ALL_TESTS();
}
