#include <iostream>

#include <gtest/gtest.h>

#include "solver.h"

TEST(SolverTest, Simple) {
  Solver solver;
  std::vector<std::pair<int, int>> solution;
  bool succeeded =
      solver.Solve(State({{1, 1, 1, 2}, {2, 2, 2, 1}, {}}, 4), solution);
  ASSERT_TRUE(succeeded);
  EXPECT_EQ(solution.size(), 3);
}
