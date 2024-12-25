#include <algorithm>
#include <cstdio>
#include <fstream>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_set>
#include <utility>
#include <vector>

template <typename T> struct robot_t {
  std::pair<T, T> pos;
  std::pair<T, T> velocity;
};

std::vector<std::string> split(std::string str, char del) {
  const auto del_idx{str.find(del)};

  return {str.substr(0, del_idx), str.substr(del_idx + 1, str.size())};
};

std::vector<robot_t<int>> parse_robots(std::ifstream &&fstream) {
  std::vector<robot_t<int>> robots{};

  const auto csv_int_pair{[](std::string const &str) -> std::pair<int, int> {
    const auto parts{split(str, ',')};

    return {std::stoi(parts[0]), std::stoi(parts[1])};
  }};

  std::string line;
  while (std::getline(fstream, line)) {
    const auto parts{split(line, ' ')};

    robots.push_back({

        csv_int_pair(parts[0].substr(2, parts[0].size())),
        csv_int_pair(parts[1].substr(2, parts[1].size()))});
  }

  return robots;
}

template <size_t width, size_t height>
std::vector<robot_t<int>> sim_bathroom(std::vector<robot_t<int>> &&robots,
                                       size_t steps) {
  constexpr int width_i = static_cast<int>(width);
  constexpr int height_i = static_cast<int>(height);

  const auto sim_step{[](robot_t<int> const &robot) -> robot_t<int> {
    std::pair<int, int> new_pos{robot.pos.first + robot.velocity.first,
                                robot.pos.second + robot.velocity.second};

    if (new_pos.first < 0) {
      new_pos.first += width_i;
    }

    if (new_pos.second < 0) {
      new_pos.second += height_i;
    }

    if (new_pos.first > width_i - 1) {
      new_pos.first -= width_i;
    }

    if (new_pos.second > height_i - 1) {
      new_pos.second -= height_i;
    }

    return {new_pos, robot.velocity};
  }};

  std::ranges::for_each(std::views::iota(0ul, steps), [&](int) {
    for (const auto [idx, robot] : robots | std::views::enumerate) {
      robots[idx] = sim_step(robot);
    }
  });

  return robots;
};

// int so compiler shuts up
template <int width, int height>
  requires(width % 2 == 1) && (height % 2 == 1)
std::vector<int> safety_factor(std::vector<robot_t<int>> const &robots) {
  constexpr int width_dist = (width - 1) / 2;
  constexpr int height_dist = (height - 1) / 2;

  std::vector<int> quads{0, 0, 0, 0};

  for (const auto &robot : robots) {
    if (robot.pos.first == width_dist || robot.pos.second == height_dist) {
      continue;
    }

    if (robot.pos.first > width_dist) {
      if (robot.pos.second > height_dist) {
        ++quads[3];
      } else {
        ++quads[2];
      }
    } else {
      if (robot.pos.second > height_dist) {
        ++quads[1];
      } else {
        ++quads[0];
      }
    }
  }

  return quads;
};

template <int width, int height>
void display_map(std::vector<robot_t<int>> robots, int day) {
  std::set<std::pair<int, int>> tiles{};

  std::for_each(robots.cbegin(), robots.cend(),
                [&tiles](auto const &robot) { tiles.emplace(robot.pos); });


  // got tired of waiting
  if (tiles.size() != robots.size()) {
    std::printf("%d no solution.\n", day);
    return;
  }

  for (const auto x : std::views::iota(0, width)) {
    for (const auto y : std::views::iota(0, height)) {
      if (tiles.contains({x, y})) {
        putchar('.');
      } else {
        putchar(' ');
      }
    }

    putchar('\n');
  }

  std::printf("day %d above:\n", day);
  std::this_thread::sleep_for(std::chrono::milliseconds(50000));
};

#ifdef MAIN
int main() {
  std::ifstream stream("inputs/14.txt");

  auto robots{parse_robots(std::move(stream))};

  // skip to a day that i thought i saw a tree...
  const auto inital_offset{1617};
  robots = sim_bathroom<101, 103>(std::move(robots), inital_offset);

  std::ranges::for_each(std::views::iota(0), [&](int n) {
    robots = sim_bathroom<101, 103>(std::move(robots), 1);

    display_map<101, 103>(robots, n + inital_offset);
  });
}
#endif
