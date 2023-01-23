#ifndef _STATE_H
#define _STATE_H

#include <cstddef>
#include <utility>
#include <vector>

class State {
 public:
  State() {}
  State(const std::vector<std::vector<int>>& tubes, int volume);
  bool Pour(int from, int to);
  size_t Hash() const;
  bool Done() const;
  int NumTubes() const { return tubes_.size(); }
  int Volume() const { return volume_; }
  bool operator==(const State& other) const {
    return tubes_ == other.tubes_;
  }
  void Dump() const;
  std::pair<int, int> HowTo(const State& other) const;

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
