#ifndef _STATE_H
#define _STATE_H

#include <cstddef>
#include <utility>
#include <vector>

#include "absl/status/statusor.h"

class State {
 public:
  static absl::StatusOr<State> Create(
      const std::vector<std::vector<int>>& tubes, int volume);

  int NumTubes() const { return tubes_.size(); }
  int Volume() const { return volume_; }

  int Pour(int from, int to);
  void Pour(int from, int to, int amount);

  size_t Hash() const;

  bool Done() const;

  int EstimatedCost() const;

  bool operator==(const State& other) const {
    return tubes_ == other.tubes_;
  }

  std::string DebugString() const;

 private:
  std::vector<std::vector<int>> tubes_;
  int volume_;
};

namespace std {
template <>
struct hash<State> {
  size_t operator()(const State& s) const {
    return s.Hash();
  }
};
}  // namespace std

#endif  // _STATE_H
