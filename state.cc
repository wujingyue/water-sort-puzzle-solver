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

static int NumSegments(const std::vector<int>& v) {
  int num_segments = 0;
  for (size_t i = 0, size = v.size(); i < size; i++) {
    if (i == 0 || v[i] != v[i - 1]) {
      num_segments++;
    }
  }
  return num_segments;
}

void State::Pour(const int from, const int to, int water) {
  auto& from_tube = tubes_[from];
  auto& to_tube = tubes_[to];

  for (; water > 0; water--) {
    to_tube.push_back(from_tube.back());
    from_tube.pop_back();
  }
}

int State::Pour(const int from, const int to) {
  auto& from_tube = tubes_[from];
  auto& to_tube = tubes_[to];

  if ((int)from_tube.size() == volume_ && NumSegments(from_tube) == 1) {
    return 0;
  }

  if (to_tube.empty()) {
    for (int i = 0; i < to; i++) {
      if (tubes_[i].empty()) {
        return 0;
      }
    }
  }

  int water = 0;
  while (!from_tube.empty() && (int)to_tube.size() < volume_) {
    const int color = from_tube.back();
    if (!to_tube.empty() && color != to_tube.back()) {
      break;
    }

    from_tube.pop_back();
    to_tube.push_back(color);

    water++;
  }
  return water;
}

size_t State::Hash() const {
  size_t seed = NumTubes() * volume_;
  for (const auto& tube : tubes_) {
    const int water = tube.size();
    for (int i = 0; i < volume_; i++) {
      int x = (i < water ? tube[i] : 0);
      x = ((x >> 16) ^ x) * 0x45d9f3b;
      x = ((x >> 16) ^ x) * 0x45d9f3b;
      x = (x >> 16) ^ x;
      seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
  }
  return seed;
}

bool State::Done() const {
  for (const auto& tube : tubes_) {
    const int water = tube.size();
    if (water != 0 && water != volume_) {
      return false;
    }
    if (NumSegments(tube) > 1) {
      return false;
    }
  }
  return true;
}

int State::EstimatedCost() const {
  int water = 0;
  int num_segments = 0;
  for (const auto& tube : tubes_) {
    water += tube.size();
    num_segments += NumSegments(tube);
  }
  assert(water % volume_ == 0);
  return num_segments - water / volume_;
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
