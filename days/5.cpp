
#include <algorithm>
#include <fstream>
#include <ranges>
#include <set>
#include <unordered_map>
#include <vector>

#include "../lib/file_views.hpp"

struct page_info {
  std::unordered_map<int, std::set<int>> prefix_map;
  std::vector<std::vector<int>> updates;
};

page_info read_page_info() {
  page_info page_info{};
  std::ifstream fstream{"inputs/5.txt"};
  auto lines{std::views::istream<lsv_file_view>(fstream) |
             std::views::transform(
                 [](auto &lsv) -> std::string & { return lsv.get(); })};
  auto prefix_lines =
      lines | std::views::take_while([](auto &str) { return !str.empty(); });

  for (auto &prefix_line : prefix_lines) {
    const auto space_idx{prefix_line.find('|')};
    const auto before{std::stoi(prefix_line.substr(0, space_idx))};
    const auto after{
        std::stoi(prefix_line.substr(space_idx + 1, prefix_line.size()))};

    page_info.prefix_map[before].insert(after);
  }

  for (auto &update_line : lines) {
    std::vector<int> update;
    for (auto page : update_line | std::views::split(',')) {
      update.push_back(std::stoi(std::string_view(page).data()));
    }
    page_info.updates.push_back(update);
  }

  return page_info;
}

bool is_valid_update(page_info &info, std::vector<int> const &update) {
  bool valid{true};
  for (uint i = 0; i < update.size(); ++i) {
    for (uint j = i; j < update.size(); ++j) {
      if (info.prefix_map[update[j]].contains(update[i])) {
        valid = false;
        break;
      }
    }

    if (!valid) {
      break;
    }
  }

  return valid;
}

int get_middle_page_sums(auto &&updates) {
  int middle_page_sum{0};
  for (const auto &v : updates) {
    middle_page_sum += v[v.size() / 2];
  }

  return middle_page_sum;
}

std::vector<int> sort_update(page_info &page_info,
                             std::vector<int> const &update) {
  std::vector<int> sorted(update);
  std::ranges::sort(sorted, [&page_info](int a, int b){
    return page_info.prefix_map[a].contains(b);
  });
  return sorted;
}

int main() {
  auto page_info = read_page_info();

  const auto build_valid_page_filter{[&page_info](bool filter_valid) {
    return std::views::filter(
        [&page_info, filter_valid](std::vector<int> const &update) {
          return filter_valid == is_valid_update(page_info, update);
        });
  }};

  const auto valid_page_sum{
      get_middle_page_sums(page_info.updates | build_valid_page_filter(true))};
  std::printf("valid middle page sums: %d\n", valid_page_sum);

  const auto invalid_sorted_page_sum{get_middle_page_sums(
      page_info.updates | build_valid_page_filter(false) |
      std::views::transform([&page_info](std::vector<int> &update) {
        return sort_update(page_info, update);
      }))};

  std::printf("invalid sorted middle page sums: %d\n", invalid_sorted_page_sum);
}