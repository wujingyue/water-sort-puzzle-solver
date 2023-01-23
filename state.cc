#include "state.h"

#include <cassert>
#include <iostream>
#include <unordered_map>

State::State(const std::vector<std::vector<int>>& tubes, const int volume)
    : tubes_(tubes), volume_(volume) {
  std::unordered_map<int, int> volume_of_color;
  assert(volume > 0);
  for (auto& tube : tubes_) {
    assert((int)tube.size() <= volume);
    tube.reserve(volume);
    for (const int color : tube) {
      volume_of_color[color]++;
    }
  }
  for (const auto& [color, v] : volume_of_color) {
    (void)color;
    assert(v == volume);
  }
}

static bool OneColor(const std::vector<int>& v) {
  for (size_t i = 1, size = v.size(); i < size; i++) {
    if (v[i] != v[i - 1]) {
      return false;
    }
  }
  return true;
}

bool State::Pour(const int from, const int to) {
  auto& from_tube = tubes_[from];
  auto& to_tube = tubes_[to];

  if ((int)from_tube.size() == volume_ && OneColor(from_tube)) {
    return false;
  }

  if (to_tube.empty()) {
    for (int i = 0; i < to; i++) {
      if (tubes_[i].empty()) {
        return false;
      }
    }
  }

  bool changed = false;
  while (!from_tube.empty() && (int)to_tube.size() < volume_) {
    const int color = from_tube.back();
    if (!to_tube.empty() && color != to_tube.back()) {
      break;
    }

    from_tube.pop_back();
    to_tube.push_back(color);

    changed = true;
  }
  return changed;
}

size_t State::Hash() const {
  size_t seed = NumTubes() * volume_;
  for (const auto& tube : tubes_) {
    for (int x : tube) {
      x = ((x >> 16) ^ x) * 0x45d9f3b;
      x = ((x >> 16) ^ x) * 0x45d9f3b;
      x = (x >> 16) ^ x;
      seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    for (int i = tube.size(); i < volume_; i++) {
      seed ^= 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
  }
  return seed;
}

bool State::Done() const {
  for (const auto& tube : tubes_) {
    if (!tube.empty() && (int)tube.size() < volume_) {
      return false;
    }
    for (size_t i = 1, size = tube.size(); i < size; i++) {
      if (tube[i] != tube[i - 1]) {
        return false;
      }
    }
  }
  return true;
}

void State::Dump() const {
  for (const auto& tube : tubes_) {
    std::cerr << "|";
    for (const int color : tube) {
      std::cerr << " " << color;
    }
    std::cerr << std::endl;
  }
}

std::pair<int, int> State::HowTo(const State& other) const {
  int from = -1;
  int to = -1;
  assert(NumTubes() == other.NumTubes());
  for (int i = 0; i < NumTubes(); i++) {
    if (tubes_[i].size() < other.tubes_[i].size()) {
      assert(to == -1);
      to = i;
    } else if (tubes_[i].size() > other.tubes_[i].size()) {
      assert(from == -1);
      from = i;
    }
  }
  assert(from != -1);
  assert(to != -1);
  return {from, to};
}
