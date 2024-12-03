#include <fstream>
#include <regex>
#include <string>

int main() {
  std::ifstream fstream{"inputs/3.txt"};
  std::string whole_input(std::istreambuf_iterator<char>(fstream), {});
  whole_input = "do()" + whole_input;
  const std::regex input_regexp(
      "do\\(\\)(?:(?!don't\\(\\)).)*mul\\(([0-9][0-9]?[0-9]?),([0-9][0-9]?[0-9]?)\\)");
  std::smatch matches;

  auto ans{0};
  std::string::const_iterator search_start(whole_input.cbegin());
  while (
      regex_search(search_start, whole_input.cend(), matches, input_regexp)) {

    const auto first{std::stoi(matches[1])};
    const auto second{std::stoi(matches[2])};
    ans += first * second;
    search_start = matches.suffix().first;
  }

  std::printf("ans: %d\n", ans);
}