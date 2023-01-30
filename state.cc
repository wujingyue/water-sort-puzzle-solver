#include "state.h"

#include <iostream>
#include <unordered_map>

#include "absl/log/check.h"
#include "absl/strings/str_cat.h"

/*static*/ absl::StatusOr<State> State::Create(
    const std::vector<std::vector<int>>& tubes, const int volume) {
  // Sanity checks.
  if (volume <= 0) {
    return absl::InvalidArgumentError(absl::StrCat(
        "`volume` is expected to be positive, but ", volume, " was given."));
  }

  std::unordered_map<int, int> volume_of_color;
  for (size_t i = 0; i < tubes.size(); i++) {
    const auto& tube = tubes[i];
    if ((int)tube.size() > volume) {
      return absl::InvalidArgumentError(
          absl::StrCat("The height of tube ", i, " (", tube.size(),
                       ") is larger than the given volume (", volume, ")."));
    }
    for (const int color : tube) {
      volume_of_color[color]++;
    }
  }
  for (const auto& [color, v] : volume_of_color) {
    if (v != volume) {
      return absl::InvalidArgumentError(
          absl::StrCat("The volume of color ", color, " (", v,
                       ") does not equal the given volume (", volume, ")."));
    }
  }

  State s;
  s.tubes_ = tubes;
  s.volume_ = volume;
  for (auto& tube : s.tubes_) {
    tube.reserve(volume);
  }
  return s;
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

  // It's pointless to pour from a full and one-color tube.
  if ((int)from_tube.size() == volume_ && NumSegments(from_tube) == 1) {
    return 0;
  }

  if (to_tube.empty()) {
    // If there are multiple empty tubes, pour into the first one to reduce
    // state redundancy.
    for (int i = 0; i < to; i++) {
      if (tubes_[i].empty()) {
        return 0;
      }
    }

    // It's pointless to pour from a one-color tube to an empty tube.
    if (NumSegments(from_tube) == 1) {
      return 0;
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
  // The idea is taken from https://stackoverflow.com/a/72073933.
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
  CHECK(water % volume_ == 0);
  return num_segments - water / volume_;
}

std::string State::DebugString() const {
  std::string str;
  for (const auto& tube : tubes_) {
    absl::StrAppend(&str, "|");
    for (const int color : tube) {
      absl::StrAppend(&str, " ", color);
    }
    absl::StrAppend(&str, "\n");
  }
  return str;
}
