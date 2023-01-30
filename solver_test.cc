#include <iostream>

#include <gtest/gtest.h>

#include "solver.h"

TEST(SolverTest, Small) {
  Solver solver(10);
  const absl::StatusOr<std::vector<std::pair<int, int>>> solution =
      solver.Solve({{1, 1, 1, 2}, {2, 2, 2, 1}, {}}, 4);
  ASSERT_TRUE(solution.ok());
  EXPECT_EQ(solution->size(), 3);
}

TEST(SolverTest, NoSolutions) {
  Solver solver(50);
  const absl::StatusOr<std::vector<std::pair<int, int>>> solution =
      solver.Solve({{1, 2, 3, 4}, {1, 2, 3, 4}, {1, 2, 3, 4}, {1, 2, 3, 4}, {}},
                   4);
  ASSERT_FALSE(solution.ok());
}

TEST(SolverTest, Medium) {
  Solver solver(50);
  const absl::StatusOr<std::vector<std::pair<int, int>>> solution =
      solver.Solve(
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
          4);
  ASSERT_TRUE(solution.ok());
  EXPECT_EQ(solution->size(), 29);
}

TEST(SolverTest, Large) {
  Solver solver(50);
  const absl::StatusOr<std::vector<std::pair<int, int>>> solution =
      solver.Solve(
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
          4);
  ASSERT_TRUE(solution.ok());
  EXPECT_EQ(solution->size(), 41);
}
