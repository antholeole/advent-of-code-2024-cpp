#include <fstream>
#include <queue>
#include <ranges>
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include <unordered_set>
#include "../lib/file_views.hpp"

// the list is sizeof(t) < 4 ∀ t ∈ T, but
// use 64 bit ints for easy strtoi.
using list_id_t = u_int64_t;
// num of chars per id in the input list.
constexpr auto list_id_chars = 5;
const auto line_to_ints = [](auto &lsv) {
  return std::tuple<list_id_t, list_id_t>(
      std::stoi(lsv.get().substr(0, list_id_chars + 1)),
      std::stoi(lsv.get().substr(list_id_chars)));
};
// part 1
u_int64_t calc_delta(std::ifstream &&fstream) {
  std::priority_queue<u_int32_t> pq_l;
  std::priority_queue<u_int32_t> pq_r;
  const auto get_pq_delta = [&pq_l, &pq_r]() -> u_int64_t {
    if (pq_l.top() > pq_r.top()) {
      return pq_l.top() - pq_r.top();
    } else {
      return pq_r.top() - pq_l.top();
    }
  };
  for (const auto [l, r] : std::views::istream<lsv_file_view>(fstream) |
                               std::views::transform(line_to_ints)) {
    pq_l.push(l);
    pq_r.push(r);
  }
  u_int64_t total_delta{0};
  while (pq_l.size() && pq_r.size()) {
    total_delta += get_pq_delta();
    pq_l.pop();
    pq_r.pop();
  }
  return total_delta;
}
// part 2
u_int64_t calc_dist(std::ifstream &&fstream) {
  std::unordered_set<list_id_t> appears_in_left{};
  std::unordered_map<list_id_t, size_t> right_freq{};
  for (const auto [l, r] : std::views::istream<lsv_file_view>(fstream) |
                               std::views::transform(line_to_ints)) {
    appears_in_left.insert(l);
    right_freq[r] += 1;
  }
  u_int64_t similarity_score{0};
  for (const auto left_element : appears_in_left) {
    // if it does not exist in right, the default 0 is what we want.
    similarity_score += left_element * right_freq[left_element];
  }
  return similarity_score;
}
int main() {
  std::printf("total delta: %lu\n", calc_delta(std::ifstream{"inputs/1.txt"}));
  std::printf("sim score: %lu\n", calc_dist(std::ifstream{"inputs/1.txt"}));
}