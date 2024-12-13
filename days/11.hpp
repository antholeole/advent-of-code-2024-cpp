#include <cstdint>
#include <fstream>
#include <future>
#include <map>
#include <ranges>
#include <string>
#include <vector>

struct stone_processor {
  inline uint64_t process_stone(uint64_t stone, int iters) {
    const std::pair<uint64_t, int> pair{stone, iters};
    if (known_stones.contains(pair)) {
      return known_stones.at(pair);
    }

    uint64_t ret{0};
    const std::string stone_str(std::to_string(stone));
    if (iters == 0) {
      ret = 1;
    } else if (stone == 0) {
      ret = process_stone(1, iters - 1);
    } else if (stone_str.size() % 2 == 0) {
      const auto mid{stone_str.size() / 2};
      const auto stone_a{std::stoll(stone_str.substr(0, mid))};
      const auto stone_b{std::stoll(stone_str.substr(mid, stone_str.size()))};
      ret =
          process_stone(stone_a, iters - 1) + process_stone(stone_b, iters - 1);
    } else {
      ret = process_stone(stone * 2024, iters - 1);
    }

    known_stones[pair] = ret;
    return ret;
  };

private:
  std::map<std::pair<uint64_t, int>, uint64_t> known_stones{};
};

inline uint64_t stone_blink(std::vector<uint64_t> const &stones, const int iters) {
  uint64_t acc{0};
  stone_processor sp{};
  for (const auto stone : stones) {
    acc += sp.process_stone(stone, iters);
  }

  return acc;
}

#ifdef MAIN
int main() {
  std::ifstream stream("inputs/11.txt");
  std::string whole_input(std::istreambuf_iterator<char>(stream), {});

  std::vector<uint64_t> input{};
  for (const auto &&f : whole_input | std::views::split(' ')) {
    input.push_back(std::stoll(std::string_view(f).data()));
  }

  const auto res1{stone_blink(input, 25)};
  std::printf("pt1: %lu\n", res1);

  const auto res2{stone_blink(input, 75)};
  std::printf("pt2: %lu\n", res2);
}
#endif
